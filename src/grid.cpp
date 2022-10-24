#include "grid.h"
#include <QDebug>
#include <QtConcurrent>

const QSet<int> Cell::s_basePossibleSolutions = { 1, 2, 3, 4, 5, 6, 7, 8, 9};

Cell::Cell(const QString &id, int value, QObject *p)
    : QObject(p)
    , m_id(id)
    , m_oValue(value)
    , m_value(value)
    , m_currentPossibleSolutions(s_basePossibleSolutions)
{
    Q_ASSERT(value >= 0 && value <= 9);

    if (m_value > 0)
    {
        m_currentPossibleSolutions = { m_value };
    }
}

void Cell::setID(const QString &id)
{
    m_id = id;
}

QString Cell::getID() const
{
    return m_id;
}

void Cell::setValue(int value)
{
    Q_ASSERT(value >= 0 && value <= 9);

    if (m_value != value && m_value == 0)
    {
        m_value = value;
        m_currentPossibleSolutions = { m_value };

        emit slovedChanged(m_value);
    }
}

int Cell::getValue() const
{
    return m_value;
}

int Cell::getOValue() const
{
    return m_oValue;
}

bool Cell::removePossiableSolutions(QSet<int> solutions)
{
    bool removed = false;

    for (auto const &solution : solutions)
    {
        removed |= removePossiableSolution(solution);
    }

    return removed;
}

bool Cell::removePossiableSolution(int solution)
{
    bool removed = false;
    if (!isSolved())
    {
        removed = m_currentPossibleSolutions.remove(solution);
        if (m_currentPossibleSolutions.size() == 1)
        {
            qDebug() << "Naked Single:" << getID() << "by removing" << solution <<  m_currentPossibleSolutions.values().first();
            setValue(m_currentPossibleSolutions.values().first());
        }
    }
    return removed;
}

const QSet<int> Cell::getPossibleSolutions() const
{
    return m_currentPossibleSolutions;
}

bool Cell::hiddenSingle()
{
    bool foundHiddenSignle = false;
    for (const auto &section : m_sections)
    {
        auto justOne = getPossibleSolutions();
        for (const auto &cell : *section)
        {
            if (cell->getID() != getID())
            {
                justOne.subtract(cell->getPossibleSolutions());
            }
        }

        if (justOne.size() == 1)
        {
            qDebug() << "Hidden Single:" << getID() << justOne.values().first();
            setValue(justOne.values()[0]);
            foundHiddenSignle = true;
            break;
        }
    }

    return foundHiddenSignle;
}

bool Cell::pointingPairs()
{
    bool foundHiddenPair = false;

    if (getPossibleSolutions().size() >= 2)
    {
        auto const possibilities = getPossibleSolutions();
        for (const auto &section : m_sections)
        {
            QMap<int, Section> possablitiesInOtherCells;
            for (const auto &possibility : possibilities)
            {
                possablitiesInOtherCells.insert(possibility, {});
            }

            for (const auto &cell : *section)
            {
                if (cell->getID() != getID() && // Cannot be this cell
                   !cell->isSolved())         // No new infmation from an already solved cell
                {
                    for (const auto& cellP : cell->getPossibleSolutions())
                    {
                        if (possablitiesInOtherCells.contains(cellP))
                        {
                            possablitiesInOtherCells[cellP].append(cell);
                        }
                    }
                }
            }

            // Once gathered all the cells that share the same possiblity as this one
            // Is there a cell:
            // 1. there is one other cell the share a possiblity
            // 2. that share another section to this one
            for (auto &valueCell : possablitiesInOtherCells.toStdMap())
            {
                if (valueCell.second.size() == 1)
                {
                    auto pair = valueCell.second.first();
                    for (auto &anotherSection : m_sections)
                    {
                        if (anotherSection != section && pair->m_sections.contains(anotherSection))
                        {
                            QList<QString> cellsUpdated;
                            // Found a pair in two sections and need to clean up the other section
                            for (const auto &cell : *anotherSection)
                            {
                                if (cell->getID() != getID() && cell->getID() != pair->getID())
                                {
                                    if (cell->removePossiableSolution(valueCell.first))
                                    {
                                        cellsUpdated << cell->getID();
                                        foundHiddenPair = true;
                                    }
                                }
                            }
                            if (foundHiddenPair)
                            {
                                qDebug() << "Pointing Pairs:" << getID() << "and" << pair->getID() << valueCell.first << cellsUpdated;
                                break;
                            }
                        }
                    }

                    if (foundHiddenPair)
                    {
                        break;
                    }
                }
            }

            if (foundHiddenPair)
            {
                break;
            }
        }
    }

    return foundHiddenPair;
}

