bool getBit(int num, int i)
{
    return (num & (1<<i)) != 0;
}

int setBit (int num, int i)
{
    return num | (1 << i);
}

int clearBit (int num, int i)
{
    auto mask = ~(1<<i);
    return num & mask;
}

int updateBit(int  num, int i, bool set1)
{
    auto value = set1 ? 1: 0;
    auto mask = ~(1 << i);
    return (num & mask) | (value << i);
}

int isBigEndian(void)
{
    union 
    {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1; 
}
