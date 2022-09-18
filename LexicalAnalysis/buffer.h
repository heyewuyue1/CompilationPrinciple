#include <fstream>
#include <iostream>
using namespace std;
namespace LexicalAnalysis
{
class Buffer {
public:
    Buffer(const char *, unsigned int);
    unsigned int fillLeftBuffer();
    unsigned int fillRightBuffer();
    char operator[](int loc);
    ~Buffer();
private:
    std::ifstream sourceFile;
    unsigned int _bufferSize;
    char *leftBuffer;
    char *rightBuffer;
};
}