bool Cell::nakedPair()
{
    bool foundNakedPair = false;
    if (getPossibleSolutions().size() == 2)
    {
        auto const possibilities = getPossibleSolutions();
        for (const auto &section : m_sections)
        {
            Section matchingPair;
            for (const auto &cell : *section)
            {
                if (cell->getID() != getID() && cell->getPossibleSolutions() == possibilities)
                {
                    matchingPair << cell;
                }
            }

            Q_ASSERT(matchingPair.size() < 2);
            if (matchingPair.size() == 1)
            {
                QList<QString> idsChanged;
                for (const auto &cell : *section)
                {
                    if (cell->getID() != getID() &&
                        cell->getID() != matchingPair.first()->getID())
                    {
                        if (cell->removePossiableSolutions(possibilities))
                        {
                            idsChanged << cell->getID();
                            foundNakedPair = true;
                        }
                    }
                }

                if (foundNakedPair)
                {
                    qDebug() << "Naked Pairs:" << getID() << "and" << matchingPair.first()->getID() << possibilities << idsChanged;
                }
            }
        }
    }
    return foundNakedPair;
}

bool Cell::hiddenPair()
{
    bool foundHiddenPair = false;
    if (getPossibleSolutions().size() >= 2)
    {
        auto const possibilities = getPossibleSolutions();
        for (const auto &section : m_sections)
        {
            const auto refSection = *section;
            for (int i = 0; i < refSection.size(); ++i)
            {
                const auto compareCell = refSection[i];
                auto comarePairPossiblities = compareCell->getPossibleSolutions();
                comarePairPossiblities.intersect(possibilities);
                if (compareCell->getID() != getID() && comarePairPossiblities.size() == 2)
                {
                    Section listOfCellsWithPair;
                    for (int j = 0; j < refSection.size(); ++j)
                    {
                        const auto checkCell = refSection[j];
                        if (checkCell->getID() != getID() && checkCell->getID() != compareCell->getID() &&
                            checkCell->getPossibleSolutions().intersects(comarePairPossiblities))
                        {
                            listOfCellsWithPair << checkCell;
                        }
                    }

                    // No other cells has the pair, now remove the other possiblities from these two cells
                    if (listOfCellsWithPair.size() == 0)
                    {
                        auto const removeMy = possibilities - comarePairPossiblities;
                        removePossiableSolutions(removeMy);
                        auto const removeTheirs = compareCell->getPossibleSolutions() - comarePairPossiblities;
                        compareCell->removePossiableSolutions(removeTheirs);

                        if (removeMy.size() > 0 || removeTheirs.size() > 0)
                        {
                            qDebug() << "Found hidden pair:" << getID() << "and" << compareCell->getID() << comarePairPossiblities << removeMy << removeTheirs;
                            foundHiddenPair = true;
                        }
                    }
                }
            }
        }
    }
    return foundHiddenPair;
}

Section Cell::getLockedPairs()
{
    return Section();
}

bool Cell::xWing()
{
    bool foundXWing = false;

    auto lockedPairs = getLockedPairs();
    for (auto pair : lockedPairs)
    {

    }

    return foundXWing;
}

bool Cell::updatePossibleSolutions(Strategies stragtegy)
{
    if (!isSolved())
    {
        switch (stragtegy)
        {
        case Strategies::HiddenSingle:
            return hiddenSingle();
        case Strategies::PointingPair:
            return pointingPairs();
        case Strategies::NakedPair:
            return nakedPair();
        case Strategies::HiddenPair:
            return hiddenPair();
        case Strategies::XWing:
            return xWing();
        case Strategies::END_OF_STRATEGIES:
        default:
            Q_ASSERT(false);
        }
    }

    return false;

}

bool Cell::isSolved() const
{
    return m_value > 0;
}

void Cell::addSection(Section *section)
{
    m_sections << section;

    for (auto &cell : *section)
    {
        if (cell->getID() != getID())
        {
            connect(cell, &Cell::slovedChanged, this, &Cell::removePossiableSolution);

            if (cell->isSolved())
            {
                removePossiableSolution(cell->getValue());
            }
        }
    }
}

void Cell::addSectionBlock(Section *section)
{
    addSection(section);
    m_block = section;
}

Grid::Grid(int id, const QString &gridData, QObject *p)
    : QObject(p)
    , m_id(id)
    , m_version(QString::number(id))
{
    for (int row = 0; row < 9; ++row)
    {
        QString idRow = QString("abcdefghijklmnopqrstuvwxyz").at(row);
        idRow = idRow.toUpper();
        for (int column = 0; column < 9; ++column)
        {
            int cellNumber = row * 9 + column;
            int value = QString(gridData[cellNumber]).toInt();
            QString id = idRow + QString::number(column + 1);

            m_grid[row][column] = new Cell(id, value, this);
        }
    }


    setCellsSections();
}

// Grid::Grid(const Grid &oldGrid,
//      int rowToSet, int columnToSet, int number)
//     : m_id(oldGrid.m_id)
//     , m_version(oldGrid.m_version + ".{" + QString::number(rowToSet) + "," + QString::number(columnToSet) + "," + QString::number(number) + "}")
// {
//     m_grid.clear();
//     for (int row = 0; row < 9; ++row)
//     {
//         m_grid.append(QList<QSharedPointer<Cell>>({}));
//         for (int column = 0; column < 9; ++column)
//         {
//             if (row == rowToSet && columnToSet == column)
//             {
//                 cell = QSharedPointer<Cell>::create(number);
//             }
//             else
//             {
//                 auto oldCell = oldGrid.m_grid[row][column];
//                 assert(oldCell);
//
//                 cell = QSharedPointer<RealCell>::create(oldCell->getValue());
//             }
//
//             m_grid[row].append(cell);
//         }
//     }
//     createSections();
// }

