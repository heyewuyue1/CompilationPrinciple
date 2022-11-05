#include "slr.h"
#include <iostream>
#include <fstream>
using namespace SLR;
#define contains(vector, item) (find(vector.begin(), vector.end(), item) != vector.end())

void loadSyntax()
{
    vector<Symbol> buf;

    buf.push_back(E);
    syntax.push_back(Generation(_E, buf));
    buf.clear();

    buf.push_back(E); buf.push_back(a); buf.push_back(T);
    syntax.push_back(Generation(E, buf));
    buf.clear();

    buf.push_back(T);
    syntax.push_back(Generation(E, buf));
    buf.clear();

    buf.push_back(T); buf.push_back(b); buf.push_back(F);
    syntax.push_back(Generation(T, buf));
    buf.clear();

    buf.push_back(F);
    syntax.push_back(Generation(T, buf));
    buf.clear();

    buf.push_back(lp); buf.push_back(E); buf.push_back(rp);
    syntax.push_back(Generation(F, buf));
    buf.clear();

    buf.push_back(num);
    syntax.push_back(Generation(F, buf));
    buf.clear();
}

void loadSLR()
{
    for(int i = 0; i < 12; i++)
        for (int j = 0; j < 9; j++)
            preTable[i][j] = new Action(NDefine, -1);
    *preTable[0][5] = Action(Shift, 4); *preTable[0][7] = Action(Shift, 5);
    *preTable[0][0] = Action(Goto, 1); *preTable[0][1] = Action(Goto, 2); *preTable[0][2] = Action(Goto, 3);
    *preTable[1][3] = Action(Shift, 6); *preTable[1][8] = Action(Accept, 0);
    *preTable[2][3] = *preTable[2][6] =  *preTable[2][8] = Action(Reduce, 2); *preTable[2][4] = Action(Shift, 7);
    *preTable[3][3] = *preTable[3][4] = *preTable[3][6] = *preTable[3][8] = Action(Reduce, 4);
    *preTable[4][5] = Action(Shift, 4); *preTable[4][7] = Action(Shift, 5);
    *preTable[4][0] = Action(Goto, 8); *preTable[4][1] = Action(Goto, 2); *preTable[4][2] = Action(Goto, 3);
    *preTable[5][3] = *preTable[5][4] = *preTable[5][6] = *preTable[5][8] = Action(Reduce, 6);
    *preTable[6][5] = Action(Shift, 4); *preTable[6][7] = Action(Shift, 5);
    *preTable[6][1] = Action(Goto, 9); *preTable[6][2] = Action(Goto, 3);
    *preTable[7][5] = Action(Shift, 4); *preTable[7][7] = Action(Shift, 5); *preTable[7][2] = Action(Goto, 10);
    *preTable[8][3] = Action(Shift, 6); *preTable[8][6] = Action(Shift, 11);
    *preTable[9][3] = Action(Reduce, 1); *preTable[9][4] = Action(Shift, 7); *preTable[9][8] = Action(Reduce, 1);
    *preTable[9][6] = Action(Reduce, 1);
    *preTable[10][3] = *preTable[10][4] = *preTable[10][6] = *preTable[10][8] = Action(Reduce, 3);
    *preTable[11][3] = *preTable[11][4] = *preTable[11][6] = *preTable[11][8] = Action(Reduce, 5);
}

Symbol recogSymbol(char c)
{
    if (isdigit(c))
        return num;
    if (c == '+' || c == '-')
        return a;
    if (c == '*' || c == '/')
        return b;
    if (c == '(')
        return lp;
    if (c == ')')
        return rp;
    if(c == '$')
        return SLR::end;
    return {true, "Error", -1};
}

int analysis(string buf)
{
    buf += '$';
    vector<int> stateStack(0);
    vector<Symbol> symbolStack(0);
    stateStack.push_back(0);
    symbolStack.push_back(SLR::end);
    bool isAccept = false;
    int p = 0;
    while (!isAccept)
    {
        cout << "[StateStack]: ";
        for (auto i: stateStack)
            cout << i << ' ';
        cout << endl;
        cout << "[SymbolStack]: ";
        for (auto i: symbolStack)
            cout << i.text << ' ';
        cout << endl;
        cout << "[Left string]: ";
        for (int j = p; j < buf.length(); j++)
            cout << buf[j];
        cout << endl;
        
        Symbol curSym = recogSymbol(buf[p]);
        if (curSym.id == -1){
            cout << "Unexpected character. Aborting..." << endl;
            return 1;
        }
        Action *curAct = preTable[stateStack.back()][curSym.id];
        cout << "[Action]: " << FancyATS[curAct->action] << ' ' << curAct->use << endl;
        if (curAct->action == Shift) {
            stateStack.push_back(curAct->use);
            symbolStack.push_back(curSym);
            p++;
        } else if (curAct->action == Reduce) {
            Generation g = syntax[curAct->use];
            for (int i = 0; i < g.right.size(); i++) {
                stateStack.pop_back();
                symbolStack.pop_back();
            }
            symbolStack.push_back(g.left);
            stateStack.push_back(preTable[stateStack.back()][symbolStack.back().id]->use);
        } else if (curAct->action == Accept)
            isAccept = true;
        else {
            cout << "Syntax Error at " << p + 1 << endl;
            return 2;
        }
        cout << endl;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    loadSyntax();
    loadSLR();
    cout << "Predict Table:" << endl;
    cout << "  " ;
    for (int i = 0; i < 9; i++)
        cout << symbols[i].text << "  ";
    cout << endl;
    for (int i = 0; i < 12; i++){
        cout << i << ' ';
        for (int j = 0; j < 9; j++)
            cout << ActionTypeStr[preTable[i][j]->action] << preTable[i][j]->use << ' ';
        cout << endl;
    }
    string rawStr;
    if (argc >= 2) {
        ifstream input(argv[1]);
        input >> rawStr;
    } else 
        cin >> rawStr;
    int ret = analysis(rawStr);
    cout << string(ret? "Deny.": "Accept.") << endl;
    return 0;
}