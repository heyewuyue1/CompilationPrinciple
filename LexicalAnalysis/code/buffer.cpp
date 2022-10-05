#include "buffer.h"
namespace LexicalAnalysis
{

Buffer::Buffer(const char * filePath, unsigned int bufferSize)
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
}

unsigned int Buffer::fillLeftBuffer()
{
    sourceFile.read(leftBuffer, _bufferSize);
    int count = sourceFile.gcount();
    if (count < _bufferSize)
        leftBuffer[count] = EOF;
    return count;
}

unsigned int Buffer::fillRightBuffer()
{
    sourceFile.read(rightBuffer, _bufferSize);
    int count = sourceFile.gcount();
    if (count < _bufferSize)
        rightBuffer[count] = EOF;
    return count;
}

char Buffer::operator[](int loc)
{
    return loc % (2 * _bufferSize) < _bufferSize ? 
            leftBuffer[loc % (2 * _bufferSize)]:
            rightBuffer[loc % (2 * _bufferSize) - _bufferSize];
}

Buffer::~Buffer()
{
    sourceFile.close();
    delete[] leftBuffer;
    delete[] rightBuffer;
}

} 