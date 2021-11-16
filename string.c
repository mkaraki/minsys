#include "string.h"

int compaireString(IN CHAR16 *a, IN CHAR16 *b)
{
    int i = 0;
    while (a[i] != 0)
    {
        if (a[i] != b[i])
        {
            return 0;
        }
        i++;
    }
    return 1;
}

int strtoint(IN CHAR16 *source)
{
    int result = 0;

    int firstpos = 0;
    for (int i = 0; source[i] != 0; i++)
        firstpos = i;

    int isminus = 0;

    int j = 0;
    for (int i = firstpos; i >= 0; i--)
    {
        if (i == firstpos && source[i] == '-')
        {
            isminus = 1;
            continue;
        }

        if (source[i] < L'0' || source[i] > L'9')
            break;

        result += (source[i] - L'0') * intpow(10, j);
        j++;
    }

    if (isminus)
        result *= -1;

    return result;
}