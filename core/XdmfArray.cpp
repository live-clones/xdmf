/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfArray.cpp                                                       */
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

#include <boost/tokenizer.hpp>
#include <boost/assign.hpp>
#include <limits>
#include <sstream>
#include <utility>
#include <stack>
#include <math.h>
#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfHeavyDataController.hpp"
#include "XdmfVisitor.hpp"
#include "XdmfError.hpp"

std::string XdmfArray::mSupportedOperations = "|#()";
const std::string XdmfArray::mValidVariableChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_:.";
const std::string XdmfArray::mValidDigitChars = "-1234567890.";
//list the priorities for the operations, based on the order of operations
//the index of the corresponding operation in validOperationChars is the same as the index of its priority in this array
int XdmfArray::mOperationPriority [8] = {2, 1, 0, 0};
//the higher the value, the earlier the operation is evaluated in the order of operations
//with the exception of parenthesis which are evaluated as soon as the closing parenthesis is found

//note, it doesn't handle overloaded functions well. Will generate errors unless overload methods are typecast.
std::map<std::string, shared_ptr<XdmfArray> (*)(std::vector<shared_ptr<XdmfArray> >)> XdmfArray::arrayFunctions = boost::assign::map_list_of ("SUM", (shared_ptr<XdmfArray> (*)(std::vector<shared_ptr<XdmfArray> >))XdmfArray::sum) ("AVE", (shared_ptr<XdmfArray> (*)(std::vector<shared_ptr<XdmfArray> >))XdmfArray::ave);
std::map<char, shared_ptr<XdmfArray> (*)(shared_ptr<XdmfArray>, shared_ptr<XdmfArray>)> XdmfArray::operations = boost::assign::map_list_of ('|', XdmfArray::chunk) ('#', XdmfArray::interlace);



XDMF_CHILDREN_IMPLEMENTATION(XdmfArray,
                             XdmfHeavyDataController,
                             HeavyDataController,
                             Name)

class XdmfArray::Clear : public boost::static_visitor<void> {
public:

  Clear(XdmfArray * const array) :
    mArray(array)
  {
  }

  void
  operator()(const boost::blank & array) const
  {
    return;
  }

  template<typename T>
  void
  operator()(const shared_ptr<std::vector<T> > & array) const
  {
    array->clear();
  }

  template<typename T>
  void
  operator()(const boost::shared_array<const T> & array) const
  {
    mArray->internalizeArrayPointer();
    boost::apply_visitor(*this,
                         mArray->mArray);
  }

private: 
  XdmfArray * const mArray;
};

class XdmfArray::Erase : public boost::static_visitor<void> {
public:

  Erase(XdmfArray * const array,
        const unsigned int index) :
    mArray(array),
    mIndex(index)
  {
  }

  void
  operator()(const boost::blank & array) const
  {
    return;
  }

  template<typename T>
  void
  operator()(const shared_ptr<std::vector<T> > & array) const
  {
    array->erase(array->begin() + mIndex);
  }

  template<typename T>
  void
  operator()(const boost::shared_array<const T> & array) const
  {
    mArray->internalizeArrayPointer();
    boost::apply_visitor(*this,
                         mArray->mArray);
  }

private:

  XdmfArray * const mArray;
  const unsigned int mIndex;
};

class XdmfArray::GetArrayType :
  public boost::static_visitor<shared_ptr<const XdmfArrayType> > {
public:

  GetArrayType(const shared_ptr<XdmfHeavyDataController> & heavyDataController) :
    mHeavyDataController(heavyDataController)
  {
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const char * const) const
  {
    return XdmfArrayType::Int8();
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const short * const) const
  {
    return XdmfArrayType::Int16();
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const int * const) const
  {
    return XdmfArrayType::Int32();
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const long * const) const
  {
    return XdmfArrayType::Int64();
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const float * const) const
  {
    return XdmfArrayType::Float32();
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const double * const) const
  {
    return XdmfArrayType::Float64();
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const unsigned char * const) const
  {
    return XdmfArrayType::UInt8();
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const unsigned short * const) const
  {
    return XdmfArrayType::UInt16();
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const unsigned int * const) const
  {
    return XdmfArrayType::UInt32();
  }

  shared_ptr<const XdmfArrayType>
  getArrayType(const std::string * const) const
  {
    return XdmfArrayType::String();
  }

  shared_ptr<const XdmfArrayType>
  operator()(const boost::blank & array) const
  {
    if(mHeavyDataController) {
      return mHeavyDataController->getType();
    }
    return XdmfArrayType::Uninitialized();
  }

  template<typename T>
  shared_ptr<const XdmfArrayType>
  operator()(const shared_ptr<std::vector<T> > & array) const
  {
    return this->getArrayType(&(array.get()->operator[](0)));
  }

  template<typename T>
  shared_ptr<const XdmfArrayType>
  operator()(const boost::shared_array<const T> & array) const
  {
    return this->getArrayType(array.get());
  }

private:

  const shared_ptr<XdmfHeavyDataController> mHeavyDataController;
};

class XdmfArray::GetCapacity : public boost::static_visitor<unsigned int> {
public:

  GetCapacity()
  {
  }

  unsigned int
  operator()(const boost::blank & array) const
  {
    return 0;
  }

  template<typename T>
  unsigned int
  operator()(const shared_ptr<std::vector<T> > & array) const
  {
    return array->capacity();
  }

  template<typename T>
  unsigned int
  operator()(const boost::shared_array<const T> & array) const
  {
    return 0;
  }
};

class XdmfArray::GetValuesPointer :
  public boost::static_visitor<const void *> {
public:

  GetValuesPointer()
  {
  }

  const void *
  operator()(const boost::blank & array) const
  {
    return NULL;
  }

  template<typename T>
  const void *
  operator()(const shared_ptr<std::vector<T> > & array) const
  {
    return &array->operator[](0);
  }

  template<typename T>
  const void *
  operator()(const boost::shared_array<const T> & array) const
  {
    return array.get();
  }
};

class XdmfArray::GetValuesString : public boost::static_visitor<std::string> {
public:

