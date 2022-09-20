#pragma once
#ifndef READER_H
#define READER_H

#include <fstream>
#include <regex>
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
    Token getNumber();
    Cursor begin, end;
    Buffer buffer;
    unsigned int _bufferSize;
};
}

#endif