#ifndef CPARSE_H
#define CPARSE_H

#include <fstream>
#include <string>

namespace CGL {


class CParse
{
protected:
    char* d;
public:
    CParse(char *string = 0);
    CParse(const char *string);
    static CParse parseFile(const std::string& filename);
    inline char *data() { return d; }

    void UNIXRowSymbol();

    int integer();
    double real();
    std::string string();
    std::string word();
    float fixFloat8();

    void skipRow();
    void skipTo(const char* dest);
    inline void skipTo(const std::string& dest) { skipTo(dest.c_str()); }
    bool testPrew(const char* dest);
    inline bool testPrew(const std::string& dest) { return testPrew(dest.c_str()); }

    CParse& operator +=(int v) { d += v; return *this; }
    CParse& operator ++() { ++d; return *this; }
    CParse& operator ++(int) { ++d; return *this; }
    int operator- (const CParse& p) const { return d - p.d; }
    char operator *() const { return *d; }
    char operator [](int i) const { return d[i]; }
};
typedef CParse Parse;




}

#endif // CPARSE_H
