#include "reader.h"
using namespace LexicalAnalysis;
using namespace std;

int main(int argc, char *argv[])
{
    // char filePath[] = "./test/test1.c";
    // if (argc == 1)
    // {
    //     filePath = argv[0];
    // }
    Reader r("./test/test2.c");
    Token t;
    while (r.getToken(t))
        cout <<  t;
    cout << t;
    return 0;
}