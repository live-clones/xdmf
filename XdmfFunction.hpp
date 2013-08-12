/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfFunction.hpp                                                    */
/*                                                                           */
/*  Author:                                                                  */
/*     Kenneth Leiter                                                        */
/*     kenneth.leiter@arl.army.mil                                           */
/*     US Army Research Laboratory                                           */
/*     Aberdeen Proving Ground, MD                                           */
/*                                                                           */
/*     Copyright @ 2011 US Army Research Laboratory                          */
/*     All Rights Reserved                                                   */
/*     See Copyright.txt for details                                         */
/*                                                                           */
/*     This software is distributed WITHOUT ANY WARRANTY; without            */
/*     even the implied warranty of MERCHANTABILITY or FITNESS               */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice             */
/*     for more information.                                                 */
/*                                                                           */
/*****************************************************************************/

#ifndef XDMFFUNCTION_HPP_
#define XDMFFUNCTION_HPP_

// Includes
#include "Xdmf.hpp"
#include "XdmfArray.hpp"

/**
 * @brief Manipulates arrays based on expressions.
 *
 * The function class provides a way to manipulate XdmfArrays via predefined functions.
 */
class XDMF_EXPORT XdmfFunction : public XdmfItem {

public:

  /**
   * Create a new XdmfFunction
   *
   * @return constructed XdmfFunction.
   */
  static shared_ptr<XdmfFunction> New();

  virtual ~XdmfFunction();

  LOKI_DEFINE_VISITABLE(XdmfFunction, XdmfItem);

  static const std::string ItemTag;

  /**
   * Adds an operation to the list of viable operators.
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline //#declareoperation
   * @until //#declareoperation
   * @skipline //#programstart
   * @until //#programstart
   * @skipline //#addOperation
   * @until //#addOperation
   * @skipline //#programend
   * @until //#programend
   * @skipline //#defineoperation
   * @until //#defineoperation
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline #//defineoperation
   * @until #//defineoperation
   * @skipline #//programstart
   * @until #//programstart
   * @skipline #//addOperation
   * @until #//addOperation
   *
   * @param	newoperator	the character to be associated with the provided binary operation
   * @param	functionref	a pointer to the function to be associated with the provided operator
   * @param	priority	used to determine order of operations the higher the value the earlier it is evaluated
   */
  static int addOperation(char newoperator,
                          shared_ptr<XdmfArray>(*functionref)(shared_ptr<XdmfArray>, shared_ptr<XdmfArray>),
                          int priority);

  /*
   * adds a specified function to the list of functions used while evaluating strings
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline //#declarefunction
   * @until //#declarefunction
   * @skipline //#programstart
   * @until //#programstart
   * @skipline //#addFunction
   * @until //#addFunction
   * @skipline //#programend
   * @until //#programend
   * @skipline //#definefunction
   * @until //#definefunction
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline #//definefunction
   * @until #//definefunction
   * @skipline #//programstart
   * @until #//programstart
   * @skipline #//addFunction
   * @until #//addFunction
   *
   * @param	name		A string to be associated with the provided function during string evaluation
   * @param	functionref	A pointer to the function to be associated with the given string
   * @return			The total number of functions currently associated
   */
  static int addFunction(std::string name,
                         shared_ptr<XdmfArray>(*functionref)(std::vector<shared_ptr<XdmfArray> >));

  /**
   * Averages the values contained in all the provided arrays.
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline //#valueinit
   * @until //#valueinit
   * @skipline //#average
   * @until //#average
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline #//valueinit
   * @until #//valueinit
   * @skipline #//average
   * @until #//average
   *
   * @param	values	a vector containing the arrays to be used
   * @return		an XdmfArray containing  one value which is the average of all values contained within the provided arrays
   */
  static shared_ptr<XdmfArray> average(std::vector<shared_ptr<XdmfArray> > values);

  /**
   * Joins the two provided arrays together end to end.
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline //#valueinit
   * @until //#valueinit
   * @skipline //#chunk
   * @until //#chunk
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline #//valueinit
   * @until #//valueinit
   * @skipline #//chunk
   * @until #//chunk
   *
   * @param	val1	the first array being evaluated
   * @param	val2	the second array being evaluated
   * @return		the arrays joined end to end
   */
  static shared_ptr<XdmfArray> chunk(shared_ptr<XdmfArray> val1,
                                     shared_ptr<XdmfArray> val2);

