typedef struct Node
{
    int a, b;
    long long c;
    union
    {
        float d;
        double e;
    };
    LL f;
} Node;

char ch[10];
enum TT
{
    namea,
    nameb,
};
extern eee;

static int func(unsigned long long var)
{
    var += var * (var - var) + ((var / var) % var);
    unsigned LL *restrict p = &var;
    return var;
}

inline int f()
{
    return sizeof(LL);
}

int main(int argc, char *argv[])
{
    Node node;
    Node *pnode = &node;
    node.a = pnode->a;
    node.b++;
    auto unsigned int x = node.b & ((pnode->c | ch[0]) ^ node.a--);
    volatile unsigned char m = x << 5 + 'c';
    const float y = 3.14, z = 3e5;
    _Bool t = x < z && y > z || x <= y && y >= z;
    node.a = m++ + x;
    _Complex n;
    goto label;
    char *str = "str/*comment*/";
label:
    if (y != z)
        x <<= 2 ^ 3;
    else if (!(x >> 2))
        x >>= 2;
    else
        x &= 5;
    m ^= (x |= ~node.b);
    x -= (node.a *= (m /= (node.b %= 2u)));
    while (1)
    {
        /* This is a loop */
        if (5u)
            break;
        if (y <= z)
            continue;
    }
    do // a comment
    {
        signed _temp = 0x0123456789abcdefUL, temp_temp = 00123251;
        float __ = 0x12f.2p+4;
        switch (_temp)
        {
        case 'd':
            /* Comment
            ???
            @ # `

            break;
            */
            break;

        default:
            break;
        }
    } while (0);
    // a comment "string" 'c' 123321
    for (register int i = 1; i <= -1; ++i)
        ;

    printf("%llu\n", 3ul ? 2llu : 45ull);
    printf("floats %f %lf\n",
           1.e-2f, (double).356e+3l);
    return x, 0;
}