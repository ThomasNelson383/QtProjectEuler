// Text from the problem https://projecteuler.net/problem=7
// 10001st prime:
// By listing the first six prime numbers: 2, 3, 5, 7, 11, and 13, we can see that the 6th prime is 13.
//
// What is the 10 001st prime number?
//

const int FIND_PRIME = 10001;

#include <QDebug>
#include <QSet>

bool isPrime(int number, const QList<int> &foundPrimes)
{
    int lastCheck = sqrt(number);
    bool notAPrime = false;
    for (int check : foundPrimes)
    {
        if (check > lastCheck)
        {
            break;
        }
        if (number % check == 0)
        {
            notAPrime = true;
            break;
        }
    }

    return !notAPrime;
}

int main()
{
    QList<int> foundPrimes = { 2, 3 };
    int i = 5;
    while(true)
    {
        if (isPrime(i, foundPrimes))
        {
            foundPrimes << i;
            if (foundPrimes.count() ==  FIND_PRIME)
            {
                break;
            }
        }
        i += 2;
    }

    qDebug() << i;
    return 0;
}
