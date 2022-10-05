#include "reader.h"
#define isLetter(c) ((c) == '_' || ('a' <= (c) && 'z' >= (c)) || ('A' <= (c) && 'Z' >= (c)))
#define isDigit(c) ('0' <= (c) && '9' >= (c))
#define isBlank(c) ((c) == ' ' || (c) == '\n')
#define OCT_REGEX "0[0-7]*(u|l|ul|uL|Ul|UL|lu|lU|Lu|LU|LL|ll|ull|uLL|Ull|ULL|llu|llU|LLu|LLU)?"
#define HEX_FLOAT_REGEX "0(x|X)[[:xdigit:]]+(\\.[[:xdigit:]]+)?(p|P)[-,+]?[0-9]+(f|F|l|L)?"
#define HEX_INT_REGEX "0(x|X)[[:xdigit:]]+(u|l|ul|uL|Ul|UL|lu|lU|Lu|LU|LL|ll|ull|uLL|Ull|ULL|llu|llU|LLu|LLU)?"
#define BIN_REGEX "0(b|B)(0|1)+(u|l|ul|uL|Ul|UL|lu|lU|Lu|LU|LL|ll|ull|uLL|Ull|ULL|llu|llU|LLu|LLU)?"
#define DEC_INT_REGEX "[1-9][0-9]*(u|l|ul|uL|Ul|UL|lu|lU|Lu|LU|LL|ll|ull|uLL|Ull|ULL|llu|llU|LLu|LLU)?"
#define DEC_FLOAT_REGEX "[0-9]*((\\.[0-9]*)|((e|E)[-,+]?[0-9]*)|(\\.[0-9]*)((e|E)[-,+]?[0-9]*))(f|F|l|L)?"


