#include <vector>
#include "reader.h"
using namespace std;

namespace LexicalAnalysis
{

typedef struct TableItem
{
    Token token;
    Cursor loc;
} TableItem;

class Lex {
public:
    void analysis();
    Lex(const char *);
    void printRes();
private:
    vector<TableItem> _tokenTable;
    unsigned int tokenNum[9];
    Reader _reader;
    unsigned int lines;
    unsigned int charNum;
};
} // namespace LexicalAnalysis
