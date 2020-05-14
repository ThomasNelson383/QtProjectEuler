// Text from the problem https://projecteuler.net/problem=96
// Su Doku:
// Su Doku (Japanese meaning number place) is the name given to a popular puzzle concept. Its origin is unclear, but credit must be attributed to Leonhard Euler who invented a similar, and much more difficult, puzzle idea called Latin Squares. The objective of Su Doku puzzles, however, is to replace the blanks (or zeros) in a 9 by 9 grid in such that each row, column, and 3 by 3 box contains each of the digits 1 to 9. Below is an example of a typical starting puzzle grid and its solution grid.
//
// A well constructed Su Doku puzzle has a unique solution and can be solved by logic, although it may be necessary to employ "guess and test" methods in order to eliminate options (there is much contested opinion over this). The complexity of the search determines the difficulty of the puzzle; the example above is considered easy because it can be solved by straight forward direct deduction.
//
// The 6K text file, sudoku.txt (right click and 'Save Link/Target As...'), contains fifty different Su Doku puzzles ranging in difficulty, but all with unique solutions (the first puzzle in the file is the example above).
//
// By solving all fifty puzzles find the sum of the 3-digit numbers found in the top left corner of each solution grid; for example, 483 is the 3-digit number found in the top left corner of the solution grid above.

#include <QDebug>
#include <QList>
#include <QFile>
#include <QList>
#include <QElapsedTimer>

#include "grid.h"

static const QString filePath = "../../../res/p096_sudoku.txt";
static const int startNumber = 0;
static const int numSuDokuPuzzles = 50;

static QElapsedTimer stopwatch;


int main()
{
    QFile f(filePath);
    if (!f.exists() || !f.open(QFile::OpenModeFlag::ReadOnly))
    {
        return -1; // Failed to either have the file, or open it
    }

    QString fileData = f.readAll();
    fileData.remove('\n');
    QList<QString> suDokuData = fileData.split(QRegExp("Grid [0-9][0-9]"));
    suDokuData.removeAt(0); // Blank grid

    Q_ASSERT(suDokuData.length() == numSuDokuPuzzles); // Should have gotten all 50, but testing we only care about numSuDokuPuzzles

    QList<Grid *> puzzles;
    for (int i = startNumber; i < numSuDokuPuzzles; ++i)
    {
        puzzles.append(new Grid(i+1, suDokuData[i]));
    }
    stopwatch.start();
    QList<int> topLeftNumbers;
    for (Grid *puzzle : puzzles)
    {
        stopwatch.restart();
        puzzle->solve();
        qDebug() << *puzzle;
        qDebug() << "Time:" << stopwatch.elapsed() << "ms";
        topLeftNumbers << puzzle->topLeftSum();
        delete puzzle;
    }
    puzzles.clear();

    int sum = 0;
    int numberFailed = 0;
    for (int topLeft : topLeftNumbers)
    {
        if (topLeft == 0)
        {
            ++numberFailed;
        }
        else
        {
            sum += topLeft;
        }
    }

    qDebug() << "sum: " << sum << "failed:" << numberFailed;
    return 0;
}