  GetValuesString(const int arrayPointerNumValues) :
    mArrayPointerNumValues(arrayPointerNumValues)
  {
  }

  template<typename T, typename U>
  std::string
  getValuesString(const T * const array,
                  const int numValues) const
  {
    const int lastIndex = numValues - 1;

    if(lastIndex < 0) {
      return "";
    }

    std::stringstream toReturn;
    toReturn.precision(std::numeric_limits<U>::digits10 + 2);
    for(int i=0; i<lastIndex; ++i) {
      toReturn << (U)array[i] << " ";
    }
    toReturn << (U)array[lastIndex];
    return toReturn.str();
  }

  std::string
  getValuesString(const char * const array,
                  const int numValues) const
  {
    return getValuesString<char, int>(array, numValues);
  }

  std::string
  getValuesString(const unsigned char * const array,
                  const int numValues) const
  {
    return getValuesString<unsigned char, int>(array, numValues);
  }

  template<typename T>
  std::string
  getValuesString(const T * const array,
                  const int numValues) const
  {
    return getValuesString<T, T>(array, numValues);
  }

  std::string
  operator()(const boost::blank & array) const
  {
    return "";
  }

  template<typename T>
  std::string
  operator()(const shared_ptr<std::vector<T> > & array) const
  {
    return getValuesString(&(array->operator[](0)), array->size());
  }

  template<typename T>
  std::string
  operator()(const boost::shared_array<const T> & array) const
  {
    return getValuesString(array.get(), mArrayPointerNumValues);
  }

private:

  const unsigned int mArrayPointerNumValues;
};

class XdmfArray::InsertArray : public boost::static_visitor<void> {
public:

  InsertArray(XdmfArray * const array,
              const unsigned int startIndex,
              const unsigned int valuesStartIndex,
              const unsigned int numValues,
              const unsigned int arrayStride,
              const unsigned int valuesStride,
              std::vector<unsigned int> & dimensions,
              const shared_ptr<const XdmfArray> & arrayToCopy) :
    mArray(array),
    mStartIndex(startIndex),
    mValuesStartIndex(valuesStartIndex),
    mNumValues(numValues),
    mArrayStride(arrayStride),
    mValuesStride(valuesStride),
    mDimensions(dimensions),
    mArrayToCopy(arrayToCopy)
  {
  }

  void
  operator()(const boost::blank & array) const
  {
    mArray->initialize(mArrayToCopy->getArrayType());
    boost::apply_visitor(*this,
                         mArray->mArray);
  }

  template<typename T>
  void
  operator()(const shared_ptr<std::vector<T> > & array) const
  {
    unsigned int size = mStartIndex + mNumValues;
    if(mArrayStride > 1) {
      size = mStartIndex + mNumValues * mArrayStride - (mStartIndex%mArrayStride);
    }
    if(array->size() < size) {
      array->resize(size);
      mDimensions.clear();
    }
    mArrayToCopy->getValues(mValuesStartIndex,
                            &(array->operator[](mStartIndex)),
                            mNumValues,
                            mValuesStride,
                            mArrayStride);
  }

  template<typename T>
  void
  operator()(const boost::shared_array<const T> & array) const
  {
    mArray->internalizeArrayPointer();
    boost::apply_visitor(*this,
                         mArray->mArray);
  }

private:

  XdmfArray * const mArray;
  const unsigned int mStartIndex;
  const unsigned int mValuesStartIndex;
  const unsigned int mNumValues;
  const unsigned int mArrayStride;
  const unsigned int mValuesStride;
  std::vector<unsigned int> & mDimensions;
  const shared_ptr<const XdmfArray> mArrayToCopy;
};

class XdmfArray::InternalizeArrayPointer : public boost::static_visitor<void> {
public:

  InternalizeArrayPointer(XdmfArray * const array) :
    mArray(array)
  {
  }

  void
  operator()(const boost::blank & array) const
  {
    return;
  }

  template<typename T>
  void
  operator()(const shared_ptr<std::vector<T> > & array) const
  {
    return;
  }

  template<typename T>
  void
  operator()(const boost::shared_array<const T> & array) const
  {
    const T * const pointer = array.get();
    shared_ptr<std::vector<T> > newArray(new std::vector<T>(pointer,
                                                            pointer + mArray->mArrayPointerNumValues));
    mArray->mArray = newArray;
    mArray->mArrayPointerNumValues = 0;
  }

private:

  XdmfArray * const mArray;
};

class XdmfArray::Reserve : public boost::static_visitor<void> {
public:

  Reserve(XdmfArray * const array,
          const unsigned int size):
    mArray(array),
    mSize(size)
  {
  }

  void
  operator()(const boost::blank & array) const
  {
    mArray->mTmpReserveSize = mSize;
  }

  template<typename T>
  void
  operator()(shared_ptr<std::vector<T> > & array) const
  {
    array->reserve(mSize);
  }

  template<typename T>
  void
  operator()(const boost::shared_array<const T> & array) const
  {
    mArray->internalizeArrayPointer();
    boost::apply_visitor(*this,
                         mArray->mArray);
  }

private:

  XdmfArray * const mArray;
  const unsigned int mSize;
};

class XdmfArray::Size : public boost::static_visitor<unsigned int> {
public:

  Size(const XdmfArray * const array) :
    mArray(array)
  {
  }

  unsigned int
  operator()(const boost::blank & array) const
  {
    if(mArray->mHeavyDataControllers.size()>0) {
	int total = 0;
	for (int i = 0; i < mArray->mHeavyDataControllers.size(); i++)
	{
		total += mArray->mHeavyDataControllers[i]->getSize();
	}
      return total;//modify this to compile all controllers
    }
    return 0;
  }

  template<typename T>
  unsigned int
  operator()(const shared_ptr<std::vector<T> > & array) const
  {
    return array->size();
  }

  template<typename T>
  unsigned int
  operator()(const boost::shared_array<const T> & array) const
  {
    return mArray->mArrayPointerNumValues;
  }

private:

  const XdmfArray * const mArray; 
};

shared_ptr<XdmfArray>
XdmfArray::New()
{
  shared_ptr<XdmfArray> p(new XdmfArray());
  return p;
}

