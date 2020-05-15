// Text from the problem https://projecteuler.net/problem=9
// Special Pythagorean triplet:
// A Pythagorean triplet is a set of three natural numbers, a < b < c, for which,
//
// a² + b² = c²
// For example, 3² + 4² = 9 + 16 = 25 = 5².
//
// There exists exactly one Pythagorean triplet for which a + b + c = 1000.
// Find the product abc.

#include <QDebug>
#include <math.h>


int main()
{
    // c = 1000 - a - b
    //
    // a^2 + b^2 = (1000 - a - b)^2
    // b = (1000 * (a - 500))/(a - 1000)
    // a != 1000

    int foundNumber = 0;
    for (int a = 1; a < 997; ++a)
    {
        int b = (1000 *(a - 500))/(a - 1000);
        int c = sqrt(a*a + b*b);

        if (a < b && b < c)
        {
            if (a + b + c == 1000)
            {
                foundNumber = a * b * c;
                qDebug() << a << b << c;
                break;
            }
        }
    }

    qDebug() << foundNumber;
    return 0;
}
