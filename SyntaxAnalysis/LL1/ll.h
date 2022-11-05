#include <vector>
using namespace std;

/*
* E->TE'
* E'-> +TE'|-TE'|e
* T-> FT'
* T'-> *FT'|/FT'|e
*/

namespace LL
{
    typedef struct
    {
        bool isTerminator;
        const char *text;
        int id;
    } Symbol;

    class Generation
    {
    public:
        Symbol left;
        vector<Symbol> right;
        Generation(Symbol _left, vector<Symbol> _right): left(_left), right(_right){}
    };
    Symbol E = {false, "E", 0};
    Symbol _E = {false, "E'", 1};
    Symbol T = {false, "T", 2};
    Symbol _T = {false, "T'", 3};
    Symbol F = {false, "F", 4};

    Symbol plus = {true, "+", 5};
    Symbol minus = {true, "-", 6};
    Symbol multi = {true, "*", 7};
    Symbol divide = {true, "/", 8};
    Symbol lp = {true, "(", 9};
    Symbol rp = {true, ")", 10};
    Symbol num = {true, "num", 11};
    Symbol empty = {true, "e", 12};
    Symbol end = {true, "$", 13};
    Symbol symbols[14] = {
        E, _E, T, _T, F,
        plus, minus, multi, divide,
        lp, rp, num,
        empty, end
    };

    bool operator==(Symbol a, Symbol b) {return a.id == b.id;}
    bool operator!=(Symbol a, Symbol b) {return a.id != b.id;}

    static vector<Generation> syntax;
    static vector<vector<Symbol> > first(14);
    static vector<vector<Symbol> > follow(14);
    static int preTable[5][8];
}