XdmfArray::XdmfArray() :
  mArrayPointerNumValues(0),
  mName(""),
  mTmpReserveSize(0)
{
}

XdmfArray::~XdmfArray()
{
}

const std::string XdmfArray::ItemTag = "DataItem";

void
XdmfArray::clear()
{
  boost::apply_visitor(Clear(this), 
                       mArray);
  mDimensions.clear();
}

void
XdmfArray::erase(const unsigned int index)
{
  boost::apply_visitor(Erase(this,
                             index),
                       mArray);
  mDimensions.clear();
}

//This function and the functions it depends on are reimplemented in XdmfCore.i in order to properly interface with python.
//There are differences between the versions, but the overall algorithm remains mostly the same.
shared_ptr<XdmfArray>
XdmfArray::evaluateExpression(std::string expression, std::map<std::string, shared_ptr<XdmfArray> > variables)
{
	std::stack<shared_ptr<XdmfArray> > valueStack;
	std::stack<char> operationStack;

	//string is parsed left to right
	//elements of the same priority are evaluated right to left
	for (int i = 0; i < expression.size(); i++)
	{
		if (mValidDigitChars.find(expression[i]) != std::string::npos)//found to be a digit
		{
			//progress until a non-digit is found
			int valueStart = i;
			while (mValidDigitChars.find(expression[i + 1]) != std::string::npos)
			{
				i++;
			}
			//push back to the value stack
			shared_ptr<XdmfArray> valueArray = XdmfArray::New();
			valueArray->insert(0, atof(expression.substr(valueStart, i + 1 - valueStart).c_str()));//use this to convert to double
			valueStack.push(valueArray);
		}
		else if (mValidVariableChars.find(expression[i]) != std::string::npos)//found to be a variable
		{
			int valueStart = i;
			//progress until a nonvariable value is found
			while (mValidVariableChars.find(expression[i + 1]) != std::string::npos)
			{
				i++;
			}
			//convert to equivalent
			if (variables.find(expression.substr(valueStart, i + 1 - valueStart)) == variables.end())
			{
				if (arrayFunctions.find(expression.substr(valueStart, i + 1 - valueStart)) == arrayFunctions.end())
				{
					XdmfError::message(XdmfError::FATAL,
						"Error: Invalid Variable in evaluateExpression " + expression.substr(valueStart, i + 1 - valueStart));
				}
				else
				{
					std::string currentFunction = expression.substr(valueStart, i + 1 - valueStart);
					//check if next character is an open parenthesis
					if (expression[i+1] != '(')
					{
						XdmfError::message(XdmfError::FATAL,
							"Error: No values supplied to function " + expression.substr(valueStart, i + 1 - valueStart));
					}
					//if it is grab the string between paranthesis
					i = i + 2;
					valueStart = i;
					int numOpenParenthesis = 0;
					while ((expression[i] != ')' || numOpenParenthesis) && i < expression.size())
					{
						if (expression[i] == '(')
						{
							numOpenParenthesis++;
						}
						else if (expression[i] == ')')
						{
							numOpenParenthesis--;
						}
						i++;
					}
					std::string functionParameters = expression.substr(valueStart, i - valueStart);
					std::vector<shared_ptr<XdmfArray> > parameterVector;
					//split that string at commas
					size_t parameterSplit = 0;
					while (parameterSplit != std::string::npos)
					{
						parameterSplit = 0;
						parameterSplit = functionParameters.find_first_of(",", parameterSplit);
						//feed the substrings to the parse function
						if (parameterSplit == std::string::npos)
						{
							parameterVector.push_back(evaluateExpression(functionParameters, variables));
						}
						else
						{
							parameterVector.push_back(evaluateExpression(functionParameters.substr(0, parameterSplit), variables));
							functionParameters = functionParameters.substr(parameterSplit+1);
						}
					}
					valueStack.push(evaluateFunction(parameterVector, currentFunction));
				}
			}
			else
			{
				//push equivalent to value stack
				valueStack.push(variables.find(expression.substr(valueStart, i + 1 - valueStart))->second);
			}
		}
		else if (mSupportedOperations.find(expression[i]) != std::string::npos)//found to be an operation
		{
			//pop operations off the stack until one of a lower or equal importance is found
			if (operationStack.size() > 0)
			{
				if (expression[i] == ')')
				{
					//to close a parenthesis pop off all operations until another parentheis is found
					while (operationStack.size() > 0 && operationStack.top() != '(')
					{
						if (valueStack.size() < 2)//must be at least two values for this loop to work properly
						{
							XdmfError::message(XdmfError::FATAL,
								"Error: Not Enough Values in evaluateExpression");
						}
						else
						{
							shared_ptr<XdmfArray> val2 = valueStack.top();
							valueStack.pop();
							shared_ptr<XdmfArray> val1 = valueStack.top();
							valueStack.pop();
							valueStack.push(evaluateOperation(val1, val2, operationStack.top()));
							operationStack.pop();
						}
					}
					operationStack.pop();
				}
				else if (expression[i] == '(')
				{
					//just add it if it's a start parenthesis
					//nothing happens here in that case
					//addition happens after the if statement
				}
				else
				{
					int operationLocation = getOperationPriority(expression[i]);
					int topOperationLocation = getOperationPriority(operationStack.top());
					//see order of operations to determine importance
					while (operationStack.size() > 0 && operationLocation < topOperationLocation)
					{
						if (valueStack.size() < 2)//must be at least two values for this loop to work properly
						{
							XdmfError::message(XdmfError::FATAL,
								"Error: Not Enough Values in evaluateExpression");
						}
						else
						{
							shared_ptr<XdmfArray> val2 = valueStack.top();
							valueStack.pop();
							shared_ptr<XdmfArray> val1 = valueStack.top();
							valueStack.pop();
							valueStack.push(evaluateOperation(val1, val2, operationStack.top()));
							operationStack.pop();
							if (operationStack.size() == 0)
							{
								break;
							}
							topOperationLocation = getOperationPriority(operationStack.top());
						}
					}
				}
			}
			if (expression[i] != ')')
			{
				//add the operation to the operation stack
				operationStack.push(expression[i]);
			}
		}
		//if not a value or operation the character is ignored
	}


	//empty what's left in the stacks before finishing
	while (valueStack.size() > 1 && operationStack.size() > 0)
	{
		if (valueStack.size() < 2)//must be at least two values for this loop to work properly
		{
			XdmfError::message(XdmfError::FATAL,
				"Error: Not Enough Values in evaluateExpression");
		}
		else
		{
			if(operationStack.top() == '(')
			{
				XdmfError::message(XdmfError::WARNING,
					"Warning: Unpaired Parenthesis");
			}
			else
			{
				shared_ptr<XdmfArray> val2 = valueStack.top();
				valueStack.pop();
				shared_ptr<XdmfArray> val1 = valueStack.top();
				valueStack.pop();
				if (operationStack.size() == 0)
				{
					XdmfError::message(XdmfError::FATAL,
						"Error: Not Enough Operators in evaluateExpression");
				}
				else
				{
					valueStack.push(evaluateOperation(val1, val2, operationStack.top()));
					operationStack.pop();
				}
			}
		}
	}

	//throw error if there's extra operations
	if (operationStack.size() > 0)
	{
		XdmfError::message(XdmfError::WARNING,
			"Warning: Left Over Operators in evaluateExpression");
	}

	if (valueStack.size() > 1)
	{
		XdmfError::message(XdmfError::WARNING,
			"Warning: Left Over Values in evaluateExpression");
	}

	return valueStack.top();
}

