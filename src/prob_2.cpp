// Text from the problem https://projecteuler.net/problem=2
// Even Fibonacci numbers:
// Each new term in the Fibonacci sequence is generated by adding the previous two terms.
// By starting with 1 and 2, the first 10 terms will be:
//
// 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, ...
//
// By considering the terms in the Fibonacci sequence whose values do not exceed four million,
// find the sum of the even-valued terms.
#include <QDebug>

static const int end   = 4000000;

int main()
{
    int sum = 0;
    int a = 1;
    int b = 1;
    while (b < end)
    {
        int temp = b;
        b = a + b;
        a = temp;
        if (b % 2 == 0)
        {
            sum += b;
        }
    }
    qDebug() << sum;

    return 0;
}
