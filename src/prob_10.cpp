// Text from the problem https://projecteuler.net/problem=10
// Summation of primes
// The sum of the primes below 10 is 2 + 3 + 5 + 7 = 17.
//
// Find the sum of all the primes below two million.
#include <QDebug>
#include <QSet>

static const quint64 MAX_NUMBER = 2 * pow(10, 6);


bool isPrime(quint64 number, const QList<quint64> &foundPrimes)
{
    quint64 lastCheck = sqrt(number);
    bool notAPrime = false;
    for (quint64 check : foundPrimes)
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
    QList<quint64> foundPrimes = { /*2 is implied by skipping each other number*/ 3 };
    quint64 sumPrimes = 5;
    for (quint64 i = 5; i < MAX_NUMBER; i += 2)
    {
        if (isPrime(i, foundPrimes))
        {
            foundPrimes << i;
            sumPrimes += i;
        }
    }

    qDebug() << sumPrimes;
    return 0;
}
