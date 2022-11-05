#include "ll.h"
#include <iostream>
#include <fstream>
using namespace LL;
#define contains(vector, item) (find(vector.begin(), vector.end(), item) != vector.end())
/*
* E->TE'
* E'-> +TE'|-TE'|e
* T-> FT'
* T'-> *FT'|/FT'|e
* F-> (E)
* F-> num
*/

void loadSyntax()
{
    vector<Symbol> buf;

    buf.push_back(T); buf.push_back(_E);
    syntax.push_back(Generation(E, buf));
    buf.clear();

    buf.push_back(LL::plus); buf.push_back(T); buf.push_back(_E);
    syntax.push_back(Generation(_E, buf));
    buf.clear();

    buf.push_back(LL::minus); buf.push_back(T); buf.push_back(_E);
    syntax.push_back(Generation(_E, buf));
    buf.clear();

    buf.push_back(LL::empty);
    syntax.push_back(Generation(_E, buf));
    buf.clear();

    buf.push_back(F); buf.push_back(_T);
    syntax.push_back(Generation(T, buf));
    buf.clear();

    buf.push_back(multi); buf.push_back(F); buf.push_back(_T);
    syntax.push_back(Generation(_T, buf));
    buf.clear();

    buf.push_back(divide); buf.push_back(F); buf.push_back(_T);
    syntax.push_back(Generation(_T, buf));
    buf.clear();

    buf.push_back(LL::empty);
    syntax.push_back(Generation(_T, buf));
    buf.clear();

    buf.push_back(lp); buf.push_back(E); buf.push_back(rp);
    syntax.push_back(Generation(F, buf));
    buf.clear();

    buf.push_back(num);
    syntax.push_back(Generation(F, buf));
    buf.clear();
}

void getFirst(Symbol s)
{
    if (s.isTerminator){
        first[s.id].push_back(s);
    }
    else
        for (Generation gen: syntax)
            if (gen.left.id == s.id) {
                if (first[gen.right[0].id].empty())
                    getFirst(gen.right[0]);
                for (Symbol sym: first[gen.right[0].id])
                    first[s.id].push_back(sym);
            }
}

void getFollow(Symbol s)
{
    if (s == E && !contains(follow[E.id], LL::end))
        follow[E.id].push_back(LL::end);

    for (auto gen: syntax) {
        auto loc = find(gen.right.begin(), gen.right.end(), s);
        if (loc != gen.right.end() && gen.left != s) {
            if (loc == gen.right.end() - 1) {
                getFollow(gen.left);
                for (auto symbol: follow[gen.left.id])
                    if (!contains(follow[s.id], symbol))
                        follow[s.id].push_back(symbol);
            } else if ((*(loc + 1)).isTerminator){
                if (!contains(follow[s.id], *(loc + 1)))
                    follow[s.id].push_back(*(loc + 1));
            } else {
                for (auto symbol: first[(*(loc + 1)).id]) {
                    if (symbol != LL::empty) {
                        if (!contains(follow[s.id], symbol))
                            follow[s.id].push_back(symbol);
                    } else {
                        if (follow[gen.left.id].empty())
                            getFollow(gen.left);
                        for (auto symbol: follow[gen.left.id])
                            if (!contains(follow[s.id], symbol))
                                follow[s.id].push_back(symbol);
                    }
                }
            }
        }
    }
}

void makeTable()
{
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 8; j++)
            preTable[i][j] = -1;
    for (int i = 0; i < syntax.size(); i++) {
        Generation gen = syntax[i];
        for (auto fi: first[gen.right[0].id]) {
            if (fi == LL::empty) {
                for (auto fo: follow[gen.left.id])
                    preTable[gen.left.id][fo.id - 5] = i;
            } else
                preTable[gen.left.id][fi.id - 5] = i;
        }
    }
    for (int i = 0; i < 5; i++) {
        for (auto symbol: follow[i]) {
            if (preTable[i][symbol.id - 5] == -1)
                preTable[i][symbol.id - 5] = -2;
        }
    }
}

Symbol recogSymbol(char c)
{
    for (auto symbol: symbols)
        if ((symbol == num && isdigit(c)) || symbol.text == string(1, c))
            return symbol;
    return {true, "error", -1};
}

int analysis(string buf)
{
    buf += '$';
    vector<Symbol> analizer;
    analizer.push_back(LL::end);
    analizer.push_back(E);
    int i = 0;
    int ret = 0;
    while (!analizer.empty()) {
        cout << "[Stack]: ";
        for (auto symbol: analizer)
            cout << symbol.text;
        cout << endl;
        cout << "[Left string]: ";
        for (int j = i; j < buf.length(); j++)
            cout << buf[j];
        cout << endl;

        if (analizer.back().isTerminator) {
            if ((analizer.back() == num && isdigit(buf[i])) || (analizer.back().text == string(1, buf[i]))){
                analizer.pop_back();
                i++;
                cout << "[Action]: Reduce " << buf[i - 1] << endl;
            }
            else {
                cout << "[Action]: Unexpected character " << buf[i] << ", popping stack..." << endl;
                analizer.pop_back();
                ret = 1;
            }
        } else {
            Symbol bufSym = recogSymbol(buf[i]);
            if (bufSym.id == -1 || preTable[analizer.back().id][bufSym.id - 5] == -1) {
                cout << "[Action]: Unexpected character " << buf[i] << ", moving on..." << endl;
                ret = 1;
                i++;

            } else if (preTable[analizer.back().id][bufSym.id - 5] == -2) {
                cout << "[Action]: Synchorize character detected, popping stack..." << endl;
                analizer.pop_back();
                ret = 1;
            } else {
                int genId = preTable[analizer.back().id][bufSym.id - 5];
                analizer.pop_back();
                for (int j = syntax[genId].right.size() - 1; j >= 0; j--)
                    if (syntax[genId].right[j] != LL::empty)
                        analizer.push_back(syntax[genId].right[j]);
                cout << "[Action]:  " << syntax[genId].left.text << "->";
                for (int j = 0; j < syntax[genId].right.size(); j++)
                    cout << syntax[genId].right[j].text;
                cout << endl;
            }
        }
        cout << endl;
    }

    return ret;
}

int main(int argc, char *argv[])
{
    loadSyntax();
    for (auto symbol: symbols)
        if (first[symbol.id].empty())
            getFirst(symbol);

    for (auto symbol: symbols)
        if (follow[symbol.id].empty() && !symbol.isTerminator)
            getFollow(symbol);

    cout << "First Set:" << endl;
    for (auto symbol: symbols){
        cout << symbol.text << ": ";
        for (Symbol sym: first[symbol.id])
            cout << sym.text << ' ';
        cout << endl;
    }

    cout << "Follow Set:" << endl;
    for (auto symbol: symbols){
        if (!symbol.isTerminator) {
            cout << symbol.text << ": ";
            for (Symbol sym: follow[symbol.id])
                cout << sym.text << ' ';
            cout << endl;
        }
    }

    makeTable();
    cout << "Predict Table:" << endl;
    cout << "  " ;
    for (int i = 0; i < 8; i++)
        cout << symbols[i + 5].text << ' ';
    cout << endl;
    for (int i = 0; i < 5; i++){
        cout << symbols[i].text << ' ';
        for (int j = 0; j < 8; j++)
            cout << preTable[i][j] << ' ';
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