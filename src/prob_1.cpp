// Text from the problem https://projecteuler.net/problem=1
// Multiples of 3 and 5:
// If we list all the natural numbers below 10 that are multiples of 3 or 5, we get 3, 5, 6 and 9. The sum of these multiples is 23.
//
// Find the sum of all the multiples of 3 or 5 below 1000.
#include <QDebug>
#include <QList>

static const int start = 0;
static const int end   = 1000;
static const QList<int> list = {3, 5}; // Inclase I wanted to increase this later

int main()
{
    int sum = 0;
    for (int i = start; i < end; ++i)
    {
        if (std::any_of(list.cbegin(), list.cend(), [i](int item){
                return (i % item) == 0;
               }))
        {
            sum += i;
        }
    }
    qDebug() << sum;

    return 0;
}
