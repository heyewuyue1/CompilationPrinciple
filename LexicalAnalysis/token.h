#pragma once
#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>
using namespace std;

namespace LexicalAnalysis
{

enum TokenType{
    Identifier,
    ConstInt,
    ConstFloat,
    ConstChar,
    ConstString,
    KeyWord,
    Operator,
    Seperator,
    Error
};

extern const char * tokenTypeStr[];

class Token
{
public:
    Token(TokenType type, string attr);
    Token();

    TokenType type();
    void setType(TokenType);
    string attr();
    void setAttr(string);

    friend ostream& operator<<(ostream&, const Token&);

private:
    TokenType _type;
    string _attr;
};
}

#endif