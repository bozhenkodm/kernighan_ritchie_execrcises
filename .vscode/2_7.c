/*def invert(x, p, n):
    bprint(x)
    crop = (x >> (p + 1 - n)) & ~(~0 << n)
    bprint(crop)
    reverted = ~crop & ~(~0 << n)
    bprint(reverted)
    xt = ((x >> p + 1 << n | reverted) << (p + 1 - n)) | (x & ~(~0 << p + 1 - n))
    bprint(xt)
*/
unsigned int invert(unsigned int x, unsigned int p, unsigned int n);

unsigned int invert(unsigned int x, unsigned int p, unsigned int n)
{
    unsigned int reverted;
    int shift;
    if ((shift = p + 1 - n) < 0)
    {
        shift = 0;
    }
    reverted = ~((x >> shift) & ~(~0 << n)) & ~(~0 << n);
    return ((x >> (p + 1) << n | reverted) << shift) | (x & ~(~0 << shift));
}
