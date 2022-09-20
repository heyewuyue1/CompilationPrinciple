#include "token.h"

namespace LexicalAnalysis
{

const char * tokenTypeStr[] = {
    "Identifier",
    "Int",
    "Float",
    "Char",
    "String",
    "KeyWord",
    "Operator",
    "Seperator",
    "Error"
};

Token::Token()
{
    _type = Error;
    _attr = '-';
}

Token::Token(TokenType type, string attr):
_type(type), _attr(attr){}

TokenType Token::type()
{
    return _type;
}

void Token::setType(TokenType type)
{
    _type = type;
}

string Token::attr()
{
    return _attr;
}

void Token::setAttr(string attr)
{
    _attr = attr;
}

ostream& operator<<(ostream &cout, const Token& token)
{
    cout << '<' << tokenTypeStr[*(int*)&token._type] << ", " << token._attr << '>';
    return cout;
}

}