shared_ptr<XdmfArray>
XdmfArray::evaluateOperation(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2, char operation)
{
	if (operations.find(operation) != operations.end())
	{
		return (*(shared_ptr<XdmfArray>(*)(shared_ptr<XdmfArray>, shared_ptr<XdmfArray>))operations[operation])(val1, val2);
	}
	else
	{
		return shared_ptr<XdmfArray>();
	}
}

int
XdmfArray::addOperation(char newoperator, shared_ptr<XdmfArray>(*operationref)(shared_ptr<XdmfArray>, shared_ptr<XdmfArray>), int priority)
{
	if (newoperator == '(' || newoperator == ')')
	{
		XdmfError::message(XdmfError::FATAL,
			"Error: Parenthesis can not be redefined");
	}
	//give warning if the operation already exists
	size_t origsize = operations.size();
	operations[newoperator] = operationref;//place reference in the associated location
	if (origsize == operations.size())
	{//it's nice to let people know they're doing this so they don't get surprised about changes in behavior
		XdmfError::message(XdmfError::WARNING,
			"Warning: Function Overwritten");
		//overwrite the existing info for that operation
		//add the priority to the specified location in the priority array
		size_t priorityLocation = mSupportedOperations.find(newoperator);
		mOperationPriority[priorityLocation] = priority;
	}
	else
	{
		//create new operation
		//as long as the operation isn't a valid function character
		if (mValidVariableChars.find(newoperator) != std::string::npos || mValidDigitChars.find(newoperator) != std::string::npos)
		{
			XdmfError::message(XdmfError::FATAL,
				"Error: Operation Overlaps with Variables");
		}
		else //build the operation
		{
			//add operation to the supported character string
			mSupportedOperations.push_back(newoperator);
			int priorityArraySize = sizeof(mOperationPriority)/sizeof(int);
			if (mSupportedOperations.size()-1 > priorityArraySize)//first check to see if the priority array is large enough
			{
				//if it isn't make it bigger, double size should be fine
				int newArray [priorityArraySize*2];
				std::copy(mOperationPriority, mOperationPriority+(priorityArraySize-1), newArray);
				delete mOperationPriority;
				*mOperationPriority = *newArray;
			}
			size_t priorityLocation = mSupportedOperations.find(newoperator);
			mOperationPriority[priorityLocation] = priority;
		}
	}
	return operations.size();
}

shared_ptr<XdmfArray>
XdmfArray::chunk(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2)
{
	//join chunk (add the new array to the end of the first one)
	//joins into new array and returns it
	shared_ptr<XdmfArray> returnArray = XdmfArray::New();
	returnArray->insert(0, val1, 0, val1->getSize(),  1, 1);
	returnArray->insert(val1->getSize(), val2, 0, val2->getSize(), 1, 1);
	return returnArray;
}

