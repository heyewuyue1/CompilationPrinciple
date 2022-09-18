#include "reader.h"
#define isLetter(c) ((c) == '_' || ('a' <= (c) && 'z' >= (c)) || ('A' <= (c) && 'Z' >= (c)))
#define isDigit(c) ('0' <= (c) && '9' >= (c))
#define isBlank(c) ((c) == ' ' || (c) == '\n')



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
        while (isDigit(buffer[end]) || isLetter(buffer[end])){
            forward(1, 1);
        }
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
        if (buffer[end] == '='){
            forward(0, 2);
            forward(1, 2);
            nextToken = Token(Operator, "Multiply Assign");
        }
        else{
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
    // 处理数字
    else if (isDigit(buffer[begin])) {
        bool isFloat, isInt, isError = false;
        // 处理2进制，8进制，16进制整数
        if (buffer[begin] == '0' && buffer[end] != '.') {
            isInt = true;
            if (buffer[end] == 'b' || buffer[end] == 'B') {
                forward(1, 1);
                while (buffer[end] == '0' || buffer[end] == '1')
                    forward(1, 1);
                if (('2' <= buffer[end] && '9' >= buffer[end]) ||
                    ('a' <= buffer[end] && 'f' >= buffer[end]) ||
                    ('A' <= buffer[end] && 'F' >= buffer[end]))
                    isError = true;
            }
            else if (buffer[end]  == 'x' || buffer[end] == 'X') {
                 forward(1, 1);
                while ((buffer[end] >= '0' && buffer[end] <= '9') ||
                    (buffer[end] >= 'a' && buffer[end] <= 'f') ||
                    (buffer[end] >= 'A' && buffer[end] <= 'F'))
                    forward(1, 1);
            }
            else if (isDigit(buffer[end])) {
                forward(1, 1);
                while (buffer[end] >= '0' && buffer[end] <= '7')
                    forward(1, 1);
                if (('8' <= buffer[end] && '9' >= buffer[end]) ||
                ('a' <= buffer[end] && 'f' >= buffer[end]) ||
                ('A' <= buffer[end] && 'F' >= buffer[end]))
                    isError = true;
            }  // 先跳过数字部分

            // 判断尾缀
            if (buffer[end] == 'u' || buffer[end] == 'U') {
                forward(1, 1);
                if (buffer[end] == 'l') {
                    forward(1, 1);
                    if (buffer[end] == 'l')
                        forward(1, 1);
                }
                else if (buffer[end] == 'L') {
                    forward(1, 1);
                    if (buffer[end] == 'L')
                        forward(1, 1);
                }
            }
            else if (buffer[end] == 'l') {
                forward(1, 1);
                if (buffer[end] == 'l')
                    forward(1, 1);
            }
            else if (buffer[end] == 'L') {
                forward(1, 1);
                if (buffer[end] == 'L')
                    forward(1, 1);
            }
            else if (isLetter(buffer[end]) || '_' == buffer[end] || '.' == buffer[end])
                isError = true;
        }

        while (isDigit(buffer[end]))
            forward(1, 1);
        if (buffer[end] == '.') {
            isFloat = true;
            forward(1, 1);
            while (isDigit(buffer[end]))
                forward(1, 1);
            if (buffer[end] == 'e' || buffer[end] == 'E') {
                forward(1, 1);
                if (buffer[end] == '-')
                    forward(1, 1);
                while (isDigit(buffer[end]))
                    forward(1, 1);
            }
            if (buffer[end] == 'l'||buffer[end] == 'L' ||
                buffer[end] == 'f'||buffer[end] == 'F')
                forward(1, 1);
            else if (isLetter(buffer[end]) || '_' == buffer[end] || '.' == buffer[end])
                isError = true;
        }
        else if (buffer[end] == 'e' || buffer[end] == 'E') {
            isFloat = true;
            forward(1, 1);
            if (buffer[end] == '-')
                forward(1, 1);
            while (isDigit(buffer[end]))
                forward(1, 1);
            if (buffer[end] == 'l'||buffer[end] == 'L' ||
                buffer[end] == 'f'||buffer[end] == 'F')
                forward(1, 1);
            else if (isLetter(buffer[end]) || '_' == buffer[end] || '.' == buffer[end])
                isError = true;
        }
        else if (buffer[end] == 'u' || buffer[end] == 'U') {
            forward(1, 1);
            if (buffer[end] == 'l') {
                forward(1, 1);
                if (buffer[end] == 'l')
                    forward(1, 1);
                else if (isLetter(buffer[end]) || '_' == buffer[end] || '.' == buffer[end])
                    isError = true;
            }
            else if (buffer[end] == 'L') {
                forward(1, 1);
                if (buffer[end] == 'L')
                    forward(1, 1);
                else if (isLetter(buffer[end]) || '_' == buffer[end] || '.' == buffer[end])
                    isError = true;
            }
            else if (isLetter(buffer[end]) || '_' == buffer[end] || '.' == buffer[end])
                isError = true;
        }
        else if (buffer[end] == 'l') {
            forward(1, 1);
            if (buffer[end] == 'l')
                forward(1, 1);
            else if (isLetter(buffer[end]) || '_' == buffer[end] || '.' == buffer[end])
                isError = true;
        }
        else if (buffer[end] == 'L') {
            forward(1, 1);
            if (buffer[end] == 'L')
                forward(1, 1);
            else if (isLetter(buffer[end]) || '_' == buffer[end] || '.' == buffer[end])
                isError = true;
        }
        else if ((buffer[end] == 'f' || buffer[end] == 'F') && !isInt){
            isFloat = true;
            forward(1, 1);
        }
        else if (isLetter(buffer[end]) || '_' == buffer[end] || '.' == buffer[end])
            isError = true;
        if (!isError) {
            string num("");
            while (begin != end){
                num.append(1, buffer[begin]);
                forward(0, 1);
            }
            nextToken = isFloat ? Token(ConstFloat, num): Token(ConstInt, num);
        }
        else
            nextToken = digitsError();
    }
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

Token Reader::digitsError()
{
    string ret("");
    while (isDigit(buffer[end]) || isLetter(buffer[end]) || '_' == buffer[end] || '.' == buffer[end])
        forward(1, 1);
    while (begin.loc != end.loc) {
        ret.append(1, buffer[begin]);
        forward(0, 1);
    }
    forward(1, 1);

    return Token(Error, string("Invalid number \'").append(ret).append("\'"));
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