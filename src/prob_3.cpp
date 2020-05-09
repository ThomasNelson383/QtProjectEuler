// Text from the problem https://projecteuler.net/problem=3
// Largest prime factor:
// The prime factors of 13195 are 5, 7, 13 and 29.
//
// What is the largest prime factor of the number 600851475143 ?
#include <QDebug>

static const long long number = 600851475143;

int main()
{
    long long num = number;
    long long i = 2;
    // Devides the number by i^x until it's not a factor
    // Then it moves to another number
    while (i * i <= num)
    {
        if (num % i)
        {
            i += 1;
        }
        else
        {
            num /= i;
        }
    }
    qDebug() << num;

    return 0;
}
