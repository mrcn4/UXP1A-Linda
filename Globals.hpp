#ifndef GLOBALS_HPP
#define GLOBALS_HPP

struct Globals{
    static const int c_MaxFDExclusive = 1024;
    static const int c_ClientTimeoutSeconds = 3;
    static const int c_WriteFD = c_MaxFDExclusive-1;
    static const int c_ReadFD = c_MaxFDExclusive-2;
};

#endif
