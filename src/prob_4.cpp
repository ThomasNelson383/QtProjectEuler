// Text from the problem https://projecteuler.net/problem=4
// Largest palindrome product:
// A palindromic number reads the same both ways. The largest palindrome made from the product of two 2-digit numbers is 9009 = 91 × 99.
//
// Find the largest palindrome made from the product of two 3-digit numbers.
#include <QDebug>
#include <QString>

bool isPalindrome(int number)
{
    QString s = QString::number(number);

    for (int i = 0; i < s.length()/2; ++i)
    {
        if (s.at(i) != s.at(s.length() - 1 - i))
        {
            return false;
        }
    }
    return true;
}

// This algrothim is odd
// Looping on A from 999 to 100 or to the highest B found
// then Looping over B then is going from A to 100
//   So first few: 999 * 999, 999 * 998
// Then if we found a Palindrom then we only need to go down to that B
//   First palindrom found was 995 x 583, so no we are
//   looping A from 995 to 583, and B from that same
//   because we no 994 * 582 will always yeild a smaller number
// We are also skipping any extra numbers if we found a number that is smaller than our max palindrome

int main()
{
    int maxPalindrome = 0;
    int maxB = 100;
    for (int a = 999; a > maxB; --a)
    {
        for (int b = a;  b > maxB; --b)
        {
            int num = a * b;
            if (num < maxPalindrome)
            {
                break; // We shouldn't need to test any more numbers if we it's already less than the maxPalindrome found
            }
            if (isPalindrome(num))
            {
                if (maxPalindrome < num)
                {
                    maxPalindrome = num;
                    maxB = b; // Setting this, so we don't have left go pass this point
                }
            }
        }
    }


    qDebug() << maxPalindrome;

    return 0;
}
