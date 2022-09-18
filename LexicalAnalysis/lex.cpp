#include "lex.h"

namespace LexicalAnalysis
{

Lex::Lex(const char * filePath):
_reader(filePath), lines(0), charNum(0)
{
    for (int i = 0; i < 9; i++)
        tokenNum[i] = 0;
}

void Lex::analysis()
{
    TableItem i;
    while (_reader.getToken(i.token, i.loc))
    {
        _tokenTable.push_back(i);
        tokenNum[i.token.type()]++;
    }
    _tokenTable.push_back(i);
    tokenNum[i.token.type()]++;
    charNum = _reader.getCharNum();
    lines = _reader.getLineNum();
}

void Lex::printRes()
{
    for (int i = 0; i < _tokenTable.size(); i++)
        cout << _tokenTable[i].token << _tokenTable[i].loc << endl;
    cout << "[Statics]" << endl;
    cout << "Total characters: " << charNum << endl;
    cout << "Total lines: " << lines << endl;
    cout << "Total tokens: " << _tokenTable.size() << endl;
}

}