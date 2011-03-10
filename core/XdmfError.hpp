#ifndef XDMFERROR_HPP_
#define XDMFERROR_HPP_

#include <iostream>
#include <sstream>

#include "XdmfCore.hpp"

class XDMFCORE_EXPORT XdmfError
{
public:
    XdmfError();
    ~XdmfError();

    enum Level {FATAL, WARNING, DEBUG};

    static void setLevel(Level l);
    static Level getLevel();
    static void message(Level l, std::string msg);

    static void setBuffer(std::streambuf* buf);
    static std::streambuf* getInternalBuffer();
    static std::string getInternalString();

private:
    static Level mLevel;
    static std::streambuf* mBuf;
    static std::ostringstream mStream;
    static std::string newline;

    static void WriteToStream(std::string msg);
};

#endif
