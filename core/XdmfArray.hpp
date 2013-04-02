/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfArray.hpp                                                       */
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

#ifndef XDMFARRAY_HPP_
#define XDMFARRAY_HPP_

// Forward Declarations
class XdmfArrayType;
class XdmfHeavyDataController;

// Includes
#include "XdmfCore.hpp"
#include "XdmfItem.hpp"
#include <boost/shared_array.hpp>
#include <boost/variant.hpp>

/**
 * @brief Provides storage for data values that are read in or will be
 * written to heavy data on disk.
 *
 * XdmfArray provides a single interface for storing a variety of data
 * types.  The data type stored is determined by the type initially
 * inserted into the array.  An array can be initialized with a
 * specific data type before insertion of values by calling
 * initialize().
 *
 * An XdmfArray is associated with heavy data files on disk through an
 * XdmfHeavyDataController. When an Xdmf file is read from disk,
 * XdmfHeavyDataControllers are attached to all created XdmfArrays
 * that contain values stored in heavy data. These values are not read
 * into memory when the Xdmf file is parsed. The array is
 * uninitialized and the return value of isInitialized() is false.  In
 * order to read the heavy data values into memory, read() must be
 * called. This will cause the array to ask for values to be read from
 * disk using the XdmfHeavyDataController. After the values have been
 * read from heavy data on disk, isInitialized() will return true.
 *
 * XdmfArray allows for insertion and retrieval of data in two
 * fundamental ways:
 *
 * By Copy:
 *
 *   getValue
 *   getValues
 *   insert
 *   pushBack
 *
 * XdmfArray stores its own copy of the data.  Modifications to the
 * data stored in the XdmfArray will not change values stored in the
 * original array.
 *
 * By Shared Reference:
 *
 *   getValuesInternal
 *   setValuesInternal
 *
 * XdmfArray shares a reference to the data.  No copy is
 * made. XdmfArray holds a shared pointer to the original data.
 * Modifications to the data stored in the XdmfArray also causes
 * modification to values stored in the original array.
 *
 * Xdmf supports the following datatypes:
 *   Int8
 *   Int16
 *   Int32
 *   Int64
 *   Float32
 *   Float64
 *   UInt8
 *   UInt16
 *   UInt32
 *   String
 */
class XDMFCORE_EXPORT XdmfArray : public XdmfItem {

public:

  /**
   * Create a new XdmfArray.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline New
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline New
   *
   * @return constructed XdmfArray.
   */
  static shared_ptr<XdmfArray> New();

  virtual ~XdmfArray();

  LOKI_DEFINE_VISITABLE(XdmfArray, XdmfItem);
  XDMF_CHILDREN(XdmfArray, XdmfHeavyDataController, HeavyDataController, Name);
  static const std::string ItemTag;

  /**
   * Remove all values from this array.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline clear
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline Assuming
   * @skipline clear
   */
  void clear();

