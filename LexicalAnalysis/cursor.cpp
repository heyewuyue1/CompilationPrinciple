#include "cursor.h"

namespace LexicalAnalysis
{
Cursor::Cursor()
{
    loc = 0;
    line = 1;
    row = 1;
}

Cursor::Cursor(unsigned int loc, unsigned int line, unsigned int row)
:loc(loc), line(line), row(row){}

void Cursor::nextLine()
{
    line++;
    row = 0;
}

ostream& operator<<(ostream& cout, const Cursor& cur)
{
    cout << ':' << cur.line << ':' << cur.row;
    return cout;
}

Cursor::operator unsigned int()
{
    return loc;
}
}