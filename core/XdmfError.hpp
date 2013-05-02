#ifndef XDMFERROR_HPP_
#define XDMFERROR_HPP_

#include <iosfwd>
#include <sstream>
#include <exception>

#include "XdmfCore.hpp"

class XDMFCORE_EXPORT XdmfError  : public std::exception
{
public:
    enum Level {FATAL, WARNING, DEBUG};

    /**
     * One of the constructors for XdmfError, this one doesn't print to the buffer.
     *
     * Example of use:
     *
     * C++
     *
     * @dontinclude ExampleXdmfError.cpp
     * @skipline DEBUG
     *
     * Python:
     *
     * @dontinclude XdmfExampleError.py
     * @skip exampleError
     * @skipline try
     * @until setLevel
     *
     * @param	level	the error level of the exception being constructed
     * @param	message	the message to be attached to the exception
     */
    XdmfError(Level level, std::string message);

    ~XdmfError() throw();

    /**
     * Sets the error level of the exception.
     *
     * Example of use:
     *
     * C++
     *
     * @dontinclude ExampleXdmfError.cpp
     * @skipline try
     * @until catch
     * @skipline {
     * @skipline setLevel
     * @skipline }
     *
     * Python:
     *
     * @dontinclude XdmfExampleError.py
     * @skip exampleError
     * @skipline try
     * @until setLevel
     *
     * @param	l	the new level of the exception
     */
    void setLevel(Level l);

    /**
     * Gets the error level of the exception.
     *
     * Example of use:
     *
     * C++
     *
     * @dontinclude ExampleXdmfError.cpp
     * @skipline try
     * @until catch
     * @skipline {
     * @skipline getLevel
     * @skipline }
     *
     * Python:
     *
     * @dontinclude XdmfExampleError.py
     * @skip exampleError
     * @skipline try
     * @until getLevel
     *
     * @return	the error level of the exception
     */
    Level getLevel();

    /**
     * Sets the level limit for Errors. This determines what level of errors will be thrown with message.
     *
     * Example of use:
     *
     * C++
     *
     * @dontinclude ExampleXdmfError.cpp
     * @skipline setLevelLimit
     *
     * Python
     *
     * @dontinclude XdmfExampleError.py
     * @skipline setLevelLimit
     *
     * @param	l	the cutoff level for sending exceptions via message
     */
    static void setLevelLimit(Level l);

    /**
     * Gets the level limit for Errors.
     *
     * Example of use:
     *
     * C++
     *
     * @dontinclude ExampleXdmfError.cpp
     * @skipline getLevelLimit
     *
     * Python
     *
     * @dontinclude XdmfExampleError.py
     * @skipline getLevelLimit
     *
     * @return	gets the cuttof level for sending exceptions via message
     */
    static Level getLevelLimit();

    /**
     * Alternate constructor for XdmfError exceptions.
     * This one automatically prints out the message provided if the error level is within range.
     * If not within range an error will not be thrown
     *
     * Example of use:
     *
     * C++
     *
     * @dontinclude ExampleXdmfError.cpp
     * @skipline try
     * @until catch
     * @skipline {
     * @skipline what
     * @skipline }
     *
     * Python: Generates a RuntimeError instead of an XdmfError in Python
     *
     * @dontinclude XdmfExampleError.py
     * @skipline try
     * @until print
     *
     * @param	l	the level of the error to be generated
     * @param	msg	the message to be associated with the error generated and printed out
     */
    static void message(Level l, std::string msg);

    /**
     * Sets which buffer the error messages are printed to with the message function.
     *
     * Example of use:
     *
     * C++
     *
     * @dontinclude ExampleXdmfError.cpp
     * @skipline setBuffer
     *
     * Python: Not supported in Python
     *
     * @param	buf	the buffer that the error messages will be printed to
     */
    static void setBuffer(std::streambuf* buf);

    /**
     * Gets the message associated with this exception.
     *
     * Example of use:
     *
     * C++
     *
     * @dontinclude ExampleXdmfError.cpp
     * @skipline try
     * @until catch
     * @skipline {
     * @skipline what
     * @skipline }
     *
     * Python
     *
     * @dontinclude XdmfExampleError.py
     * @skip exampleError
     * @skipline try
     * @until what
     *
     * @return	the message associated with the exception
     */
    virtual const char * what() const throw();

private:
    Level mLevel;
    static Level mLevelLimit;
    static std::streambuf* mBuf;
    std::string mMessage;

    static void WriteToStream(std::string msg);
};

#endif
