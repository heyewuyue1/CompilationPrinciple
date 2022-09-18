#include <iostream>
using namespace std;

namespace LexicalAnalysis
{
class Cursor
{
public:
    operator unsigned int();
    Cursor();
    Cursor(unsigned int, unsigned int, unsigned int);
    void nextLine();
    friend ostream& operator<<(ostream&, const Cursor&);
    unsigned int loc, line, row;
};
}