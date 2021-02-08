#include <stdio.h>

unsigned int setbits(unsigned int x, unsigned int p, unsigned int n, unsigned int y);

int main(void)
{
    unsigned int x, p, n, y;
    scanf("%u", &x);
    scanf("%u", &p);
    scanf("%u", &n);
    scanf("%u", &y);

    printf("%u\n", setbits(x, p, n, y));
    return 0;
}

unsigned int setbits(unsigned int x, unsigned int p, unsigned int n, unsigned int y)
{
    unsigned int xt;
    const unsigned int zero = 0;
    y &= ~(~zero << n);
    xt = x >> (p + 1) << n;
    xt |= y;
    xt <<= p + 1 - n;
    x &= ~(~zero << (p + 1 - n));
    return xt | x;
}
