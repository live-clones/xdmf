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
     * @skipline //#initialization
     * @until //#initialization
     *
     * Python:
     *
     * @dontinclude XdmfExampleError.py
     * @skipline #//initialization
     * @until #//initialization
     *
     * @param	level		the error level of the exception being constructed
     * @param	message		the message to be attached to the exception
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
     * @skipline //#initialization
     * @until //#initialization
     * @skipline //#setLevel
     * @until //#setLevel
     *
     * Python:
     *
     * @dontinclude XdmfExampleError.py
     * @skipline #//setLevel
     * @until #//setLevel
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
     * @skipline //#initialization
     * @until //#initialization
     * @skipline //#getLevel
     * @until //#getLevel
     *
     * Python:
     *
     * @dontinclude XdmfExampleError.py
     * @skipline #//getLevel
     * @until #//getLevel
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
     * @skipline //#initialization
     * @until //#initialization
     * @skipline //#setLevelLimit
     * @until //#setLevelLimit
     *
     * Python
     *
     * @dontinclude XdmfExampleError.py
     * @skipline #//setLevelLimit
     * @until #//setLevelLimit
     *
     * @param	l	the cutoff level for sending exceptions via message
     */
    static void setLevelLimit(Level l);

    /**
     * Sets the minimum Error level that displays messages with the message function.
     *
     * Example of use:
     *
     * C++
     *
     * @dontinclude ExampleXdmfError.cpp
     * @skipline //#setSuppressionLevel
     * @until //#setSuppressionLevel
     *
     * Python
     *
     * @dontinclude XdmfExampleError.py
     * @skipline #//setSuppressionLevel
     * @until #//setSuppressionLevel
     *
     * @param	l	The new minimum error level to display a message
     */
    static void setSuppressionLevel(Level l);

    /**
     * Gets the level limit for Errors.
     *
     * Example of use:
     *
     * C++
     *
     * @dontinclude ExampleXdmfError.cpp
     * @skipline //#initialization
     * @until //#initialization
     * @skipline //#getLevelLimit
     * @until //#getLevelLimit
     *
     * Python
     *
     * @dontinclude XdmfExampleError.py
     * @skipline #//getLevelLimit
     * @until #//getLevelLimit
     *
     * @return	gets the cuttof level for sending exceptions via message
     */
    static Level getLevelLimit();

    /**
     * Gets the minimum Error level that displays messages with the message function.
     *
     * @dontinclude ExampleXdmfError.cpp
     * @skipline //#getSuppressionLevel
     * @until //#getSuppressionLevel
     *
     * Python
     *
     * @dontinclude XdmfExampleError.py
     * @skipline #//getSuppressionLevel
     * @until #//getSuppressionLevel
     *
     * @return	The minimum error level to display a message
     */
    static Level getSuppressionLevel();

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
     * @skipline //#initialization
     * @until //#initialization
     * @skipline //#message
     * @until //#message
     *
     * Python: Generates a RuntimeError instead of an XdmfError in Python
     *
     * @dontinclude XdmfExampleError.py
     * @skipline #//message
     * @until #//message
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
     * @skipline //#initialization
     * @until //#initialization
     * @skipline //#setBuffer
     * @until //#setBuffer
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
     * @skipline //#initialization
     * @until //#initialization
     * @skipline //#what
     * @until //#what
     *
     * Python
     *
     * @dontinclude XdmfExampleError.py
     * @skipline #//what
     * @until #//what
     *
     * @return	the message associated with the exception
     */
    virtual const char * what() const throw();

private:
    Level mLevel;
    static Level mLevelLimit;
    static Level mSuppressLevel;
    static std::streambuf* mBuf;
    std::string mMessage;

    static void WriteToStream(std::string msg);
};

#endif
