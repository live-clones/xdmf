#include <XdmfError.hpp>

XdmfError::XdmfError()
{}

XdmfError::~XdmfError()
{}

/*******************************
 *** Public Static Functions ***
 *******************************/

XdmfError::Level
XdmfError::getLevel()
{
    return XdmfError::mLevel;
}
void
XdmfError::setLevel(Level l)
{
    XdmfError::mLevel = l;
}

void
XdmfError::message(Level level, std::string msg)
{
    if(level<=XdmfError::getLevel())
        XdmfError::WriteToStream(msg);
}

void
XdmfError::setBuffer(std::streambuf* buf)
{
    XdmfError::mBuf = buf;
}

std::streambuf*
XdmfError::getInternalBuffer()
{
    return XdmfError::mStream.rdbuf();
}

std::string
XdmfError::getInternalString()
{
    return XdmfError::mStream.str();
}

/********************************
 *** Private Static Functions ***
 ********************************/

void
XdmfError::WriteToStream(std::string msg)
{
    if(msg[msg.length()-1] != XdmfError::newline[0])
        msg+=XdmfError::newline;
    XdmfError::mBuf->sputn(msg.c_str(),msg.length());
}

/******************************************
 *** Initialize Static Member Variables ***
 ******************************************/

XdmfError::Level XdmfError::mLevel = FATAL;
std::streambuf* XdmfError::mBuf=std::cout.rdbuf();
std::ostringstream XdmfError::mStream;
std::string XdmfError::newline = "\n";
