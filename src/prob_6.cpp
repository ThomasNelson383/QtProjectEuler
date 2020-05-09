// Text from the problem https://projecteuler.net/problem=6
// Sum square difference:
// The sum of the squares of the first ten natural numbers is,
//
// 1²+2²+...+10²=385
// The square of the sum of the first ten natural numbers is,
//
// (1+2+...+10)²=552=3025
// Hence the difference between the sum of the squares of the first ten natural numbers and the square of the sum is 3025−385=2640.
//
// Find the difference between the sum of the squares of the first one hundred natural numbers and the square of the sum.
#include <QDebug>
#include <QThread>

static const int end = 100;

int main()
{
    int sum = 0;
    int squared = 0;

    QThread *sumThread = QThread::create([&sum]()
    {
        for (int i = 0; i <= end; ++i)
        {
            sum += i;
        }
        sum = pow(sum, 2);
    });
    sumThread->start();

    QThread *squaredThread = QThread::create([&squared]()
    {
        for (int i = 0; i <= end; ++i)
        {
            squared += pow(i, 2);
        }
    });
    squaredThread->start();

    sumThread->wait();
    squaredThread->wait();

    qDebug() << sum - squared;

    return 0;
}
