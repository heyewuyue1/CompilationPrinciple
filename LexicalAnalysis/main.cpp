#include "lex.h"
using namespace LexicalAnalysis;
using namespace std;

int main(int argc, char *argv[])
{
    string filePath("./test/test1.c");
    if (argc == 2){
        filePath = argv[1];
    }
    Lex lexer(filePath.c_str());
    lexer.analysis();
    lexer.printRes();
    return 0;
}