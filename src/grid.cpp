#include "grid.h"
#include <QDebug>

const QSet<int> Cell::startingPossableSolutions = {1, 2, 3, 4, 5, 6, 7, 8, 9};

Cell::Cell(int value)
    : m_value(value)
    , m_sectionsIn()
    , m_possableSolutions(startingPossableSolutions)
{
    Q_ASSERT(value >= 0 && value <= 9);
    if (m_value != 0)
    {
        m_possableSolutions = { m_value };
    }
}

void Cell::setValue(int value)
{
    Q_ASSERT(value >= 0 && value <= 9);
    Q_ASSERT(m_value == 0);

    m_value = value;
    m_possableSolutions = { m_value };
}

int Cell::getValue() const
{
    return m_value;
}

const QSet<int> &Cell::possableSolutions() const
{
    return m_possableSolutions;
}

bool Cell::updatePossableSolutions(QSet<Section *> &updateSections, bool firstPass)
{
    if (m_possableSolutions.count() == 1)
    {
        if (firstPass)
        {
            updateSections.unite(m_sectionsIn);
        }
        return true;
    }


    for (Section *section : m_sectionsIn)
    {
        QSet<int> solutionsLeftToFind({});
        QSet<int> doubleCheckSoultion = startingPossableSolutions;
        for (Cell *cell : *section)
        {
            if (cell == this)
            {
                continue; // skipping the skill we trying to update
            }
            if (cell->getValue() == 0)
            {
                solutionsLeftToFind |= cell->possableSolutions();
            }
            else
            {
                m_possableSolutions.remove(cell->getValue());
                if (!doubleCheckSoultion.remove(cell->getValue()))
                {
                    return false;
                }
            }

            if (m_possableSolutions.count() == 1)
            {
                setValue(*m_possableSolutions.begin());
                updateSections.unite(m_sectionsIn);
                return true;
            }
            else if (m_possableSolutions.count() == 0)
            {
                return false;
            }
        }

        // Haven't solved, check last possblites
        for (auto solution : m_possableSolutions)
        {
            if (!solutionsLeftToFind.contains(solution))
            {
                setValue(solution);
                updateSections.unite(m_sectionsIn);
                return true;
            }
        }
    }

    return true;
}

void Cell::addSection(Section *section)
{
    m_sectionsIn.insert(section);

    Q_ASSERT(m_sectionsIn.size() <= 3);
}


Grid::Grid(int id, const QString &gridData)
    : m_version(QString::number(id))
    , m_grid(9)
    , m_rows(9)
    , m_columns(9)
    , m_blocks(9)
{
    for (int row = 0; row < 9; ++row)
    {
        for (int column = 0; column < 9; ++column)
        {
            m_grid[row].append(new Cell(QString(gridData[row * 9 + column]).toInt()));
        }
    }
    createSections();
}

Grid::Grid(int subId, const Grid &oldGrid,
     int rowToSet, int columnToSet, int number)
    : m_version(oldGrid.m_version + "." + QString::number(subId))
    , m_grid(9)
    , m_rows(9)
    , m_columns(9)
    , m_blocks(9)
{
    for (int row = 0; row < 9; ++row)
    {
        m_grid[row].resize(9);
        for (int column = 0; column < 9; ++column)
        {
            m_grid[row][column] = new Cell(oldGrid.m_grid[row][column]->getValue());
        }
    }

    Q_ASSERT(m_grid[rowToSet][columnToSet]->getValue() == 0);

    createSections();

    m_grid[rowToSet][columnToSet]->setValue(number);
}

Grid::~Grid()
{
    for (int row = 0; row < 9; ++row)
    {
        for (int column = 0; column < 9; ++column)
        {
            delete m_grid[row][column];
            m_grid[row][column] = NULL;
        }
    }
}

void Grid::createSections()
{
    for (int i = 0; i < 9; ++i)
    {
        m_rows[i] = Section();
        m_columns[i] = Section();
        m_blocks[i] = Section();
    }

    for (int row = 0; row < 9; ++row)
    {
        for (int column = 0; column < 9; ++column)
        {
            const int blockIndex = (row / 3) * 3 + column / 3;
            m_blocks[blockIndex].append(m_grid[row][column]);
            m_rows[row].append(m_grid[row][column]);
            m_columns[column].append(m_grid[row][column]);

            m_grid[row][column]->addSection(&m_rows[row]);
            m_grid[row][column]->addSection(&m_columns[column]);
            m_grid[row][column]->addSection(&m_blocks[blockIndex]);
        }
    }
}

bool Grid::solve()
{
    bool keepTrying = true;
    bool firstPass = true;
    do
    {
        keepTrying = false;
        for (int row = 0; row < 9; ++row)
        {
            for (int column = 0; column < 9; ++column)
            {
                QSet<Section *> updateSections({});
                int len = m_grid[row][column]->possableSolutions().count();
                if (!m_grid[row][column]->updatePossableSolutions(updateSections, firstPass))
                {
                    return false;
                }

                while (!updateSections.isEmpty())
                {
                    auto s = *updateSections.begin();
                    for (Cell *cell : *s)
                    {
                        if (!cell->updatePossableSolutions(updateSections))
                        {
                            return false;
                        }
                    }
                    updateSections.remove(s);
                }
                keepTrying = keepTrying || len != m_grid[row][column]->possableSolutions().count();
            }
        }
        firstPass = false;
    } while (keepTrying);

    QMap<QPair<int, int>, const QSet<int> *> listOfGuesses;
    for (int row = 0; row < 9; ++row)
    {
        for (int column = 0; column < 9; ++column)
        {
            if (m_grid[row][column]->possableSolutions().count() > 1)
            {
                listOfGuesses.insert({row, column}, &m_grid[row][column]->possableSolutions());
            }
        }
    }

    // Guessing
    int subId = 0;
    for (const auto &guess : listOfGuesses.toStdMap())
    {
        for (const auto &nubmerToTry : *guess.second)
        {
            Grid g(subId++, *this, guess.first.first, guess.first.second, nubmerToTry);
            bool solved = g.solve();
            if (solved)
            {
                std::swap(g.m_grid, m_grid);
                return true;
            }
        }
    }

    return isSolved();
}

bool Grid::isSolved()
{
    QVector<QVector<Section> *> allSections;
    allSections << &m_rows; allSections << &m_columns; allSections << &m_blocks;
    for (const auto &section : allSections)
    {
        for (const auto &cells : *section)
        {
            QSet<int> check = Cell::startingPossableSolutions;
            for (const auto &cell : cells)
            {
                if (!check.remove(cell->getValue()))
                {
                    qDebug() << "FAILED";
                    qDebug() << *this;
                    return false;
                }
            }
            if (!check.empty())
            {
                return false;
            }
        }
    }

    return true;
}

int Grid::topLeftSum()
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

    qDebug() << grid.m_version << endl;
    for (int row = 0; row < 9; ++row)
    {
        for (int column = 0; column < 9; column += 3)
        {
            debug << grid.m_grid[row][column]->getValue()
                  << grid.m_grid[row][column + 1]->getValue()
                  << grid.m_grid[row][column + 2]->getValue()
                  << "|";
        }
        debug << endl;
        if (row == 2 || row == 5)
        {
            debug << "-----------------------" << endl;
        }
    }


    return debug;
}