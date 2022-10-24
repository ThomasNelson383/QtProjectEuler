#ifndef GRID_H
#define GRID_H

#include <QObject>
#include <QMutex>
#include <QList>
#include <QMap>
#include <QSet>
#include <QSharedPointer>
#include <QPair>

class Cell;
typedef QList<Cell*> Section;

class Cell : public QObject
{
    Q_OBJECT
public:
    enum class Strategies : int {
        EASIST_STRAATEGY = 0,
        HiddenSingle = 0,
        PointingPair = 1,
        NakedPair,
        HiddenPair,
        XWing,

        END_OF_STRATEGIES
    };
    Cell(const QString &id = "FF", int value = 0, QObject *p = Q_NULLPTR);
    ~Cell() {}

    // Cell(const Cell &other) {
    //     this->m_value = other.m_value;
    //     this->m_currentPossibleSolutions = other.m_currentPossibleSolutions;
    // }
    // Cell &operator=(const Cell &other)
    // {
    //     this->m_value = other.m_value;
    //     this->m_currentPossibleSolutions = other.m_currentPossibleSolutions;\

    //     return *this;
    // }
    void setID(const QString &id);
    QString getID() const;

    void setValue(int value);
    int getValue() const;
    int getOValue() const;

    const QSet<int> getPossibleSolutions() const;

    bool isSolved() const;

    void addSection(Section *section);
    void addSectionBlock(Section *section);

    bool updatePossibleSolutions(Strategies stragtegy);

    Section getLockedPairs();

    static const QSet<int> s_basePossibleSolutions;

public slots:
    bool removePossiableSolution(int solution);
    bool removePossiableSolutions(QSet<int> solutions);

signals:
    void possibleSolutionsChanged(QSet<int> &possiablisties);
    void slovedChanged(int solution);

private:
    QString m_id = "FF";
    const int m_oValue = 0;
    int m_value = 0;

    QSet<int> m_currentPossibleSolutions;

    bool hiddenSingle();
    bool pointingPairs();
    bool nakedPair();
    bool hiddenPair();
    bool xWing();

    QList<Section*> m_sections;
    Section* m_block;
};

class Grid : public QObject
{
public:
    Grid(int id, const QString &gridData, QObject *p = Q_NULLPTR);
    // Grid(const Grid &oldGrid,
    //      int rowToSet, int columnToSet, int number);
    ~Grid();

    bool solve();
    bool isSolved();
    int topLeftSum() const;

    friend QDebug operator<<(QDebug debug, const Grid &grid);

private:
    int m_id;
    QString m_version;

    Cell* m_grid[9][9];

    Section m_rows[9];
    Section m_columns[9];
    Section m_blocks[9];

    void setCellsSections();
};

#endif // GRID_H
