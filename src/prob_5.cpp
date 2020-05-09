// Text from the problem https://projecteuler.net/problem=5
// Smallest multiple:
// 2520 is the smallest number that can be divided by each of the numbers from 1 to 10 without any remainder.
//
// What is the smallest positive number that is evenly divisible by all of the numbers from 1 to 20?
#include <QDebug>

static const int a = 1;
static const int b = 20;

int main()
{
    qint64 highestNumberPossable = 1;
    for (int i = a; i <= b; ++i)
    {
        highestNumberPossable *= i;
    }
    qint64 smallestNumber = highestNumberPossable;
    for (qint64 i = b; i < highestNumberPossable; i += b) // Skipping every b, because it can't be devisable any way
    {
        bool found = true;
        for (qint64 j = b - 1; j >= a; --j) // Checking each number to see if it's devisable
        {
            if (i % j != 0)
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            // Becase we started from the bottom and going up,
            // this will be the smallestNumber
            smallestNumber = i;
            break;
        }
    }


    qDebug() << smallestNumber;

    return 0;
}