  /**
   * Evaluates an expression based on the list of variables provided.
   * A list of valid operations is retrievable from the getSupportedOperations static method.
   * None of the XdmfArrays provided are modified during the evaluation process.
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline //#declarefunction
   * @until //#declarefunction
   * @skipline //#declareoperation
   * @until //#declareoperation
   * @skipline //#programstart
   * @until //#programstart
   * @skipline //#valueinit
   * @until //#valueinit
   * @skipline //#addOperation
   * @until //#addOperation
   * @skipline //#addFunction
   * @until //#addFunction
   * @skipline //#evaluateExpression
   * @until //#evaluateExpression
   * @skipline //#programend
   * @until //#programend
   * @skipline //#definefunction
   * @until //#definefunction
   * @skipline //#defineoperation
   * @until //#defineoperation
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline #//definefunction
   * @until #//definefunction
   * @skipline #//defineoperation
   * @until #//defineoperation
   * @skipline #//programstart
   * @until #//programstart
   * @skipline #//valueinit
   * @until #//valueinit
   * @skipline #//addOperation
   * @until #//addOperation
   * @skipline #//addFunction
   * @until #//addFunction
   * @skipline #//evaluateExpression
   * @until #//evaluateExpression
   *
   * @param	expression	a string containing the expresion to be evaluated
   * @param	variables	a map of strings to their XdmfArray equivalent
   * @return			a shared pointer to the XdmfArray resulting from the expression
   */
  static shared_ptr<XdmfArray> evaluateExpression(std::string expression,
                                                  std::map<std::string, shared_ptr<XdmfArray> > variables);

  /**
   * Evaluates the operation specified using the two shared pointers to XdmfArrays provided.
   * A list of valid operations is retrievable from the getSupportedOperations static method.
   * None of the XdmfArrays provided are modified during the evaluation process.
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline //#declareoperation
   * @until //#declareoperation
   * @skipline //#programstart
   * @until //#programstart
   * @skipline //#valueinit
   * @until //#valueinit
   * @skipline //#addOperation
   * @until //#addOperation
   * @skipline //#evaluateOperation
   * @until //#evaluateOperation
   * @skipline //#programend
   * @until //#programend
   * @skipline //#defineoperation
   * @until //#defineoperation
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline #//defineoperation
   * @until #//defineoperation
   * @skipline #//programstart
   * @until #//programstart
   * @skipline #//valueinit
   * @until #//valueinit
   * @skipline #//addOperation
   * @until #//addOperation
   * @skipline #//evaluateOperation
   * @until #//evaluateOperation
   *
   * @param	val1		the first array being evaluated
   * @param	val2		the second array being evaluated
   * @param	operation	a character specifying the operation performed
   * @return			a shared pointer to the Xdmf Array that results from the calculation
   */
  static shared_ptr<XdmfArray> evaluateOperation(shared_ptr<XdmfArray> val1,
                                                 shared_ptr<XdmfArray> val2,
                                                 char operation);

  /**
   * Evaluates the function specified using the vector of XdmfArrays provided.
   * None of the XdmfArrays provided are modified during the evaluation process.
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline //#declarefunction
   * @until //#declarefunction
   * @skipline //#programstart
   * @until //#programstart
   * @skipline //#valueinit
   * @until //#valueinit
   * @skipline //#addFunction
   * @until //#addFunction
   * @skipline //#evaluateFunction
   * @until //#evaluateFunction
   * @skipline //#programend
   * @until //#programend
   * @skipline //#definefunction
   * @until //#definefunction
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline #//definefunction
   * @until #//definefunction
   * @skipline #//programstart
   * @until #//programstart
   * @skipline #//valueinit
   * @until #//valueinit
   * @skipline #//addFunction
   * @until #//addFunction
   * @skipline #//evaluateFunction
   * @until #//evaluateFunction
   *
   * @param	valueVector	a vector containing the arrays to be used
   * @param	functionName	the string associated with the function being called
   * @return			the result of the function being called, a scalar will be returned as an XdmfArray with one value
   */
  static shared_ptr<XdmfArray> evaluateFunction(std::vector<shared_ptr<XdmfArray> > valueVector,
                                                std::string functionName);