  /**
   * Remove a value from this array.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skip exampleInternalPointerConst
   * @skipline //
   * @until ]
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skip exampleArray.insertAsFloat64(newIndex, newValue)
   * @skipline contains
   * @until ]
   */
  void erase(const unsigned int index);







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
   * @skipline maximum
   * @skipline main
   * @skipline addOperation
   * @skipline addFunction
   * @skipline valueMap
   * @until }
   * @skipline maximum
   * @until else
   * @skipline {
   * @until returnArray;
   * @skipline }
   * @until }
   * @skipline prepend
   * @until }
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline maximum
   * @until else
   * @skipline maxVal
   * @until pushBackAsFloat
   * @skipline return
   * @skipline prepend
   * @until val2.getSize
   * @skipline return
   * @skipline __main__
   * @skipline addOperation
   * @skipline addFunction
   * @skipline valueMap
   * @until evaluateExpression
   *
   * @param	expression	a string containing the expresion to be evaluated
   * @param	variables	a map of strings to their XdmfArray equivalent
   * @return			a shared pointer to the XdmfArray resulting from the expression
   */
  static shared_ptr<XdmfArray> evaluateExpression(std::string expression, std::map<std::string, shared_ptr<XdmfArray> > variables);

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
   * @skipline prepend
   * @skipline main
   * @skipline answerArray
   * @skipline addOperation
   * @until evaluateOperation
   * @skipline return
   * @until }
   * @skipline prepend
   * @until }
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline prepend
   * @until val2.getSize
   * @skipline return
   * @skipline __main__
   * @skipline addOperation
   * @until evaluateOperation
   *
   * @param	val1		the first array being evaluated
   * @param	val2		the second array being evaluated
   * @param	operation	a character specifying the operation performed
   * @return			a shared pointer to the Xdmf Array that results from the calculation
   */
  static shared_ptr<XdmfArray> evaluateOperation(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2, char operation);

  /**
   * Adds an operation to the list of viable operators.
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline prepend
   * @skipline main
   * @skipline addOperation
   * @skipline return
   * @until }
   * @skipline prepend
   * @until }
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline prepend
   * @until val2.getSize
   * @skipline return
   * @skipline __main__
   * @skipline addOperation
   *
   * @param	newoperator	the character to be associated with the provided binary operation
   * @param	functionref	a pointer to the function to be associated with the provided operator
   * @param	priority	used to determine order of operations the higher the value the earlier it is evaluated
   */
  static int addOperation(char newoperator, shared_ptr<XdmfArray>(*functionref)(shared_ptr<XdmfArray>, shared_ptr<XdmfArray>), int priority);

  /**
   * Joins the two provided arrays together end to end.
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline valueArray1
   * @until (6)
   * @skipline chunk
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline valueArray1
   * @until (6)
   * @skipline chunk
   *
   * @param     val1            the first array being evaluated
   * @param     val2            the second array being evaluated
   * @return			the arrays joined end to end
   */
  static shared_ptr<XdmfArray> chunk(shared_ptr<XdmfArray>, shared_ptr<XdmfArray>);

  /**
   * Joins the two provided arrays while interspercing their values evenly.
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline valueArray1
   * @until (6)
   * @skipline interlace
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline valueArray1
   * @until (6)
   * @skipline interlace
   *
   * @param     val1            the first array being evaluated
   * @param     val2            the second array being evaluated
   * @return                    the interlaced arrays
   */
  static shared_ptr<XdmfArray> interlace(shared_ptr<XdmfArray>, shared_ptr<XdmfArray>);

  /**
   * Evaluates the function specified using the vector of XdmfArrays provided.
   * None of the XdmfArrays provided are modified during the evaluation process.
   * 
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline maximum
   * @skipline main
   * @skipline addFunction
   * @until evaluateFunction
   * @skipline return
   * @until }
   * @skipline maximum
   * @until else
   * @skipline {
   * @until returnArray;
   * @skipline }
   * @until }
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline maximum
   * @until else
   * @skipline maxVal
   * @until pushBackAsFloat
   * @skipline return
   * @skipline __main__
   * @skipline addFunction
   * @until evaluateFunction
   *
   * @param	valueVector	a vector containing the arrays to be used
   * @param	functionName	the string associated with the function being called
   * @return			the result of the function being called, a scalar will be returned as an XdmfArray with one value
   */
  static shared_ptr<XdmfArray> evaluateFunction(std::vector<shared_ptr<XdmfArray> > valueVector, std::string functionName);

  /*
   * adds a specified function to the list of functions used while evaluating strings
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline maximum
   * @skipline main
   * @skipline addFunction
   * @skipline return
   * @until }
   * @skipline maximum
   * @until else
   * @skipline {
   * @until returnArray;
   * @skipline }
   * @until }
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline maximum
   * @until else
   * @skipline maxVal
   * @until pushBackAsFloat
   * @skipline return
   * @skipline __main__
   * @skipline addFunction
   *
   * @param	name		A string to be associated with the provided function during string evaluation
   * @param	functionref	A pointer to the function to be associated with the given string
   * @return			The total number of functions currently associated
   */
  static int addFunction(std::string name, shared_ptr<XdmfArray>(*functionref)(std::vector<shared_ptr<XdmfArray> >));

  /**
   * Adds together all the values contained in the provided arrays.
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline valueArray1
   * @until valueVector
   * @skipline valueArray1
   * @until valueArray2
   * @skipline sum
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline valueArray1
   * @until valueVector
   * @skipline valueArray1
   * @until valueArray2
   * @skipline sum
   *
   * @param	values	a vector containing the arrays to be used
   * @return		an XdmfArray containing one value which is the total of all the values contained within the provided arrays
   */
  static shared_ptr<XdmfArray> sum(std::vector<shared_ptr<XdmfArray> > values);

  /**
   * Averages the values contained in all the provided arrays.
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline valueArray1
   * @until valueVector
   * @skipline valueArray1
   * @until valueArray2
   * @skipline ave
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline valueArray1
   * @until valueVector
   * @skipline valueArray1
   * @until valueArray2
   * @skipline ave
   *
   * @param	values	a vector containing the arrays to be used
   * @return		an XdmfArray containing  one value which is the average of all values contained within the provided arrays
   */
  static shared_ptr<XdmfArray> ave(std::vector<shared_ptr<XdmfArray> > values);







  /**
   * Get the data type of this array.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline getArrayType
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline Assuming
   * @skipline getArrayType
   *
   * @return a XdmfArrayType containing the data type for the array.
   */
  shared_ptr<const XdmfArrayType> getArrayType() const;

  /**
   * Get the capacity of this array, the number of values the array
   * can store without reallocation.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline getCapacity
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline Assuming
   * @skipline getCapacity
   *
   * @return the capacity of this array.
   */
  unsigned int getCapacity() const;

  /**
   * Get the dimensions of the array.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline exampleDimensions
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline Assuming
   * @skipline exampleDimensions
   *
   * @return the dimensions of the array.
   */
  std::vector<unsigned int> getDimensions() const;

  /**
   * Get the dimensions of the array as a string.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline exampleDimensionString
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline Assuming
   * @skipline exampleDimensionString
   *
   * @return the dimensions of the array as a string.
   */
  std::string getDimensionsString() const;

  std::map<std::string, std::string> getItemProperties() const;

  std::string getItemTag() const;

  /**
   * Get the name of the array.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline exampleName
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline Assuming
   * @skipline exampleName
   *
   * @return a string containing the name of the array.
   */
  std::string getName() const;





  /**
   * Gets the priority of operation whose associated character is provided. Returns -1 if the operation is not supported.
   * The higher the value the earlier that operation is evaluated during evaluateExpression.
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline getOperationPriority
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline getOperationPriority
   *
   * @param	operation	the character associated with the operation to be checked
   * @return			the priority of the operation
   */
  static int getOperationPriority(char operation);







  /**
   * Get the number of values stored in this array.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline exampleSize
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline Assuming
   * @skipline exampleSize
   *
   * @return the number of values stored in this array.
   */
  unsigned int getSize() const;





  /**
   * Gets a string that contains all the characters of the supported operations.
   * Parenthesis are included for grouping purposes in expressions.
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline getSupportedOperations
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline getSupportedOperations
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
   * @skipline getSupportedFunctions
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline getSupportedFunctions
   *
   * @return    a vector containing the strings associated with all valid functions
   */
  static const std::vector<std::string> getSupportedFunctions();

  /**
   * Gets a string that contains all strings that are viable for use when mapping
   * to shared pointers of XdmfArrays for the evaluateExpression function.
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline getValidVariableChars
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline getValidVariableChars
   *
   * @return	a string containing all valid variable characters
   */
  static const std::string getValidVariableChars();

  /**
   * Gets a string that contains all strings that are viable for use when mapping
   * to scalars (which are stored in XdmfArrays of size 1) for the evaluateExpression function.
   *
   * Example of Use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline getValidDigitChars
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline getValidDigitChars
   *
   * @return    a string containing all valid variable characters
   */
  static const std::string getValidDigitChars();





  /**
   * Get a copy of a single value stored in this array.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skip XdmfHeavyDataController
   * @skipline if
   * @until index
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline Assuming
   * @skip [0,
   * @skipline 7]
   * @until Variations
   *
   * @return the requested value.
   */
  template <typename T>
  T getValue(const unsigned int index) const;

  /**
   * Get a copy of the values stored in this array
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline New
   * @skipline initArray
   * @skipline exampleArray
   * @until {0,2,4,6,8,5,3,7,4,9}
   *
   * Python: This function is not supported in Python, it is replaced by the getNumpyArray function
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline New
   * @skipline initArray
   * @skipline exampleArray
   * @skipline getNumpyArray	
   *
   * @param startIndex the index in this array to begin copying from.
   * @param valuesPointer a pointer to an array to copy into.
   * @param numValues the number of values to copy.
   * @param arrayStride number of values to stride in this array between each
   * copy.
   * @param valuesStride number of values to stride in the pointer between each
   * copy.
   */
  template <typename T> void
  getValues(const unsigned int startIndex,
            T * const valuesPointer,
            const unsigned int numValues = 1,
            const unsigned int arrayStride = 1,
            const unsigned int valuesStride = 1) const;

  /**
   * Get a smart pointer to the internal values stored in this array.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skip getValuesString
   * @skipline //
   * @until exampleInternalVector
   *
   * Python:
   * Python does not support this version of the getValuesInternal function, it defaults to the version that returns a void pointer
   *
   * @return a smart pointer to the internal vector of values stored
   * in this array.
   */
  template <typename T>
  shared_ptr<std::vector<T> > getValuesInternal();

  /**
   * Get a pointer to the internal values stored in this array.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline exampleInternalPointer
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline Assuming
   * @skipline getValuesInternal
   * @until used
   *
   * @return a void pointer to the first value stored in this array.
   */
  void * getValuesInternal();

  /**
   * Get a pointer to the internal values stored in this array (const
   * version).
   *
   * Example of use:
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline exampleInternalPointerConst
   *
   * Python:
   * Python does not support this version of the getValuesInternal function, it defaults to the version that returns a void pointer
   *
   * @return a void pointer to the first value stored in this array.
   */
  const void * getValuesInternal() const;

  /**
   * Get the values stored in this array as a string.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline getValuesString
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline Assuming
   * @skipline exampleValueString
   * @until contained
   *
   * @return a string containing the contents of the array.
   */
  std::string getValuesString() const;

  /**
   * Initialize the array to a specific size.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline newSize
   * @skipline exampleVector
   *
   * Python: Does not support this version of initialize
   *
   * @param size the number of values in the initialized array.
   *
   * @return a smart pointer to the internal vector of values
   * initialized in this array.
   */
  template <typename T>
  shared_ptr<std::vector<T> > initialize(const unsigned int size = 0);

  /**
   * Initialize the array to specific dimensions.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline newSizeVector
   * @until exampleVector
   *
   * Python: Does not support this version of initialize
   *
   * @param dimensions the dimensions of the initialized array.
   *
   * @return a smart pointer to the internal vector of values
   * initialized in this array.
   */
  template <typename T>
  shared_ptr<std::vector<T> >
  initialize(const std::vector<unsigned int> & dimensions);

  /**
   * Initialize the array to contain a specified amount of a particular type.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline newSize
   * @skipline XdmfArrayType
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline Assuming
   * @skipline newSize
   * @skipline XdmfArrayType
   *
   * @param arrayType the type of array to initialize.
   * @param size the number of values in the initialized array.
   */
  void initialize(const shared_ptr<const XdmfArrayType> arrayType,
                  const unsigned int size = 0);

  /**
   * Initialize the array with specified dimensions to contain a particular type.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline newSizeVector
   * @until 5
   * @skipline XdmfArrayType
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline Assuming
   * @skipline UInt32Vector
   * @until initialize
   *
   * @param arrayType the type of array to initialize.
   * @param dimensions the number dimensions of the initialized array.
   */
  void initialize(const shared_ptr<const XdmfArrayType> arrayType,
                  const std::vector<unsigned int> & dimensions);

  using XdmfItem::insert;

  /**
   * Insert value into this array
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline New
   * @skipline newIndex
   * @until newValue
   * @skipline insert
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline New
   * @skipline newIndex
   * @until insertAsInt32
   *
   * @param index the index in this array to insert.
   * @param value the value to insert
   */
  template<typename T>
  void insert(const unsigned int index,
              const T & value);

  /**
   * Insert values from an XdmfArray into this array.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline New
   * @skipline initArray
   * @until insert
   * @skipline tempArray
   * @until {0,2,4,6,8,5,3,7,4,9}
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline New
   * @skipline initArray
   * @until {0,2,4,6,8,5,3,7,4,9}
   *
   * @param startIndex the index in this array to begin insertion.
   * @param values a shared pointer to an XdmfArray to copy into this array.
   * @param valuesStartIndex the index in the XdmfArray to begin copying.
   * @param numValues the number of values to copy into this array.
   * @param arrayStride number of values to stride in this array between each
   * copy.
   * @param valuesStride number of values to stride in the XdmfArray between
   * each copy.
   */
  void insert(const unsigned int startIndex,
              const shared_ptr<const XdmfArray> values,
              const unsigned int valuesStartIndex = 0,
              const unsigned int numValues = 1,
              const unsigned int arrayStride = 1,
              const unsigned int valuesStride = 1);

  /**
   * Insert values into this array.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline New
   * @skipline initArray
   * @skipline insert
   * @until {0,2,4,6,8,5,3,7,4,9}
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline New
   * @skipline initArray
   * @skipline insertAsInt32
   * @until Sublists
   *
   * @param startIndex the index in this array to begin insertion.
   * @param valuesPointer a pointer to the values to copy into this array.
   * @param numValues the number of values to copy into this array.
   * @param arrayStride number of values to stride in this array between each
   * copy.
   * @param valuesStride number of values to stride in the pointer between
   * each copy.
   */
  template<typename T>
  void insert(const unsigned int startIndex,
              const T * const valuesPointer,
              const unsigned int numValues = 1,
              const unsigned int arrayStride = 1,
              const unsigned int valuesStride = 1);

  /**
   * Returns whether the array is initialized (contains values in
   * memory).
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline isInitialized
   * @until }
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline Assuming
   * @skipline isInitialized
   * @until read
   */
  bool isInitialized() const;

  /**
   * Copy a value to the back of this array
   *
   * Example of use;
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline New
   * @skipline newValue
   * @skipline pushBack
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline New
   * @skipline newValue
   * @until pushBackAsFloat64
   */
  template <typename T>
  void pushBack(const T & value);

  /**
   * Get the first heavy data controller attached to this array.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline exampleController
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline Assuming
   * @skipline exampleController
   *
   * @return the heavy data controller attached to this array.
   */
  shared_ptr<XdmfHeavyDataController>
  getHeavyDataController();

  /**
   * Get the first heavy data controller attached to this array. (const version)
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline exampleControllerConst
   *
   * Python: Doesn't support a constant version of this function
   *
   * @return the heavy data controller attached to this array.
   */
  shared_ptr<const XdmfHeavyDataController>
  getHeavyDataController() const;

  /**
   * Replace all controllers attached to this array with the controller provided.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline exampleController
   * @until setHeaveyDataController
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline Assuming
   * @skipline exampleController
   * @until setHeaveyDataController
   *
   * @param newController the heavy data controller to attach to this array.
   */
  void
  setHeavyDataController(shared_ptr<XdmfHeavyDataController> newController);

  /**
   * Read data from disk into memory.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline !exampleArray
   * @until }
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline Assuming
   * @skipline isInitialized
   * @until read
   */
  void read();

  /**
   * Release all data currently held in memory.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline Assuming
   * @skipline release
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline Assuming
   * @skipline release
   */
  void release();

  /**
   * Set the capacity of the array to at least size.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline New
   * @skipline newSize
   * @skipline reserve
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline New
   * @skipline newSize
   * @skipline reserve
   *
   * @param size the capacity to set this array to.
   */
  void reserve(const unsigned int size);

  /**
   * Resizes the array to contain a number of values. If numValues is
   * larger than the current size, values are appended to the end of
   * the array equal to value. If numValues is less than the current
   * size, values at indices larger than numValues are removed.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline New
   * @skipline newSize
   * @skipline initArray
   * @skipline insert 
   * @until //
   * @skipline newSize
   * @until 4}
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline New
   * @skipline initArray
   * @skipline insert
   * @until #
   * @skipline newSize
   * @until resizeAsFloat64
   *
   * @param numValues the number of values to resize this array to.
   * @param value the number to initialize newly created values to, if needed.
   */
  template<typename T>
  void resize(const unsigned int numValues,
              const T & value = 0);

  /**
   * Resizes the array to specified dimensions. If the number of
   * values specified by the dimensions is larger than the current
   * size, values are appended to the end of the array equal to
   * value. If numValues is less than the current size, values at
   * indices larger than numValues are removed.
   *
   * Example of use:
   * 
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline New
   * @skipline newSizeVector
   * @until push_back(5)
   * @skipline initArray
   * @skipline insert
   * @until //
   * @skip resize
   * @skipline newSizeVector
   * @until 4}
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline New
   * @skipline initArray
   * @skipline insertAsInt32
   * @until #
   * @skipline newSizeArray
   * @until resizeAsFloat64
   *
   * @param dimensions the dimensions to resize the array to.
   * @param value the number to intialize newly created values to, if needed.
   */
  template<typename T>
  void resize(const std::vector<unsigned int> & dimensions,
              const T & value = 0);

  /**
   * Set the name of the array.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline New
   * @skipline newName
   * @until setName
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline New
   * @skipline newName
   * @until setName
   *
   * @param name of the array to set.
   */
  void setName(const std::string & name);

  /**
   * Sets the values of this array to the values stored in the
   * arrayPointer array. No copy is made. Modifications to the array
   * are not permitted through the XdmfArray API. Any calls through
   * the XdmfArray API to modify the array (i.e. any non-const
   * function) will result in the array being copied into internal
   * storage. The internal copy is then modified.  This prevents
   * situations where a realloc of the pointer could cause other
   * references to become invalid. The caller of this method can
   * continue to modify the values stored in arrayPointer on its
   * own. This function is meant for applications that have their own
   * array data structures that merely use Xdmf to output the data, an
   * operation that should not require a copy. Other applications that
   * use Xdmf for in memory data storage should avoid this function.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline initArray
   * @skipline setValuesInternal
   *
   * Python: does not support setValuesInternal
   *
   * @param arrayPointer a pointer to an array to store in this XdmfArray.
   * @param numValues the number of values in the array.
   * @param transferOwnership whether to transfer responsibility for deletion
   * of the array to XdmfArray.
   */
  template<typename T>
  void setValuesInternal(const T * const arrayPointer,
                         const unsigned int numValues,
                         const bool transferOwnership = 0);

  /**
   * Sets the values of this array to the values stored in the
   * vector. No copy is made. The caller of this method retains
   * ownership of the data and must ensure that the array is still
   * valid for the entire time Xdmf needs it.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline New
   * @skipline initVector
   * @skipline setValuesInternal
   *
   * Python: does not support setValuesInternal
   *
   * @param array a vector to store in this XdmfArray.
   * @param transferOwnership whether to transfer responsibility for deletion
   * of the array to XdmfArray.
   */
  template<typename T>
  void setValuesInternal(std::vector<T> & array,
                         const bool transferOwnership = 0);

  /**
   * Sets the values of this array to the values stored in the
   * vector. No copy is made. This array shares ownership with other
   * references to the smart pointer.
   *
   * Example of use:
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline New
   * @skipline initVector
   * @until push_back(5)
   * @skipline storeVector
   * @until setValuesInternal
   *
   * Python: does not support setValuesInternal
   *
   * @param array a smart pointer to a vector to store in this array.
   */
  template<typename T>
  void setValuesInternal(const shared_ptr<std::vector<T> > array);

  /**
   * Exchange the contents of the vector with the contents of this
   * array. No copy is made. The internal arrays are swapped.
   *
   * Example of use
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline New
   * @skipline initArray
   * @skipline insert
   * @skipline initVector
   * @until push_back(5)
   * @skipline //
   * @until //
   *
   * Python: The Python version only supports swapping XdmfArrays
   *
   * @param array a vector to exchange values with.
   * @return bool whether the swap was successful.
   */
  template<typename T>
  bool swap(std::vector<T> & array);

  /**
   * Exchange the contents of the vector with the contents of this
   * array. No copy is made. The internal arrays are swapped.
   *
   * Example of use
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline New
   * @skipline initArray
   * @skipline insert
   * @skipline initVector
   * @until push_back(5)
   * @skipline //
   * @skipline storeSwapSucceded
   * @until //
   *
   * Python: The Python version only supports swapping XdmfArrays
   *
   * @param array a smart pointer to a vector to exchange values with.
   * @return bool whether the swap was successful.
   */
  template<typename T>
  bool swap(const shared_ptr<std::vector<T> > array);

  /**
   * Exchange the contents of an XdmfArray with the contents of this
   * array. No copy is made. The internal arrays are swapped.
   *
   * Example of use
   *
   * C++
   *
   * @dontinclude ExampleXdmfArray.cpp
   * @skipline New
   * @skipline initArray
   * @skipline insert
   * @skipline swapArray
   * @until exampleArray
   * @skipline //
   *
   * Python
   *
   * @dontinclude XdmfExampleArray.py
   * @skipline New
   * @skipline initArray
   * @skipline insert
   * @skipline swapArray
   * @until exampleArray
   * @skipline #
   *
   * @param array a smart pointer to a vector to exchange values with.
   */
  void swap(const shared_ptr<XdmfArray> array);

