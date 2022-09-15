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

Reader::Reader(const char *filePath, unsigned int bufferSize=1000)
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
    // cout << "begin: " << begin << endl;
    // cout << "end: " << end << endl;
    char first;  
    first = getChar(begin);
    if (isLetter(first)){
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
            if(!strcmp(keywordList[i], id.c_str())){
                nextToken = Token(KeyWord, keywordList[i]);
                isKeyword = true;
            }
        }
        if (!isKeyword)
        {
            nextToken = Token(Identifier, id);
        }
    }
    else if (getChar(begin) == '(')
    {
        nextToken = Token(Seperator, "Left Parenthesis");
        begin++;
        forward();
    }
    else if (getChar(begin) == ')')
    {
        nextToken = Token(Seperator, "Right Parenthesis");
        begin++;
        forward();
    }
    else if (getChar(begin) == '{')
    {
        nextToken = Token(Seperator, "Left Brace");
        begin++;
        forward();
    }
    else if (getChar(begin) == '}')
    {
        nextToken = Token(Seperator, "Right Brace");
        begin++;
        forward();
    }
    else if (getChar(begin) == ';')
    {
        nextToken = Token(Seperator, "Semicolon");
        begin++;
        forward();
    }
    else if (getChar(begin) == '=')
    {
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
    else if (isDigit(getChar(begin)))
    {
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
    
    
    while (isBlank(getChar(begin)))
    {
        begin++;
        forward();
    }

    if (getChar(begin) == EOF)
    {
        return 0;
    }
    return 1;
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