  std::map<std::string, std::string> getItemProperties() const;

  virtual std::string getItemTag() const;

  /**
   * Gets the priority of operation whose associated character is provided. Returns -1 if the operation is not supported.
   * The higher the value the earlier that operation is evaluated during evaluateExpression.
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline //#getOperationPriority
   * @until //#getOperationPriority
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline #//getOperationPriority
   * @until #//getOperationPriority
   *
   * @param	operation	the character associated with the operation to be checked
   * @return			the priority of the operation
   */
  static int getOperationPriority(char operation);

  /**
   * Gets a string that contains all the characters of the supported operations.
   * Parenthesis are included for grouping purposes in expressions.
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline //#getSupportedOperations
   * @until //#getSupportedOperations
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline #//getSupportedOperations
   * @until #//getSupportedOperations
   *
   * @return	a string containing the characters for all supported operations
   */
  static const std::string getSupportedOperations();

  /**
   * Gets a string that contains all the characters of the supported operations.
   * Parenthesis are included for grouping purposes in expressions.
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline //#getSupportedFunctions
   * @until //#getSupportedFunctions
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline #//getSupportedFunctions
   * @until #//getSupportedFunctions
   *
   * @return	a vector containing the strings associated with all valid functions
   */
  static const std::vector<std::string> getSupportedFunctions();

  /**
   * Gets a string that contains all strings that are viable for use when mapping
   * to scalars (which are stored in XdmfArrays of size 1) for the evaluateExpression function.
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline //#getValidDigitChars
   * @until //#getValidDigitChars
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline #//getValidDigitChars
   * @until #//getValidDigitChars
   *
   * @return	a string containing all valid variable characters
   */
  static const std::string getValidDigitChars();

  /**
   * Gets a string that contains all strings that are viable for use when mapping
   * to shared pointers of XdmfArrays for the evaluateExpression function.
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline //#getValidVariableChars
   * @until //#getValidVariableChars
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline #//getValidVariableChars
   * @until #//getValidVariableChars
   *
   * @return	a string containing all valid variable characters
   */
  static const std::string getValidVariableChars();

  /**
   * Joins the two provided arrays while interspercing their values evenly.
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline //#valueinit
   * @until //#valueinit
   * @skipline //#interlace
   * @until //#interlace
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline #//valueinit
   * @until #//valueinit
   * @skipline #//interlace
   * @until #//interlace
   *
   * @param	val1	the first array being evaluated
   * @param	val2	the second array being evaluated
   * @return		the interlaced arrays
   */
  static shared_ptr<XdmfArray> interlace(shared_ptr<XdmfArray> val1,
                                         shared_ptr<XdmfArray> val2);

  /**
   * Adds together all the values contained in the provided arrays.
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline //#valueinit
   * @until //#valueinit
   * @skipline //#sum
   * @until //#sum
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline #//valueinit
   * @until #//valueinit
   * @skipline #//sum
   * @until #//sum
   *
   * @param	values	a vector containing the arrays to be used
   * @return		an XdmfArray containing one value which is the total of all the values contained within the provided arrays
   */
  static shared_ptr<XdmfArray> sum(std::vector<shared_ptr<XdmfArray> > values);

protected:

  XdmfFunction();

private:

  XdmfFunction(const XdmfFunction &);  // Not implemented.
  void operator=(const XdmfFunction &);  // Not implemented.


  static std::string mSupportedOperations;
  static const std::string mValidVariableChars;
  static const std::string mValidDigitChars;

  static std::map<char, int> mOperationPriority;
  static std::map<std::string, shared_ptr<XdmfArray>(*)(std::vector<shared_ptr<XdmfArray> >)> arrayFunctions;
  static std::map<char, shared_ptr<XdmfArray>(*)(shared_ptr<XdmfArray>, shared_ptr<XdmfArray>)> operations;

};

#endif /* XDMFFUNCTION_HPP_ */