namespace LexicalAnalysis
{

const char *keywordList[32] = {
    "char", "short", "int", "long", "signed", "unsigned", "float", "double", "struct", "union", "enum", "void",
    "for", "do", "while", "break", "continue",
    "if", "else", "goto",
    "switch", "case", "default",
    "return",
    "auto", "extern", "register", "static", "typedef",
    "const", "sizeof", "volatile"
};

Reader::Reader(const char *filePath, unsigned int bufferSize): 
buffer(filePath, bufferSize), begin(), end(), _bufferSize(bufferSize){}

int Reader::getToken(Token& nextToken, Cursor& curLoction)
{
    curLoction = begin;
    // 标识符&保留字
    if (isLetter(buffer[begin])){
        while (isDigit(buffer[end]) || isLetter(buffer[end]))
            forward(1, 1);
        string id("");
        while (begin != end){
            id.append(1, buffer[begin]);
            forward(0, 1);
        }
        forward(1, 1);
        bool isKeyword = false;
        for (int i = 0; i < 32; i++)
        {
            if(!strcmp(keywordList[i], id.c_str())){  // 是保留字
                if (i == 30)
                    nextToken = Token(Operator, "Size of");
                else
                    nextToken = Token(KeyWord, keywordList[i]);
                isKeyword = true;
            }
        }

        if (!isKeyword)  // 如果不是保留字，那就是标识符
            nextToken = Token(Identifier, id);
    }
    else if (buffer[begin] == '(') {
        nextToken = Token(Seperator, "Left Parenthesis");
        forward(0, 1);
        forward(1, 1);
    }
    else if (buffer[begin] == ')') {
        nextToken = Token(Seperator, "Right Parenthesis");
        forward(0, 1);
        forward(1, 1);
    }
    else if (buffer[begin] == '{') {
        nextToken = Token(Seperator, "Left Brace");
        forward(0, 1);
        forward(1, 1);
    }
    else if (buffer[begin] == '}') {
        nextToken = Token(Seperator, "Right Brace");
        forward(0, 1);
        forward(1, 1);
    }
    else if (buffer[begin] == ',') {
        forward(0, 1);
        forward(1, 1);
        nextToken = Token(Seperator, "Comma");
    }
    else if (buffer[begin] == ':') {
        forward(0, 1);
        forward(1, 1);
        nextToken = Token(Seperator, "Colon");
    }
    else if (buffer[begin] == ';') {
        nextToken = Token(Seperator, "Semicolon");
        forward(0, 1);
        forward(1, 1);
    }
    else if (buffer[begin] == '[') {
        nextToken = Token(Operator, "Left Bracket");
        forward(0, 1);
        forward(1, 1);
    }
    else if (buffer[begin] == ']') {
        nextToken = Token(Operator, "Right Bracket");
        forward(0, 1);
        forward(1, 1);
    }
    else if (buffer[begin] == '=') {
        if(buffer[end] == '='){
            forward(0, 2);
            forward(1, 2);
            nextToken = Token(Operator, "Equal");
        }
        else{
            forward(0, 1);
            forward(1, 1);
            nextToken = Token(Operator, "Assignment");
        }
    }
    else if (buffer[begin] == '-') {
        switch (buffer[end]) {
        case '-':
            forward(0, 2);
            forward(1, 2);
            nextToken = Token(Operator, "Decrease");
            break;
        case '>':
            forward(0, 2);
            forward(1, 2);
            nextToken = Token(Operator, "Struct Pointer Attribute");
            break;
        case '=':
            forward(0, 2);
            forward(1, 2);
            nextToken = Token(Operator, "Minus Assign");
            break;
        default:
            forward(0, 1);
            forward(1, 1);
            nextToken = Token(Operator, "Minus");
        }
    }
    else if (buffer[begin] == '+') {
        switch (buffer[end])
        {
        case '+':
            forward(0, 2);
            forward(1, 2);
            nextToken = Token(Operator, "Increase");
            break;
        case '=':
            forward(0, 2);
            forward(1, 2);
            nextToken = Token(Operator, "Plus Assign");
            break;
        default:
            forward(0, 1);
            forward(1, 1);
            nextToken = Token(Operator, "Plus");
        }
    }
    else if (buffer[begin] == '*') {
        if (buffer[end] == '=') {
            forward(0, 2);
            forward(1, 2);
            nextToken = Token(Operator, "Multiply Assign");
        }
        else {
            forward(0, 1);
            forward(1, 1);
            nextToken = Token(Operator, "Star");
        }
    }
    else if (buffer[begin] == '/') {
        if (buffer[end] == '='){
            forward(0, 2);
            forward(1, 2);
            nextToken = Token(Operator, "Division Assign");
        }
        else{
            forward(0, 1);
            forward(1, 1);
            nextToken = Token(Operator, "Division");
        }
    }
    else if (buffer[begin] == '%') {
        if (buffer[end] == '='){
            forward(0, 2);
            forward(1, 2);
            nextToken = Token(Operator, "Mod Assign");
        }
        else{
            forward(0, 1);
            forward(1, 1);
            nextToken = Token(Operator, "Mod");
        }
    }
    else if (buffer[begin] == '!') {
        if (buffer[end] == '=') {
            forward(0, 2);
            forward(1, 2);
            nextToken = Token(Operator, "Not Equal");
        }
        else {
            forward(0, 1);
            forward(1, 1);
            nextToken = Token(Operator, "Logic Not");
        }  
    }
    else if (buffer[begin] == '~') {
        forward(0, 1);
        forward(1, 1);
        nextToken = Token(Operator, "Digit Not");
    }
    else if (buffer[begin] == '&') {
        if (buffer[end] == '&') {
            forward(0, 2);
            forward(1, 2);
            nextToken = Token(Operator, "Logic And");
        }
        else if (buffer[end] == '='){
            forward(0, 2);
            forward(1, 2);
            nextToken = Token(Operator, "And Assign");
        }
        else {
            forward(0, 1);
            forward(1, 1);
            nextToken = Token(Operator, "Digit And");
        }  
    }
    else if (buffer[begin] == '|') {
        if (buffer[end] == '|') {
            forward(0, 2);
            forward(1, 2);
            nextToken = Token(Operator, "Logic Or");
        }
        else if (buffer[end] == '='){
            forward(0, 2);
            forward(1, 2);
            nextToken = Token(Operator, "Or Assign");
        }
        else {
            forward(0, 1);
            forward(1, 1);
            nextToken = Token(Operator, "Digit Or");
        }  
    }
    else if (buffer[begin] == '^') {
        if (buffer[end] == '=') {
            forward(0, 2);
            forward(1, 2);
            nextToken = Token(Operator, "Xor Assign");
        }
        else {
            forward(0, 1);
            forward(1, 1);
            nextToken = Token(Operator, "Digit Xor");
        }  
    }
    else if (buffer[begin] == '<') {
        if (buffer[end] == '<') {
            forward(0, 1);
            forward(1, 1);
            if (buffer[end] == '=') {
                forward(0, 2);
                forward(1, 2);
                nextToken = Token(Operator, "Left Shift Assign");
            }
            else {
                forward(0, 1);
                forward(1, 1);
                nextToken = Token(Operator, "Left Shift");
            }
        }
        else if (buffer[end] == '=') {
            forward(0, 2);
            forward(1, 2);
            nextToken = Token(Operator, "Less Equal");
        }
        else {
            forward(0, 1);
            forward(1, 1);
            nextToken = Token(Operator, "Less");
        }
        
    }
    else if (buffer[begin] == '?') {
        forward(0, 1);
        forward(1, 1);
        nextToken = Token(Operator, "Question Mark");
    }
    else if (buffer[begin] == '>') {
        if (buffer[end] == '>') {
            forward(0, 1);
            forward(1, 1);
            if (buffer[end] == '=') {
                forward(0, 2);
                forward(1, 1);
                forward(1, 1);
                nextToken = Token(Operator, "Right Shift Assign");
            }
            else {
                forward(0, 1);
                forward(1, 1);
                nextToken = Token(Operator, "Right Shift");
            }
        }
        else if (buffer[end] == '=') {
            forward(0, 2);
            forward(1, 2);
            nextToken = Token(Operator, "Greater Equal");
        }
        else {
            forward(0, 1);
            forward(1, 1);
            nextToken = Token(Operator, "Greater");
        }
        
    }  
    else if (buffer[begin] == '"') {
        while (buffer[end] != '"'){
            forward(1, 1);
        }
        forward(0, 1);
        string constStr("");
        while (begin != end){
            if (buffer[begin] == '\\' && buffer[begin.loc + 1] == '\n')
                forward(0, 2);
            constStr.append(1, buffer[begin]);
            forward(0, 1);
        }
        forward(1, 2);
        forward(0, 1);
        nextToken = Token(ConstString, constStr);
    }
    else if (buffer[begin] == '\'') {
        while (buffer[end] != '\''){
            forward(1, 1);
        }
        forward(0, 1);
        string constStr("");
        while (begin != end){
            constStr.append(1, buffer[begin]);
            forward(0, 1);
        }
        forward(1, 2);
        forward(0, 1);
        if (constStr.length() != 1)
            nextToken = Token(Error, "multi-character character constant");
        else
            nextToken = Token(ConstChar, constStr);
    }
    else if (buffer[begin] == '.' && !isDigit(buffer[end])) {
        forward(0, 1);
        forward(1, 1);
        nextToken = Token(Seperator, "dot");
    } 
    else if (isDigit(buffer[begin])  || buffer[begin] == '.')
        nextToken = getNumber();
    skipBlankComment();
    return buffer[begin] == EOF ? 0: 1;
}

void Reader::skipBlankComment()
{
    while (isBlank(buffer[begin]) ||
        (buffer[begin] == '/' && buffer[end] == '/') ||
        (buffer[begin] == '/' && buffer[end] == '*'))
    {
        while (isBlank(buffer[begin])) {
            forward(0, 1);
            forward(1, 1);
        }
        if (buffer[begin] == '/' && buffer[end] == '/') {
            while (buffer[end] != '\n') {
                forward(0, 1);
                forward(1, 1);
            }
            forward(0, 1);
            forward(1, 1);
        }
        if (buffer[begin] == '/' && buffer[end] == '*') {
            while (!(buffer[begin] == '*' && buffer[end] == '/') && buffer[end] != EOF) {
                forward(0, 1);
                forward(1, 1);
            }
            if (buffer[end] == EOF) {
                forward(0, 1);
                forward(1, 1);
            }
            else {
                forward(0, 2);
                forward(1, 2);
            }
        }
    }
}

Token Reader::getNumber()
{
    string num("");
    int retFlag = 0;
    while (isDigit(buffer[end]) || isLetter(buffer[end]) || buffer[end] == '.' || buffer[end] == '-' || buffer[end] == '+')
        forward(1, 1);
    while (begin != end){
        num.append(1, buffer[begin]);
        forward(0, 1);
    }
    forward(1, 1);
    // u, l, ul, lu, ll, ull, llu
    if (regex_match(num, regex(OCT_REGEX)))
        retFlag = 1;
    else if (regex_match(num, regex(HEX_FLOAT_REGEX)))
        retFlag = 2;
    else if (regex_match(num, regex(HEX_INT_REGEX)))
        retFlag = 1;
    else if (regex_match(num, regex(DEC_INT_REGEX)))
        retFlag = 1;
    else if (regex_match(num, regex(DEC_FLOAT_REGEX)))
        retFlag = 2;
    else if (regex_match(num, regex(BIN_REGEX)))
        retFlag = 1;

    switch (retFlag)
    {
    case 1:
        return Token(ConstInt, num);
        break;
    case 2:
        return Token(ConstFloat, num);
        break;
    default:
        return Token(Error, string("Invalid number: ").append(num));
        break;
    }
    
}

char Reader::forward(int check, unsigned int step)
{
    char c;
    if (check)
        for (int i = 0; i < step; i++)
        {
            end.loc++;
            end.row++;
            c = buffer[end];
            if (c =='\n')
                end.nextLine();
            if (end % (2 * _bufferSize) == _bufferSize)
                buffer.fillRightBuffer();
            if (end % (2 * _bufferSize) == 0)
                buffer.fillLeftBuffer();
        }
    else
        for(int i = 0; i < step; i++) {
            begin.loc++;
            begin.row++;
            c = buffer[begin];
            if (c =='\n')
                begin.nextLine();
            c = buffer[begin];
        }
    return c;
}

unsigned int Reader::getCharNum()
{
    return begin.loc;
}

unsigned int Reader::getLineNum()
{
    return begin.line;
}
}