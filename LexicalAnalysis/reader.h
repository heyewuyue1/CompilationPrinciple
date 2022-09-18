#pragma once
#ifndef READER_H
#define READER_H

#include <fstream>
#include "token.h"
#include "buffer.h"
#include "cursor.h"
using namespace std;

namespace LexicalAnalysis
{

class Reader
{
public:
    Reader(const char *filePath, unsigned int bufferSize=1000);
    int getToken(Token& nextToken, Cursor& curLoction);
    unsigned int getCharNum();
    unsigned int getLineNum();
private:
    char forward(int check, unsigned int step);
    void skipBlankComment();
    Token digitsError();
    Cursor begin, end;
    Buffer buffer;
    unsigned int _bufferSize;
};
}

#endif