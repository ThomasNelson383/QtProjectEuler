// Text from the problem https://projecteuler.net/problem=8
// Largest product in a series:
// The four adjacent digits in the 1000-digit number that have the greatest product are 9 × 9 × 8 × 9 = 5832.
//
// Find the thirteen adjacent digits in the 1000-digit number that have the greatest product. What is the value of this product?
//

#include <QDebug>

// So I can fold this, and not look at it
static const QString getBigNumber()
{
    return
    QString("73167176531330624919225119674426574742355349194934") +
    QString("96983520312774506326239578318016984801869478851843") +
    QString("85861560789112949495459501737958331952853208805511") +
    QString("12540698747158523863050715693290963295227443043557") +
    QString("66896648950445244523161731856403098711121722383113") +
    QString("62229893423380308135336276614282806444486645238749") +
    QString("30358907296290491560440772390713810515859307960866") +
    QString("70172427121883998797908792274921901699720888093776") +
    QString("65727333001053367881220235421809751254540594752243") +
    QString("52584907711670556013604839586446706324415722155397") +
    QString("53697817977846174064955149290862569321978468622482") +
    QString("83972241375657056057490261407972968652414535100474") +
    QString("82166370484403199890008895243450658541227588666881") +
    QString("16427171479924442928230863465674813919123162824586") +
    QString("17866458359124566529476545682848912883142607690042") +
    QString("24219022671055626321111109370544217506941658960408") +
    QString("07198403850962455444362981230987879927244284909188") +
    QString("84580156166097919133875499200524063689912560717606") +
    QString("05886116467109405077541002256983155200055935729725") +
    QString("71636269561882670428252483600823257530420752963450");
}

static const QString BIG_NUMBER = getBigNumber();
static const int ADJACENT_DIGTS = 13;

static quint64 getNumber(const QString &chunk)
{
    Q_ASSERT(!chunk.isNull() && chunk.length() == ADJACENT_DIGTS);

    quint64 number = 1;
    for (int i = 0; i < chunk.length(); ++i)
    {
        number *= chunk.mid(i, 1).toULongLong();
    }

    return number;
}

int main()
{
    const QString chunk = BIG_NUMBER.mid(0, ADJACENT_DIGTS);
    quint64 number = getNumber(chunk);
    quint64 maxNumber = number;
    int foundZero = 0;
    for (int i = ADJACENT_DIGTS; i < BIG_NUMBER.length() - ADJACENT_DIGTS; ++i)
    {
        int lastNumber = BIG_NUMBER.mid(i, 1).toULongLong();
        if (lastNumber == 0)
        {
            foundZero = ADJACENT_DIGTS;
            number = 1;
        }
        else
        {
            if (foundZero)
            {
                --foundZero;
            }
            else
            {
                number /= BIG_NUMBER.mid(i - ADJACENT_DIGTS,1).toULongLong();
            }
            number *= lastNumber;
        }

        if (number > maxNumber)
        {
            maxNumber = number;
        }
    }

    qDebug() << maxNumber;
    return 0;
}