protected:

  XdmfArray();

  virtual void
  populateItem(const std::map<std::string, std::string> & itemProperties,
               const std::vector<shared_ptr<XdmfItem> > & childItems,
               const XdmfCoreReader * const reader);

private:

  XdmfArray(const XdmfArray &);  // Not implemented.
  void operator=(const XdmfArray &);  // Not implemented.

  // Variant Visitor Operations
  class Clear;
  class Erase;
  class GetArrayType;
  class GetCapacity;
  template <typename T> class GetValue;
  template <typename T> class GetValues;
  class GetValuesPointer;
  class GetValuesString;
  template <typename T> class Insert;
  class InsertArray;
  class InternalizeArrayPointer;
  struct NullDeleter;
  template <typename T> class PushBack;
  class Reserve;
  template <typename T> class Resize;
  class Size;

  /**
   * After setValues() is called, XdmfArray stores a pointer that is
   * not allowed to be modified through the XdmfArray API. If the user
   * desires to modify the contents of the pointer, they must do so
   * without calling any non-const functions of XdmfArray. If they do
   * call non-const functions of XdmfArray, we attempt to accommodate
   * by copying the array pointer into internal data structures.
   */
  void internalizeArrayPointer();

  typedef boost::variant<
    boost::blank,
    shared_ptr<std::vector<char> >,
    shared_ptr<std::vector<short> >,
    shared_ptr<std::vector<int> >,
    shared_ptr<std::vector<long> >,
    shared_ptr<std::vector<float> >,
    shared_ptr<std::vector<double> >,
    shared_ptr<std::vector<unsigned char> >,
    shared_ptr<std::vector<unsigned short> >,
    shared_ptr<std::vector<unsigned int> >,
    shared_ptr<std::vector<std::string> >,
    boost::shared_array<const char>,
    boost::shared_array<const short>,
    boost::shared_array<const int>,
    boost::shared_array<const long>,
    boost::shared_array<const float>,
    boost::shared_array<const double>,
    boost::shared_array<const unsigned char>,
    boost::shared_array<const unsigned short>,
    boost::shared_array<const unsigned int>  > ArrayVariant;
  
  ArrayVariant mArray;
  unsigned int mArrayPointerNumValues;
  std::vector<unsigned int> mDimensions;
  std::string mName;
  unsigned int mTmpReserveSize;

  static std::string mSupportedOperations;
  static const std::string mValidVariableChars;
  static const std::string mValidDigitChars;
  static int mOperationPriority[];

  static std::map<std::string, shared_ptr<XdmfArray>(*)(std::vector<shared_ptr<XdmfArray> >)> arrayFunctions;
  static std::map<char, shared_ptr<XdmfArray>(*)(shared_ptr<XdmfArray>, shared_ptr<XdmfArray>)> operations;
};

#include "XdmfArray.tpp"

#ifdef _WIN32
XDMFCORE_TEMPLATE template class XDMFCORE_EXPORT
shared_ptr<const XdmfArrayType>;
XDMFCORE_TEMPLATE template class XDMFCORE_EXPORT
shared_ptr<XdmfHeavyDataController>;
XDMFCORE_TEMPLATE template class XDMFCORE_EXPORT
shared_ptr<const XdmfHeavyDataController>;
XDMFCORE_TEMPLATE template class XDMFCORE_EXPORT
shared_ptr<Loki::BaseVisitor>;
XDMFCORE_TEMPLATE template class XDMFCORE_EXPORT
Loki::Visitor<shared_ptr<XdmfArray>, shared_ptr<XdmfItem> >;
#endif

#endif /* XDMFARRAY_HPP_ */
