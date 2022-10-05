#pragma once
#include <string>
#include <iostream>
using namespace std;

namespace LexicalAnalysis
{

    enum TokenType
    {
        Identifier,  // 标志符
        ConstInt,    // 整数常量
        ConstFloat,  // 浮点常量
        ConstChar,   // 字符常量
        ConstString, // 字符串常量
        KeyWord,     // 关键字
        Operator,    // 运算符
        Seperator,   // 分隔符
        Error        // 错误标记
    };

    extern const char *tokenTypeStr[];  // 描述记号类型的文字常量

    class Token
    {
    public:
        Token(TokenType type, string attr);
        Token();

        TokenType type();
        void setType(TokenType);
        string attr();
        void setAttr(string);

        friend ostream &operator<<(ostream &, const Token &);

    private:
        TokenType _type;
        string _attr;
    };
}