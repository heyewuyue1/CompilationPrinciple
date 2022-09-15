#include "reader.h"
using namespace LexicalAnalysis;
using namespace std;

int main(int argc, char *argv[])
{
    Reader r("./test/test1.c", 1000);
    Token t;
    while (r.getToken(t))
        cout << t;
    cout << t;
    return 0;
}