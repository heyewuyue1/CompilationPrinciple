#include "reader.h"
#define getChar(loc) ((loc) % (2 * _bufferSize) < _bufferSize ? \
                leftBuffer[(loc) % (2 * _bufferSize)]: \
                rightBuffer[(loc) % (2 * _bufferSize) - _bufferSize])
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

Reader::Reader(const char *filePath, unsigned int bufferSize)
{
    sourceFile.open(filePath, ios::in);
    if (!sourceFile.is_open()){
        cout << "File not exist!" << endl;
        exit(0);
    }
    _bufferSize = bufferSize;
    leftBuffer = new char[_bufferSize];
    rightBuffer = new char[_bufferSize];
    fillLeftBuffer();
    begin = 0;
    end = 1;
}

int Reader::getToken(Token& nextToken)
{
    // 标识符&保留字
    if (isLetter(getChar(begin))){
        // cout << "isLetter";
        while (isDigit(getChar(end)) || isLetter(getChar(end))){
            forward();
        }
        string id("");
        while (begin != end){
            id.append(1, getChar(begin));
            begin++;
        }
        forward();
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
    else if (getChar(begin) == '(') {
        nextToken = Token(Seperator, "Left Parenthesis");
        begin++;
        forward();
    }
    else if (getChar(begin) == ')') {
        nextToken = Token(Seperator, "Right Parenthesis");
        begin++;
        forward();
    }
    else if (getChar(begin) == '{') {
        nextToken = Token(Seperator, "Left Brace");
        begin++;
        forward();
    }
    else if (getChar(begin) == '}') {
        nextToken = Token(Seperator, "Right Brace");
        begin++;
        forward();
    }
    else if (getChar(begin) == ',') {
        begin++;
        forward();
        nextToken = Token(Seperator, "Comma");
    }
    else if (getChar(begin) == ':') {
        begin++;
        forward();
        nextToken = Token(Seperator, "Colon");
    }
    else if (getChar(begin) == ';') {
        nextToken = Token(Seperator, "Semicolon");
        begin++;
        forward();
    }
    else if (getChar(begin) == '[') {
        nextToken = Token(Operator, "Left Bracket");
        begin++;
        forward();
    }
    else if (getChar(begin) == ']') {
        nextToken = Token(Operator, "Right Bracket");
        begin++;
        forward();
    }
    else if (getChar(begin) == '=') {
        if(getChar(end) == '='){
            begin += 2;
            forward();
            forward();
            nextToken = Token(Operator, "Equal");
        }
        else{
            begin++;
            forward();
            nextToken = Token(Operator, "Assignment");
        }
    }
    else if (getChar(begin) == '-') {
        switch (getChar(end)) {
        case '-':
            begin += 2;
            forward();
            forward();
            nextToken = Token(Operator, "Decrease");
            break;
        case '>':
            begin += 2;
            forward();
            forward();
            nextToken = Token(Operator, "Struct Pointer Attribute");
            break;
        case '=':
            begin += 2;
            forward();
            forward();
            nextToken = Token(Operator, "Minus Assign");
            break;
        default:
            begin++;
            forward();
            nextToken = Token(Operator, "Minus");
        }
    }
    else if (getChar(begin) == '+') {
        switch (getChar(end))
        {
        case '+':
            begin += 2;
            forward();
            forward();
            nextToken = Token(Operator, "Increase");
            break;
        case '=':
            begin += 2;
            forward();
            forward();
            nextToken = Token(Operator, "Plus Assign");
            break;
        default:
            begin++;
            forward();
            nextToken = Token(Operator, "Plus");
        }
    }
    else if (getChar(begin) == '*') {
        if (getChar(end) == '='){
            begin += 2;
            forward();
            forward();
            nextToken = Token(Operator, "Multiply Assign");
        }
        else{
            begin++;
            forward();
            nextToken = Token(Operator, "Star");
        }
    }
    else if (getChar(begin) == '/') {
        if (getChar(end) == '='){
            begin += 2;
            forward();
            forward();
            nextToken = Token(Operator, "Division Assign");
        }
        else{
            begin++;
            forward();
            nextToken = Token(Operator, "Division");
        }
    }
    else if (getChar(begin) == '%') {
        if (getChar(end) == '='){
            begin += 2;
            forward();
            forward();
            nextToken = Token(Operator, "Mod Assign");
        }
        else{
            begin++;
            forward();
            nextToken = Token(Operator, "Mod");
        }
    }
    else if (getChar(begin) == '!') {
        if (getChar(end) == '=') {
            begin += 2;
            forward();
            forward();
            nextToken = Token(Operator, "Not Equal");
        }
        else {
            begin++;
            forward();
            nextToken = Token(Operator, "Logic Not");
        }  
    }
    else if (getChar(begin) == '~') {
        begin++;
        forward();
        nextToken = Token(Operator, "Digit Not");
    }
    else if (getChar(begin) == '&') {
        if (getChar(end) == '&') {
            begin += 2;
            forward();
            forward();
            nextToken = Token(Operator, "Logic And");
        }
        else if (getChar(end) == '='){
            begin += 2;
            forward();
            forward();
            nextToken = Token(Operator, "And Assign");
        }
        else {
            begin++;
            forward();
            nextToken = Token(Operator, "Digit And");
        }  
    }
    else if (getChar(begin) == '|') {
        if (getChar(end) == '|') {
            begin += 2;
            forward();
            forward();
            nextToken = Token(Operator, "Logic Or");
        }
        else if (getChar(end) == '='){
            begin += 2;
            forward();
            forward();
            nextToken = Token(Operator, "Or Assign");
        }
        else {
            begin++;
            forward();
            nextToken = Token(Operator, "Digit Or");
        }  
    }
    else if (getChar(begin) == '^') {
        if (getChar(end) == '=') {
            begin += 2;
            forward();
            forward();
            nextToken = Token(Operator, "Xor Assign");
        }
        else {
            begin++;
            forward();
            nextToken = Token(Operator, "Digit Xor");
        }  
    }
    else if (getChar(begin) == '<') {
        if (getChar(end) == '<') {
            begin++;
            forward();
            if (getChar(end) == '=') {
                begin += 2;
                forward();
                forward();
                nextToken = Token(Operator, "Left Shift Assign");
            }
            else {
                begin++;
                forward();
                nextToken = Token(Operator, "Left Shift");
            }
        }
        else if (getChar(end) == '=') {
            begin += 2;
            forward();
            forward();
            nextToken = Token(Operator, "Less Equal");
        }
        else {
            begin++;
            forward();
            nextToken = Token(Operator, "Less");
        }
        
    }
    else if (getChar(begin) == '>') {
        if (getChar(end) == '>') {
            begin++;
            forward();
            if (getChar(end) == '=') {
                begin += 2;
                forward();
                forward();
                nextToken = Token(Operator, "Right Shift Assign");
            }
            else {
                begin++;
                forward();
                nextToken = Token(Operator, "Right Shift");
            }
        }
        else if (getChar(end) == '=') {
            begin += 2;
            forward();
            forward();
            nextToken = Token(Operator, "Greater Equal");
        }
        else {
            begin++;
            forward();
            nextToken = Token(Operator, "Greater");
        }
        
    }  
    else if (getChar(begin) == '"') {
        while (getChar(end) != '"'){
            forward();
        }
        begin++;
        string constStr("");
        while (begin != end){
            constStr.append(1, getChar(begin));
            begin++;
        }
        forward();
        forward();
        begin++;
        nextToken = Token(ConstString, constStr);
    }
    else if (getChar(begin) == '\'') {
        while (getChar(end) != '\''){
            forward();
        }
        begin++;
        string constStr("");
        while (begin != end){
            constStr.append(1, getChar(begin));
            begin++;
        }
        forward();
        forward();
        begin++;
        if (constStr.length() != 1)
            nextToken = Token(Error, "multi-character character constant");
        else
            nextToken = Token(ConstChar, constStr);
    }
    else if (isDigit(getChar(begin))) {
        while (isDigit(getChar(end))){
            forward();
        }
        string id("");
        while (begin != end){
            id.append(1, getChar(begin));
            begin++;
        }
        forward();
        nextToken = Token(ConstInt, id);
    }
    skipBlankComment();

    if (getChar(begin) == EOF)
    {
        return 0;
    }
    return 1;
}

void Reader::skipBlankComment()
{
    while (isBlank(getChar(begin)) ||
        (getChar(begin) == '/' && getChar(end) == '/') ||
        (getChar(begin) == '/' && getChar(end) == '*'))
    {
        while (isBlank(getChar(begin)))
        {
            begin++;
            forward();
        }
        if (getChar(begin) == '/' && getChar(end) == '/')
        {
            while (getChar(end) != '\n')
            {
                begin++;
                forward();
            }
            begin++;
            forward();
        }
        if (getChar(begin) == '/' && getChar(end) == '*')
        {
            while (!(getChar(begin) == '*' && getChar(end) == '/') && getChar(end) != EOF)
            {
                begin++;
                forward();
            }
            if (getChar(end) == EOF)
            {
                begin++;
                forward();
            }
            else{
                begin += 2;
                forward();
                forward();
            }
        }
    }
}

unsigned int Reader::fillLeftBuffer()
{
    sourceFile.read(leftBuffer, _bufferSize);
    int count = sourceFile.gcount();
    if (count < _bufferSize)
        leftBuffer[count] = EOF;
    return count;
}

unsigned int Reader::fillRightBuffer()
{
    sourceFile.read(rightBuffer, _bufferSize);
    int count = sourceFile.gcount();
    if (count < _bufferSize)
        rightBuffer[count] = EOF;
    return count;
}

char Reader::forward()
{
    char c;
    if (end % (2 * _bufferSize) < _bufferSize)
        c = leftBuffer[end % (2 * _bufferSize)];
    else
        c = rightBuffer[end % (2 * _bufferSize) - _bufferSize];
    
    end++;
    if (end % (2 * _bufferSize) == _bufferSize)
        fillRightBuffer();
    if (end % (2 * _bufferSize) == 0)
        fillLeftBuffer();

    return c;
}

Reader::~Reader()
{
    delete[] leftBuffer;
    delete[] rightBuffer;
}

}