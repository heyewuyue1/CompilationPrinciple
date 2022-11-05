#include <vector>
using namespace std;

/*
* E'->E
* E->EaT
* E->T
* T->TbF
* T->F
* F->(E)
* F->num
*/

namespace SLR
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
    Symbol _E = {false, "E'", 9};
    Symbol E = {false, "E", 0};
    Symbol T = {false, "T", 1};
    Symbol F = {false, "F", 2};

    Symbol a = {true, "a", 3};
    Symbol b = {true, "b", 4};
    Symbol lp = {true, "(", 5};
    Symbol rp = {true, ")", 6};
    Symbol num = {true, "num", 7};
    Symbol end = {true, "$", 8};
    Symbol symbols[10] = {
        E, T, F,
        a, b,
        lp, rp, num,
        end, _E
    };

    bool operator==(Symbol a, Symbol b) {return a.id == b.id;}
    bool operator!=(Symbol a, Symbol b) {return a.id != b.id;}

    enum ActionType{Shift, Reduce, Goto, Accept, NDefine};

    const char * ActionTypeStr[5] = {
        "S", "R", "", "ACC", ""
    };
    const char * FancyATS[5] = {
        "Shift", "Reduce", "Goto", "Accept", "Error"
    };

    class Action{
    public:
        ActionType action;
        int use;
        Action(ActionType _action, int _use): action(_action), use(_use){}
    };

    static vector<Generation> syntax;
    static Action *preTable[12][9];
}