#ifndef GRID_H
#define GRID_H

#include <QObject>
#include <QVector>
#include <QSet>

class Cell;
#define Section QList<Cell*>

class Cell : public QObject
{
    Q_OBJECT
public:
    Cell(int value);

    static const QSet<int> startingPossableSolutions;

    void setValue(int value);
    int getValue() const;

    const QSet<int> &possableSolutions() const;
    bool updatePossableSolutions(QSet<Section *> &updateSections, bool firstPass = false);

    void addSection(Section *section);

    int topLeftSum();
private:
    int m_value;
    QSet<Section *> m_sectionsIn;
    QSet<int> m_possableSolutions;
};

class Grid
{
public:
    Grid(int id, const QString &gridData);
    Grid(int subId, const Grid &oldGrid,
         int rowToSet, int columnToSet, int number);
    ~Grid();

    bool solve();
    bool isSolved();
    int topLeftSum();

    friend QDebug operator<<(QDebug debug, const Grid &grid);

private:
    int m_id;
    QString m_version;

    QVector<QVector<Cell*>> m_grid;
    QVector<Section>        m_rows;
    QVector<Section>        m_columns;
    QVector<Section>        m_blocks;

    static QMap<int, bool> s_solvedGrids;



    void createSections();
};

#endif // GRID_H