shared_ptr<XdmfArray>
XdmfArray::interlace(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2)
{
	//join interlace (evenly space the second array within the first one)
	//builds a new array
	shared_ptr<XdmfArray> returnArray = XdmfArray::New();
	//resize to the combined size of both arrays
	//determining what type to class it as in order to not lose data, and to still have the smallest data type of the two
	shared_ptr<const XdmfArrayType> arrayType1 = val1->getArrayType();
	shared_ptr<const XdmfArrayType> arrayType2 = val2->getArrayType();
	if (arrayType1 == XdmfArrayType::Int8())
	{
		//if floats reclass as floats of the appropriate size
		if (arrayType2 == XdmfArrayType::String())
		{
			//string is the only compatible type here
			std::string sampleValue = "";
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::Float32())
		{
			float sampleValue = 0.0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::Float64())
		{
			double sampleValue = 0.0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		//if uints reclass as ints of the appropriate size
		else if (arrayType2 == XdmfArrayType::Int64() || arrayType2 == XdmfArrayType::UInt32())
		{
			long sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::Int32() || arrayType2 == XdmfArrayType::UInt16())
		{
			int sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::Int16() || arrayType2 == XdmfArrayType::UInt8())
		{
			short sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else
		{
			char sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
	}
	else if (arrayType1 == XdmfArrayType::Int16())
	{
		//if floats reclass as floats of the appropriate size
		if (arrayType2 == XdmfArrayType::String())
		{
			//string is the only compatible type here
			std::string sampleValue = "";
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::Float32())
		{
			float sampleValue = 0.0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::Float64())
		{
			double sampleValue = 0.0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		//if uints reclass as ints of the appropriate size
		else if (arrayType2 == XdmfArrayType::Int64() || arrayType2 == XdmfArrayType::UInt32())
		{
			long sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::Int32() || arrayType2 == XdmfArrayType::UInt16())
		{
			int sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else
		{
			short sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
	}
	else if (arrayType1 == XdmfArrayType::Int32())
	{
		//if floats reclass as floats of the appropriate size
		if (arrayType2 == XdmfArrayType::String())
		{
			//string is the only compatible type here
			std::string sampleValue = "";
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::Float32())
		{
			float sampleValue = 0.0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::Float64())
		{
			double sampleValue = 0.0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		//if uints reclass as ints of the appropriate size
		else if (arrayType2 == XdmfArrayType::Int64() || arrayType2 == XdmfArrayType::UInt32())
		{
			long sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else
		{
			int sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
	}
	else if (arrayType1 == XdmfArrayType::Int64())
	{
		//if floats reclass as floats of the appropriate size
		if (arrayType2 == XdmfArrayType::String())
		{
			//string is the only compatible type here
			std::string sampleValue = "";
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::Float32())
		{
			float sampleValue = 0.0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::Float64())
		{
			double sampleValue = 0.0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		//if uints reclass as ints of the appropriate size
		else
		{
			long sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
	}
	else if (arrayType1 == XdmfArrayType::Float32())
	{
		//use floats of the appropriate size
		if (arrayType2 == XdmfArrayType::String())
		{
			//string is the only compatible type here
			std::string sampleValue = "";
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::Float64())
		{
			double sampleValue = 0.0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else
		{
			float sampleValue = 0.0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
	}
	else if (arrayType1 == XdmfArrayType::Float64())
	{
		//use floats of the appropriate size
		if (arrayType2 == XdmfArrayType::String())
		{
			//string is the only compatible type here
			std::string sampleValue = "";
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else
		{
			double sampleValue = 0.0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
	}
	else if (arrayType1 == XdmfArrayType::UInt8())
	{
		//if int are used reclass as int of the larger size
		if (arrayType2 == XdmfArrayType::Int8() || arrayType2 == XdmfArrayType::Int16())
		{
			short sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::Int32())
		{
			int sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::Int64())
		{
			long sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		//if floats are used, reclass as floats of the appropriate size
		else if (arrayType2 == XdmfArrayType::Float32())
		{
			float sampleValue = 0.0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::Float64())
		{
			double sampleValue = 0.0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		//if uints are used, adjust size as required
		else if (arrayType2 == XdmfArrayType::UInt8())
		{
			unsigned char sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::UInt16())
		{
			unsigned short sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::UInt32())
		{
			unsigned int sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::String())
		{
			//string is the only compatible type here
			std::string sampleValue = "";
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
	}
	else if (arrayType1 == XdmfArrayType::UInt16())
	{
		//if int are used reclass as int of the larger size
		if (arrayType2 == XdmfArrayType::Int8() || arrayType2 == XdmfArrayType::Int16() || arrayType2 == XdmfArrayType::Int32())
		{
			int sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::Int64())
		{
			long sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		//if floats are used, reclass as floats of the appropriate size
		else if (arrayType2 == XdmfArrayType::Float32())
		{
			float sampleValue = 0.0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::Float64())
		{
			double sampleValue = 0.0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		//if uints are used, adjust size as required
		else if (arrayType2 == XdmfArrayType::UInt8() || arrayType2 == XdmfArrayType::UInt16())
		{
			unsigned short sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::UInt32())
		{
			unsigned int sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::String())
		{
			//string is the only compatible type here
			std::string sampleValue = "";
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
	}
	else if (arrayType1 == XdmfArrayType::UInt32())
	{
		//if int are used reclass as int of the larger size
		if (arrayType2 == XdmfArrayType::Int8() || arrayType2 == XdmfArrayType::Int16() || arrayType2 == XdmfArrayType::Int32() || arrayType2 == XdmfArrayType::Int64())
		{
			long sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		//if floats are used, reclass as floats of the appropriate size
		else if (arrayType2 == XdmfArrayType::Float32())
		{
			float sampleValue = 0.0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::Float64())
		{
			double sampleValue = 0.0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		//if uints are used, adjust size as required
		else if (arrayType2 == XdmfArrayType::UInt8() || arrayType2 == XdmfArrayType::UInt16() || arrayType2 == XdmfArrayType::UInt32())
		{
			unsigned int sampleValue = 0;
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
		else if (arrayType2 == XdmfArrayType::String())
		{
			//string is the only compatible type here
			std::string sampleValue = "";
			returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
		}
	}
	else if (arrayType1 == XdmfArrayType::String())
	{
		//string is the only compatible type here
		std::string sampleValue = "";
		returnArray->resize(val1->getSize()+val2->getSize(), sampleValue);
	}
	//determine ratio of array sizes
	int arrayRatio1 = floor(static_cast<double>(val1->getSize())/val2->getSize());
	int arrayRatio2 = floor(static_cast<double>(val2->getSize())/val1->getSize());
	if (arrayRatio1 < 1)
	{
		arrayRatio1 = 1;
	}
	if (arrayRatio2 < 1)
	{
		arrayRatio2 = 1;
	}
	//stride is equal to the ratios rounded up and added together
	int stride = arrayRatio1+arrayRatio2;
	int arrayExcess1 = 0;
	int arrayExcess2 = 0;
	for (int i = 0; i < stride; i++)
	{
		//add the values of each array, using strides to interlace and starting index to offset
		//first array gets the first value of the new array
		if (i<arrayRatio1)
		{
			int amountWritten = val1->getSize()/arrayRatio1;
			if (((amountWritten * arrayRatio1) + i) < val1->getSize())
			{
				amountWritten++;
			}
			if (amountWritten > floor(val2->getSize()/arrayRatio2))
			{
				arrayExcess1 += amountWritten - floor(val2->getSize()/arrayRatio2);
				amountWritten = floor(val2->getSize()/arrayRatio2);
			}
			returnArray->insert(i, val1, i, amountWritten, stride, arrayRatio1);
		}
		else //second array takes the rest
		{
			int amountWritten = val2->getSize()/arrayRatio2;
			if (((amountWritten * arrayRatio2) + i) < val2->getSize())
			{
				amountWritten++;
			}
			if (amountWritten > floor(val1->getSize()/arrayRatio1))
			{
				arrayExcess2 += amountWritten - floor(val1->getSize()/arrayRatio1);
				amountWritten = floor(val1->getSize()/arrayRatio1);
			}
			returnArray->insert(i, val2, i-arrayRatio1, amountWritten, stride, arrayRatio2);
		}
	}
	if (arrayExcess1 > 0)
	{
		returnArray->insert(val1->getSize()+val2->getSize()-arrayExcess1, val1, 0, arrayExcess1, 1, 1);
	}
	else if (arrayExcess2 > 0)
	{
		returnArray->insert(val1->getSize()+val2->getSize()-arrayExcess2, val2, 0, arrayExcess2, 1, 1);
	}
	returnArray->resize(val1->getSize()+val2->getSize(), 0);
	//after all inserts are done, add the excess values to the end of the array? Possibly do during the initial insert process?
	return returnArray;
}

//this is how you use references to functions
shared_ptr<XdmfArray>
XdmfArray::evaluateFunction(std::vector<shared_ptr<XdmfArray> > valueVector, std::string functionName)
{
	if (arrayFunctions.find(functionName) != arrayFunctions.end())
	{
		return (*(shared_ptr<XdmfArray>(*)(std::vector<shared_ptr<XdmfArray> >))arrayFunctions[functionName])(valueVector);
	}
	else
	{
		return shared_ptr<XdmfArray>();
	}
}

int
XdmfArray::addFunction(std::string name, shared_ptr<XdmfArray>(*functionref)(std::vector<shared_ptr<XdmfArray> >))
{
	//check to ensure that the name has valid characters
	for (int i = 0; i < name.size(); i++)
	{
		if (mValidVariableChars.find(name[i]) == std::string::npos)//if the character is not found in the list of valid characters
		{//then throw an error
			XdmfError::message(XdmfError::FATAL,
				"Error: Function Name Contains Invalid Characters");
		}
	}
	size_t origsize = arrayFunctions.size();
	arrayFunctions[name] = functionref;
	if (origsize == arrayFunctions.size())//if no new functions were added
	{//toss a warning, it's nice to let people know that they're doing this
		XdmfError::message(XdmfError::WARNING,
			"Warning: Function Overwritten");
	}
	return arrayFunctions.size();
}

shared_ptr<XdmfArray>
XdmfArray::sum(std::vector<shared_ptr<XdmfArray> > values)
{
	double total = 0.0;
	for (int i = 0; i < values.size(); i++)
	{
		for (int j = 0; j < values[i]->getSize(); j++)
		{
			total += values[i]->getValue<double>(j);
		}
	}
	shared_ptr<XdmfArray> returnArray = XdmfArray::New();
	returnArray->insert(0, total);
	return returnArray;
}

shared_ptr<XdmfArray>
XdmfArray::ave(std::vector<shared_ptr<XdmfArray> > values)
{
	double total = sum(values)->getValue<double>(0);;
	int totalSize = 0;
	for (int i = 0; i < values.size(); i++)
	{
		totalSize += values[i]->getSize();
	}
	shared_ptr<XdmfArray> returnArray = XdmfArray::New();
	returnArray->insert(0, total/totalSize);
	return returnArray;
}

shared_ptr<const XdmfArrayType>
XdmfArray::getArrayType() const
{
  if (mHeavyDataControllers.size()>0)
  {
    return boost::apply_visitor(GetArrayType(mHeavyDataControllers[0]), 
                                mArray);
  }
  else
  {
    return boost::apply_visitor(GetArrayType(shared_ptr<XdmfHDF5Controller>()),
                                mArray);
  }
}

unsigned int
XdmfArray::getCapacity() const
{
  return boost::apply_visitor(GetCapacity(), 
                              mArray);
}

std::vector<unsigned int>
XdmfArray::getDimensions() const
{
  if(mDimensions.size() == 0) {
    if(!this->isInitialized() && mHeavyDataControllers.size() > 0) {
      std::vector<unsigned int> returnDimensions;
      std::vector<unsigned int> tempDimensions;
	//find the controller with the most dimensions
	int dimControllerIndex = 0;
	int dimSizeMax = 0;
	unsigned int dimTotal = 0;
	for (int i = 0; i < mHeavyDataControllers.size(); i++)
	{
		dimTotal += mHeavyDataControllers[i]->getSize();
		if (mHeavyDataControllers[i]->getSize() > dimSizeMax)
		{
			dimSizeMax = mHeavyDataControllers[i]->getSize();
			dimControllerIndex = i;
		}
	}
	//total up the size of the lower dimensions
	int controllerDimensionSubtotal = 1;
	for (int i = 0; i < mHeavyDataControllers[dimControllerIndex]->getDimensions().size() - 1; i++)
	{
		returnDimensions.push_back(mHeavyDataControllers[dimControllerIndex]->getDimensions()[i]);
		controllerDimensionSubtotal *= mHeavyDataControllers[dimControllerIndex]->getDimensions()[i];
	}
	//divide the total contained by the dimensions by the size of the lower dimensions
	returnDimensions.push_back(dimTotal/controllerDimensionSubtotal);
      return returnDimensions;
    }
    const unsigned int size = this->getSize();
    return std::vector<unsigned int>(1, size);
  }
  return mDimensions;
}

std::string
XdmfArray::getDimensionsString() const
{
  const std::vector<unsigned int> dimensions = this->getDimensions();
  return GetValuesString(dimensions.size()).getValuesString(&dimensions[0],
                                                            dimensions.size());
}

std::map<std::string, std::string>
XdmfArray::getItemProperties() const
{
  std::map<std::string, std::string> arrayProperties;
  if(mHeavyDataControllers.size()>0) {
    arrayProperties.insert(std::make_pair("Format",
                                          mHeavyDataControllers[0]->getName()));
  }
  else {
    arrayProperties.insert(std::make_pair("Format", "XML"));
  }
  arrayProperties.insert(std::make_pair("Dimensions", 
                                        this->getDimensionsString()));
  if(mName.compare("") != 0) {
    arrayProperties.insert(std::make_pair("Name", mName));
  }
  shared_ptr<const XdmfArrayType> type = this->getArrayType();
  type->getProperties(arrayProperties);
  return arrayProperties;
}

std::string
XdmfArray::getItemTag() const
{
  return ItemTag;
}

std::string
XdmfArray::getName() const
{
  return mName;
}

int
XdmfArray::getOperationPriority(char operation)
{
	size_t operationLocation = mSupportedOperations.find(operation);
	if (operationLocation != std::string::npos)
	{
		return mOperationPriority[operationLocation];
	}
	else
	{
		return -1;
	}
}

unsigned int
XdmfArray::getSize() const
{
  return boost::apply_visitor(Size(this), 
                              mArray);
}

const std::string
XdmfArray::getSupportedOperations()
{
	return mSupportedOperations;
}

const std::vector<std::string>
XdmfArray::getSupportedFunctions()
{
	std::vector<std::string> returnVector;
	for (std::map<std::string, shared_ptr<XdmfArray>(*)(std::vector<shared_ptr<XdmfArray> >)>::iterator functionWalker = arrayFunctions.begin();
		functionWalker != arrayFunctions.end(); functionWalker++)
	{
		returnVector.push_back(functionWalker->first);
	}
	return returnVector;
}

const std::string
XdmfArray::getValidVariableChars()
{
	return mValidVariableChars;
}

const std::string
XdmfArray::getValidDigitChars()
{
	return mValidDigitChars;
}

void *
XdmfArray::getValuesInternal()
{
  return const_cast<void *>
    (static_cast<const XdmfArray &>(*this).getValuesInternal());
}

const void *
XdmfArray::getValuesInternal() const
{
  return boost::apply_visitor(GetValuesPointer(), 
                              mArray);
}

std::string
XdmfArray::getValuesString() const
{
  return boost::apply_visitor(GetValuesString(mArrayPointerNumValues), 
                              mArray);
}

shared_ptr<XdmfHeavyDataController>
XdmfArray::getHeavyDataController()
{
  return boost::const_pointer_cast<XdmfHeavyDataController>
    (static_cast<const XdmfArray &>(*this).getHeavyDataController(0));
}

shared_ptr<const XdmfHeavyDataController>
XdmfArray::getHeavyDataController() const
{
  if (mHeavyDataControllers.size()>0) {
    return mHeavyDataControllers[0];
  }
  else {
    return shared_ptr<XdmfHeavyDataController>();
  }
}

void
XdmfArray::setHeavyDataController(shared_ptr<XdmfHeavyDataController> newController)
{
  //since this is replacing the previous version which was designed to completely replace the controller of the array
  //it will clear the current controllers before adding the new one in
  mHeavyDataControllers.clear();
  mHeavyDataControllers.push_back(newController);
}

void
XdmfArray::initialize(const shared_ptr<const XdmfArrayType> arrayType,
                      const unsigned int size)
{
  if(arrayType == XdmfArrayType::Int8()) {
    this->initialize<char>(size);
  }
  else if(arrayType == XdmfArrayType::Int16()) {
    this->initialize<short>(size);
  }
  else if(arrayType == XdmfArrayType::Int32()) {
    this->initialize<int>(size);
  }
  else if(arrayType == XdmfArrayType::Int64()) {
    this->initialize<long>(size);
  }
  else if(arrayType == XdmfArrayType::Float32()) {
    this->initialize<float>(size);
  }
  else if(arrayType == XdmfArrayType::Float64()) {
    this->initialize<double>(size);
  }
  else if(arrayType == XdmfArrayType::UInt8()) {
    this->initialize<unsigned char>(size);
  }
  else if(arrayType == XdmfArrayType::UInt16()) {
    this->initialize<unsigned short>(size);
  }
  else if(arrayType == XdmfArrayType::UInt32()) {
    this->initialize<unsigned int>(size);
  }
  else if(arrayType == XdmfArrayType::String()) {
    this->initialize<std::string>(size);
  }
  else if(arrayType == XdmfArrayType::Uninitialized()) {
    this->release();
  }
  else {
    XdmfError::message(XdmfError::FATAL, 
                       "Array of unsupported type in XdmfArray::initialize");
  }
}

void
XdmfArray::initialize(const shared_ptr<const XdmfArrayType> arrayType,
                      const std::vector<unsigned int> & dimensions)
{
  mDimensions = dimensions;
  const unsigned int size = std::accumulate(dimensions.begin(),
                                            dimensions.end(),
                                            1,
                                            std::multiplies<unsigned int>());
  return this->initialize(arrayType, size);
}

void
XdmfArray::insert(const unsigned int startIndex,
                  const shared_ptr<const XdmfArray> values,
                  const unsigned int valuesStartIndex,
                  const unsigned int numValues,
                  const unsigned int arrayStride,
                  const unsigned int valuesStride)
{
  boost::apply_visitor(InsertArray(this,
                                   startIndex,
                                   valuesStartIndex,
                                   numValues,
                                   arrayStride,
                                   valuesStride,
                                   mDimensions,
                                   values),
                       mArray);
}

bool
XdmfArray::isInitialized() const
{
  try {
    boost::get<boost::blank>(mArray);
    return false;
  }
  catch(const boost::bad_get & exception) {
  }
  return true;
}

void
XdmfArray::internalizeArrayPointer()
{
  boost::apply_visitor(InternalizeArrayPointer(this), 
                       mArray);
}

void
XdmfArray::populateItem(const std::map<std::string, std::string> & itemProperties,
                        const std::vector<shared_ptr<XdmfItem> > & childItems,
                        const XdmfCoreReader * const reader)
{
  //This inserts any XdmfInformation in childItems into the object.
  XdmfItem::populateItem(itemProperties, childItems, reader);

  const shared_ptr<const XdmfArrayType> arrayType = 
    XdmfArrayType::New(itemProperties);

  std::map<std::string, std::string>::const_iterator content =
    itemProperties.find("Content0");
  if(content == itemProperties.end()) {
    XdmfError::message(XdmfError::FATAL, 
                       "'Content' not found in itemProperties in "
                       "XdmfArray::populateItem");
  }

  //this was originally a constant, but the compiler doesn't like creating vectors of constant strings.
  std::vector<std::string> contentVals;
  contentVals.push_back(content->second);

  int contentIndex=1;

  bool endOfContent = false;

  while(!endOfContent)
  {
    std::stringstream contentSearch;
    contentSearch << "Content" << contentIndex;
    content = itemProperties.find(contentSearch.str());
    if(content != itemProperties.end()) {
      contentVals.push_back(content->second);
    }
    else {
      endOfContent = true;
    }
    contentIndex++;
  }

  std::map<std::string, std::string>::const_iterator dimensions =
    itemProperties.find("Dimensions");
  if(dimensions == itemProperties.end()) {
    XdmfError::message(XdmfError::FATAL, 
                       "'Dimensions' not found in itemProperties in "
                       "XdmfArray::populateItem");
  }
   
  boost::tokenizer<> tokens(dimensions->second);
  for(boost::tokenizer<>::const_iterator iter = tokens.begin();
      iter != tokens.end();
      ++iter) {
    mDimensions.push_back(atoi((*iter).c_str()));
  }

  std::map<std::string, std::string>::const_iterator format =
    itemProperties.find("Format");
  if(format == itemProperties.end()) {
    XdmfError::message(XdmfError::FATAL, 
                       "'Format' not found in itemProperties in "
                       "XdmfArray::populateItem");
  }
  const std::string & formatVal = format->second;


  if(formatVal.compare("HDF") == 0) {
    contentIndex = 0;
    int contentStep = 2;
    while (contentIndex < contentVals.size()) {
      size_t colonLocation = contentVals[contentIndex].find(":");
      if(colonLocation == std::string::npos) {
        XdmfError::message(XdmfError::FATAL, 
                           "':' not found in content in "
                           "XdmfArray::populateItem -- double check an HDF5 "
                           "data set is specified for the file");
      }

      std::string hdf5Path = contentVals[contentIndex].substr(0, colonLocation);
      std::string dataSetPath = contentVals[contentIndex].substr(colonLocation+1);

      //parse dimensions from the content
      std::vector<unsigned int> contentDims;
      try {
        //this is the string that contains the dimensions
        boost::tokenizer<> dimtokens(contentVals[contentIndex+1]);
        for(boost::tokenizer<>::const_iterator iter = dimtokens.begin();
            iter != dimtokens.end();
            ++iter) {
          contentDims.push_back(atoi((*iter).c_str()));
        }
	contentStep = 2;//if this works then the dimension content should be skipped over
      }
      catch (...) {//if it fails then it means that the next content is not a dimension string
        //in this case it is assumed that the controller will have dimensions equal to the array
        for (int j = 0; j < mDimensions.size(); j++) {
          contentDims.push_back(mDimensions[j]);
        }
        contentStep = 1;
      }

      mHeavyDataControllers.push_back( 
        XdmfHDF5Controller::New(hdf5Path,
                                dataSetPath,
                                arrayType,
                                std::vector<unsigned int>(contentDims.size(),
                                                          0),
                                std::vector<unsigned int>(contentDims.size(),
                                                          1),
                                contentDims,
                                contentDims)
      );
      contentIndex+=contentStep;
    }
  }
  else if(formatVal.compare("XML") == 0) {
    this->initialize(arrayType,
                     mDimensions);
    unsigned int index = 0;
    boost::char_separator<char> sep(" \t\n");
    boost::tokenizer<boost::char_separator<char> > tokens(contentVals[0], sep);
    if(arrayType == XdmfArrayType::String()) {
      for(boost::tokenizer<boost::char_separator<char> >::const_iterator
            iter = tokens.begin();
          iter != tokens.end();
          ++iter, ++index) {
        this->insert(index, *iter);
      }
    }
    else {
      for(boost::tokenizer<boost::char_separator<char> >::const_iterator
            iter = tokens.begin();
          iter != tokens.end();
          ++iter, ++index) {
        this->insert(index, atof((*iter).c_str()));
      }
    }
  }
  else {
    XdmfError::message(XdmfError::FATAL, 
                       "Neither 'HDF' nor 'XML' specified as 'Format' "
                       "in XdmfArray::populateItem");
  }

  std::map<std::string, std::string>::const_iterator name =
    itemProperties.find("Name");
  if(name != itemProperties.end()) {
    mName = name->second;
  }
  else {
    mName = "";
  }
}

void
XdmfArray::read()
{
  if(mHeavyDataControllers.size() > 0) {
    this->release();
    for (int i = 0; i < mHeavyDataControllers.size(); i++)
    {
      shared_ptr<XdmfArray> tempArray = XdmfArray::New();
      mHeavyDataControllers[i]->read(tempArray.get());
      unsigned int startsTotal = 0;
      unsigned int strideTotal = 1;
      unsigned int dimTotal = 1;
      for (int j = 0; j < mHeavyDataControllers[i]->getDimensions().size(); j++) {
        strideTotal *= mHeavyDataControllers[i]->getStride()[j];
	startsTotal += dimTotal * mHeavyDataControllers[i]->getStart()[j];
        dimTotal *= mHeavyDataControllers[i]->getDimensions()[j];
      }
	
      this->insert(mHeavyDataControllers[i]->getArrayOffset() + startsTotal, tempArray, 0, dimTotal, strideTotal, 1);
    }
  }
}

void
XdmfArray::release()
{
  mArray = boost::blank();
  mArrayPointerNumValues = 0;
  mDimensions.clear();
}

void
XdmfArray::reserve(const unsigned int size)
{
  boost::apply_visitor(Reserve(this,
                               size),
                       mArray);
}

void
XdmfArray::setName(const std::string & name)
{
  mName = name;
}

void
XdmfArray::swap(const shared_ptr<XdmfArray> array)
{
  std::swap(mArray, array->mArray);
  std::swap(mArrayPointerNumValues, array->mArrayPointerNumValues);
  std::swap(mDimensions, array->mDimensions);
  std::swap(mHeavyDataControllers, array->mHeavyDataControllers);
}
