#pragma once
#ifndef READER_H
#define READER_H

#include <fstream>
#include "token.h"
using namespace std;

namespace LexicalAnalysis
{

class Reader
{
public:
    Reader(const char *filePath, unsigned int bufferSize);
    int getToken(Token& nextToken);
    ~Reader();

private:
    char forward();
    inline unsigned int fillLeftBuffer();
    inline unsigned int fillRightBuffer();

    ifstream sourceFile;
    unsigned int begin;
    unsigned int end;
    unsigned int _bufferSize;
    char *leftBuffer;
    char *rightBuffer;
};
}

#endif