Grid::~Grid()
{
    // qDebug() << "Failed on" << m_id << Qt::endl << *this;
}

void Grid::setCellsSections()
{
    for (int row = 0; row < 9; ++row)
    {
        for (int column = 0; column < 9; ++column)
        {
            const int blockIndex = (row / 3) * 3 + column / 3;

            auto cell = m_grid[row][column];

            m_rows[row].append(cell);
            m_columns[column].append(cell);
            m_blocks[blockIndex].append(cell);
        }

    }

    for (int row = 0; row < 9; ++row)
    {
        for (int column = 0; column < 9; ++column)
        {
            const int blockIndex = (row / 3) * 3 + column / 3;

            m_grid[row][column]->addSection(&m_rows[row]);
            m_grid[row][column]->addSection(&m_columns[column]);
            m_grid[row][column]->addSectionBlock(&m_blocks[blockIndex]);
        }
    }
}

bool Grid::isSolved()
{
    bool isSolved = true;
    for (const auto &row : m_grid)
    {
        for (const auto &cell : row)
        {
            isSolved &= cell->isSolved();
            if (!isSolved)
            {
                break;
            }
        }
    }

    if (isSolved)
    {
        // These also better not have doubles
        for (int i = 0; i < 9; ++i)
        {
            QSet<int> row = Cell::s_basePossibleSolutions;
            QSet<int> column = Cell::s_basePossibleSolutions;
            QSet<int> block = Cell::s_basePossibleSolutions;

            for (int j = 0; j < 9; ++j)
            {
                isSolved &= row.remove(m_rows[i][j]->getValue());
                isSolved &= column.remove(m_columns[i][j]->getValue());
                isSolved &= block.remove(m_blocks[i][j]->getValue());

                if (!isSolved)
                {
                    qDebug() << *this;
                    Q_ASSERT(false);
                    break;
                }
            }
            if (!isSolved)
            {
                break;
            }
        }
    }

    return isSolved;
}

bool Grid::solve()
{
    bool somethingChanged = true;
    Cell::Strategies strategy = Cell::Strategies::EASIST_STRAATEGY;
    while (!isSolved() && somethingChanged)
    {
        somethingChanged = false;
        for (auto &row : m_grid)
        {
            for (auto &cell : row)
            {
                somethingChanged |= cell->updatePossibleSolutions(strategy);

                if (somethingChanged && strategy > Cell::Strategies::EASIST_STRAATEGY)
                {
                    break;
                }
            }

            if (somethingChanged && strategy > Cell::Strategies::EASIST_STRAATEGY)
            {
                strategy = Cell::Strategies::EASIST_STRAATEGY;
                break;
            }
        }

        if (strategy < Cell::Strategies::END_OF_STRATEGIES && !somethingChanged)
        {
            strategy = (Cell::Strategies)((int)strategy + 1);
        }

        if (strategy < Cell::Strategies::END_OF_STRATEGIES)
        {
            somethingChanged = true;
        }
    }

    return isSolved();
}

int Grid::topLeftSum() const
{
    // Q_ASSERT(solved());
    if (!m_grid[0][0]->getValue() ||
        !m_grid[0][1]->getValue() ||
        !m_grid[0][2]->getValue())
    {
        return 0;
    }
    return m_grid[0][0]->getValue() * 100 +
           m_grid[0][1]->getValue() * 10 +
           m_grid[0][2]->getValue();
}

QDebug operator<<(QDebug debug, const Grid &grid)
{

    debug << grid.m_version << Qt::endl;
    debug << "-------BEFORE--------" << Qt::endl;
    for (int row = 0; row < 9; ++row)
    {
        for (int column = 0; column < 9; column += 3)
        {
            debug  << grid.m_grid[row][column + 0]->getOValue()
                   << grid.m_grid[row][column + 1]->getOValue()
                   << grid.m_grid[row][column + 2]->getOValue()
                   << "|";
        }
        debug << Qt::endl;
        if (row == 2 || row == 5)
        {
            debug << "-----------------------" << Qt::endl;
        }
    }

    debug << "-------AFTER--------" << Qt::endl;
    for (int row = 0; row < 9; ++row)
    {
        for (int column = 0; column < 9; column += 3)
        {
            debug  << grid.m_grid[row][column + 0]->getValue()
                   << grid.m_grid[row][column + 1]->getValue()
                   << grid.m_grid[row][column + 2]->getValue()
                   << "|";
        }
        debug << Qt::endl;
        if (row == 2 || row == 5)
        {
            debug << "-----------------------" << Qt::endl;
        }
    }


    return debug;
}
