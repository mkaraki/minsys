#include "math.h"

int intpow(int x, int y)
{
    int result = 1;
    for (int i = 0; i < y; i++)
    {
        result *= x;
    }
    return result;
}

unsigned long ulongpow(unsigned long x, unsigned long y)
{
    unsigned long result = 1;
    for (unsigned long i = 0; i < y; i++)
    {
        result *= x;
    }
    return result;
}

int intabs(int v)
{
    if (v < 0)
        return v * -1;
    return v;
}