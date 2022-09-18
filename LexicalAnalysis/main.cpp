#include "lex.h"
using namespace LexicalAnalysis;
using namespace std;

int main(int argc, char *argv[])
{
    Lex lexer("./test/test1.c");
    lexer.analysis();
    lexer.printRes();
    return 0;
}