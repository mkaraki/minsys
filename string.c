#include "string.h"

int strcmp(IN CHAR16 *a, IN CHAR16 *b)
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

int getintdigit(int input)
{
    for (int i = 1;; i++)
    {
        if ((int)((unsigned long)input % ulongpow(10, i)) == input)
            return i;
    }
}

int getSpecificDigitFromInt(int src, int digit)
{
    return (int)((unsigned long)src % ulongpow(10, digit) / ulongpow(10, digit - 1));
}

void inttostr(int input, OUT CHAR16 *output)
{
    int digit_i = 0;

    if (input < 0)
    {
        output[0] = L'-';
        input = intabs(input);
        digit_i++;
    }

    for (int i = getintdigit(input); i > 0; i--)
    {
        int specific_digit = getSpecificDigitFromInt(input, i);
        output[digit_i] = L'0' + specific_digit;
        digit_i++;
    }

    output[digit_i] = 0;
}