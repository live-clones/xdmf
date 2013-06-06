#include <iostream>
#include <stack>
#include <map>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <XdmfArray.hpp>
#include <XdmfArrayType.hpp>
#include "boost/assign.hpp"

double parse(std::string expression, std::map<std::string, double> variables);
double calculation(double val1, double val2, char operation);
double function(std::vector<double> valueVector, std::string functionName);
double sum(std::vector<double> values);
double ave(std::vector<double> values);
shared_ptr<XdmfArray> parse(std::string expression, std::map<std::string, shared_ptr<XdmfArray> > variables);
shared_ptr<XdmfArray> calculation(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2, char operation);
shared_ptr<XdmfArray> invChunk(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2);
shared_ptr<XdmfArray> function(std::vector<shared_ptr<XdmfArray> > valueVector, std::string functionName);
shared_ptr<XdmfArray> sum(std::vector<shared_ptr<XdmfArray> > values);
shared_ptr<XdmfArray> ave(std::vector<shared_ptr<XdmfArray> > values);
shared_ptr<XdmfArray> maximum(std::vector<shared_ptr<XdmfArray> > values);

std::string validDigitChars = "1234567890.";
std::string validVariableChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_:.";

//std::map<std::string,  double (*)(std::vector<double>)> functions = boost::assign::map_list_of ("SUM", sum) ("AVE", ave);
std::map<std::string,  double (*)(std::vector<double>)> functions = boost::assign::map_list_of ("SUM", (double (*)(std::vector<double>))sum);

//note, it doesn't handle overloaded functions well. Will generate errors unless typecast
//std::map<std::string,  double (*)(std::vector<double>)> functions = boost::assign::map_list_of ("SUM", sum) ("AVE", ave);
std::map<std::string,  shared_ptr<XdmfArray> (*)(std::vector<shared_ptr<XdmfArray> >)> arrayFunctions = boost::assign::map_list_of ("SUM", (shared_ptr<XdmfArray> (*)(std::vector<shared_ptr<XdmfArray> >))sum) ("MAX", (shared_ptr<XdmfArray> (*)(std::vector<shared_ptr<XdmfArray> >))maximum);



int main(int, char **)
{
	XdmfArray::addFunction("MAX", maximum);
        XdmfArray::addOperation('&', invChunk, 2);
	XdmfArray::addOperation('!', invChunk, 2);
	XdmfArray::addOperation('^', invChunk, 2);
	XdmfArray::addOperation('>', invChunk, 2);
	XdmfArray::addOperation('<', invChunk, 2);
	XdmfArray::addOperation('@', invChunk, 2);

	functions["AVE"] = ave;
	//sometimes typecasts are required, sometimes they cause errors
	//I think it needs to be cast if it isn't the first variation of the function listed
	arrayFunctions["AVE"] = (shared_ptr<XdmfArray> (*)(std::vector<shared_ptr<XdmfArray> >))ave;

	std::string problemToSolve = "2*twentyfive+2*3+2^(3+8)/3+8/(4+1)+SUM(4, twentyfive*4, 6) + AVE(100, 0, 0, 0)";

	std::map<std::string, double> variableTable;
	variableTable["twentyfive"] = 25;
	variableTable["one"] = 1;
	variableTable["two"] = 2;
	variableTable["three"] = 3;
	variableTable["four"] = 4;
	variableTable["eight"] = 8;

	double answer = parse(problemToSolve, variableTable);

	std::cout << answer << std::endl;

	//unless the calculation is fully written out it won't equal properly
	assert(answer == 2*25+2*3+(double)2048/3+(double)8/5+4+100+6+25);

	//std::string arrayExpression = "A|B#C|D";
	std::string arrayExpression = "MAX(2,(AVE(A@B)#AVE(C|D)))";

	shared_ptr<XdmfArray> testArray1 = XdmfArray::New();
	for (int i = 0; i < 10; i++)
	{
		testArray1->pushBack(1);
	}
	shared_ptr<XdmfArray> testArray2 = XdmfArray::New();
	for (int i = 0; i < 10; i++)
	{
		testArray2->pushBack(2);
	}
	shared_ptr<XdmfArray> testArray3 = XdmfArray::New();
	for (int i = 0; i < 10; i++)
	{
		testArray3->pushBack(3);
	}
	shared_ptr<XdmfArray> testArray4 = XdmfArray::New();
	for (int i = 0; i < 10; i++)
	{
		testArray4->pushBack(4);
	}
	shared_ptr<XdmfArray> testArray5 = XdmfArray::New();
	for (int i = 0; i < 13; i++)
	{
		testArray5->pushBack(5);
	}

	std::map<std::string, shared_ptr<XdmfArray> > arrayVariable;
	arrayVariable["A"] = testArray1;
	arrayVariable["B"] = testArray2;
	arrayVariable["C"] = testArray3;
	arrayVariable["D"] = testArray4;
	arrayVariable["E"] = testArray5;

	std::cout << "before parsing" << std::endl;

	shared_ptr<XdmfArray> answerArray;
	answerArray = parse(arrayExpression, arrayVariable);

	std::cout << "after parsing" << std::endl;

	std::cout << "answer array = " << answerArray->getValuesString() << std::endl;

	assert(answerArray->getValuesString().compare("3.5") == 0);

	std::cout << "array size = " << answerArray->getSize() << std::endl;

	assert(answerArray->getSize() == 1);

	std::cout << "interlace" << std::endl;
	answerArray = XdmfArray::evaluateOperation(testArray1, testArray5, '#');
	std::cout << "answer array = " << answerArray->getValuesString() << std::endl;

	assert(answerArray->getValuesString().compare("1 5 1 5 1 5 1 5 1 5 1 5 1 5 1 5 1 5 1 5 5 5 5") == 0);

	std::cout << "chunk" << std::endl;
	answerArray = XdmfArray::evaluateOperation(testArray1, testArray5, '|');
	std::cout << "answer array = " << answerArray->getValuesString() << std::endl;

	assert(answerArray->getValuesString().compare("1 1 1 1 1 1 1 1 1 1 5 5 5 5 5 5 5 5 5 5 5 5 5") == 0);

	std::cout << "inverse chunk" << std::endl;
	answerArray = XdmfArray::evaluateOperation(testArray1, testArray5, '@');
	std::cout << "answer array = " << answerArray->getValuesString() << std::endl;

	assert(answerArray->getValuesString().compare("5 5 5 5 5 5 5 5 5 5 5 5 5 1 1 1 1 1 1 1 1 1 1") == 0);

	answerArray = XdmfArray::evaluateExpression(arrayExpression, arrayVariable);

	std::cout << "after parsing" << std::endl;

	std::cout << "answer array = " << answerArray->getValuesString() << std::endl;

	assert(answerArray->getValuesString().compare("3.5") == 0);

	std::cout << "array size = " << answerArray->getSize() << std::endl;

	assert(answerArray->getSize() == 1);

	return 0;
}

double parse(std::string expression, std::map<std::string, double> variables)
{
	std::stack<double> valueStack;
	std::stack<char> operationStack;

	std::string validOperationChars = "+-*/()^"; //will be global at some point, all supported operations
	int operationPriority [validOperationChars.size()];
	//list the priorities for the operations, based on the order of operations
	//the index of the corresponding operation in validOperationChars is the same as the index of its priority in this array
	operationPriority[0] = 1;//+
	operationPriority[1] = 1;//-
	operationPriority[2] = 2;//*
	operationPriority[3] = 2;///
	operationPriority[4] = 0;//(
	operationPriority[5] = 0;//)
	operationPriority[6] = 3;//^
	//the higher the value, the earlier the operation is evaluated in the order of operations
	//with the exception of parenthesis which are evaluated as soon as the closing parenthesis is found

	//string is parsed left to right
	//elements of the same priority are evaluated right to left
	for (unsigned int i = 0; i < expression.size(); i++)
	{
		if (validDigitChars.find(expression[i]) != std::string::npos)//found to be a digit
		{
			//progress until a non-digit is found
			int valueStart = i;
			while (validDigitChars.find(expression[i + 1]) != std::string::npos)
			{
				i++;
			}
			//push back to the value stack
			valueStack.push(atof(expression.substr(valueStart, i + 1 - valueStart).c_str()));//use this to convert to double
		}
		else if (validVariableChars.find(expression[i]) != std::string::npos)//found to be a variable
		{
			int valueStart = i;
			//progress until a nonvariable value is found
			while (validVariableChars.find(expression[i + 1]) != std::string::npos)
			{
				i++;
			}
			//convert to equivalent
			if (variables.find(expression.substr(valueStart, i + 1 - valueStart)) == variables.end())
			{
				if (functions.find(expression.substr(valueStart, i + 1 - valueStart)) == functions.end())
				{
					std::cout << "Error: Invalid Variable or Function: " << expression.substr(valueStart, i + 1 - valueStart) << std::endl;
					return 0;
				}
				else
				{
					std::string currentFunction = expression.substr(valueStart, i + 1 - valueStart);
					//check if next character is an open parenthesis
					if (expression[i+1] != '(')
					{
						std::cout << "Error: No values supplied to function " << expression.substr(valueStart, i + 1 - valueStart) << std::endl;
						return 0;
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
					std::vector<double> parameterVector;
					//split that string at commas
					size_t parameterSplit = 0;
					while (parameterSplit != std::string::npos)
					{
						parameterSplit = 0;
						parameterSplit = functionParameters.find_first_of(",", parameterSplit);
						//feed the substrings to the parse function
						if (parameterSplit == std::string::npos)
						{
							parameterVector.push_back(parse(functionParameters, variables));
						}
						else
						{
							parameterVector.push_back(parse(functionParameters.substr(0, parameterSplit), variables));
							functionParameters = functionParameters.substr(parameterSplit+1);
						}
					}
					valueStack.push(function(parameterVector, currentFunction));
				}
			}
			else
			{
				//push equivalent to value stack
				valueStack.push(variables.find(expression.substr(valueStart, i + 1 - valueStart))->second);
			}
		}
		else if (validOperationChars.find(expression[i]) != std::string::npos)//found to be an operation
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
							//error, not enough values
							std::cout << "Error: Not Enough Values" << std::endl;
							return 0;
						}
						else
						{
							double val2 = valueStack.top();
							valueStack.pop();
							double val1 = valueStack.top();
							valueStack.pop();
							valueStack.push(calculation(val1, val2, operationStack.top()));
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
					size_t operationLocation = validOperationChars.find(expression[i]);
					size_t topOperationLocation = validOperationChars.find(operationStack.top());
					//see order of operations to determine importance
					while (operationStack.size() > 0 && operationPriority[operationLocation] < operationPriority[topOperationLocation])
					{
						if (valueStack.size() < 2)//must be at least two values for this loop to work properly
						{
							//error, not enough values
							std::cout << "Error: Not Enough Values" << std::endl;
							return 0;
						}
						else
						{
							double val2 = valueStack.top();
							valueStack.pop();
							double val1 = valueStack.top();
							valueStack.pop();
							valueStack.push(calculation(val1, val2, operationStack.top()));
							operationStack.pop();
							if (operationStack.size() == 0) 
							{
								break;
							}
							topOperationLocation = validOperationChars.find(operationStack.top());
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
	while (valueStack.size() > 1)
	{
		if (valueStack.size() < 2)//must be at least two values for this loop to work properly
		{
			//error, not enough values
			std::cout << "Error: Not Enough Values" << std::endl;
			return 0;
		}
		else
		{
			if(operationStack.top() == '(')
			{
				std::cout << "Warning: Unpaired Parenthesis" << std::endl;
				operationStack.pop();
			}
			else
			{
				double val2 = valueStack.top();
				valueStack.pop();
				double val1 = valueStack.top();
				valueStack.pop();
				if (operationStack.size() == 0)
				{
					//error, not enough operations
					std::cout << "Error: Not Enough Operators" << std::endl;
					return 0;
				}
				else
				{
					valueStack.push(calculation(val1, val2, operationStack.top()));
					operationStack.pop();
				}
			}
		}
	}

	//throw error if there's extra operations
	if (operationStack.size() > 0)
	{
		std::cout << "Warning: Left Over Operators" << std::endl;
	}

	return valueStack.top();
}

double calculation(double val1, double val2, char operation)
{
	//perform the appropriate operation as designated by the string supplied
	if (operation == '+')//addition
	{
		return val1 + val2;
	}
	else if (operation == '-')//subtraction
	{
		return val1 - val2;
	}
	else if (operation == '*')//multiplication
	{
		return val1 * val2;
	}
	else if (operation == '/')//division
	{
		return val1 / val2;
	}
	else if (operation == '^')//exponent
	{
		return pow(val1, val2);
	}
	//insert new operations into this huge if statement
	return 0.0;//return 0 as a default
}

//this is how you use references to functions
double function(std::vector<double> valueVector, std::string functionName)
{
	return (*functions[functionName])(valueVector);
}

double sum(std::vector<double> values)
{
	double total = 0.0;
	for (unsigned int i = 0; i < values.size(); i++)
	{
		total += values[i];
	}
	return total;
}

double ave(std::vector<double> values)
{
	double total = sum(values);
	return total/values.size();
}

shared_ptr<XdmfArray> parse(std::string expression, std::map<std::string, shared_ptr<XdmfArray> > variables)
{
	std::stack<shared_ptr<XdmfArray> > valueStack;
	std::stack<char> operationStack;

	std::string validOperationChars = "|#()@"; //will be global at some point, all supported operations
	int operationPriority [validOperationChars.size()];
	//list the priorities for the operations, based on the order of operations
	//the index of the corresponding operation in validOperationChars is the same as the index of its priority in this array
	operationPriority[0] = 2;//|
	operationPriority[1] = 1;//#
	operationPriority[2] = 0;//(
	operationPriority[3] = 0;//)
	operationPriority[4] = 2;//@
	//the higher the value, the earlier the operation is evaluated in the order of operations
	//with the exception of parenthesis which are evaluated as soon as the closing parenthesis is found

	//string is parsed left to right
	//elements of the same priority are evaluated right to left
	for (unsigned int i = 0; i < expression.size(); i++)
	{
		if (validDigitChars.find(expression[i]) != std::string::npos)//found to be a digit
		{
			//progress until a non-digit is found
			int valueStart = i;
			while (validDigitChars.find(expression[i + 1]) != std::string::npos)
			{
				i++;
			}
			//push back to the value stack
			//create a new array to hold the value
			shared_ptr<XdmfArray> valueArray = XdmfArray::New();
			valueArray->insert(0, atof(expression.substr(valueStart, i + 1 - valueStart).c_str()));
			valueStack.push(valueArray);
		}
		else if (validVariableChars.find(expression[i]) != std::string::npos)//found to be a variable
		{
			int valueStart = i;
			//progress until a nonvariable value is found
			while (validVariableChars.find(expression[i + 1]) != std::string::npos)
			{
				i++;
			}
			//convert to equivalent
			if (variables.find(expression.substr(valueStart, i + 1 - valueStart)) == variables.end())
			{
				if (arrayFunctions.find(expression.substr(valueStart, i + 1 - valueStart)) == arrayFunctions.end())
				{
					std::cout << "Error: Invalid Variable or Function" << std::endl;
					return XdmfArray::New();
				}
				else
				{
					std::string currentFunction = expression.substr(valueStart, i + 1 - valueStart);
					//check if next character is an open parenthesis
					if (expression[i+1] != '(')
					{
						std::cout << "Error: No Values Supplied to Function " << expression.substr(valueStart, i + 1 - valueStart) << std::endl;
						return XdmfArray::New();
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
							parameterVector.push_back(parse(functionParameters, variables));
						}
						else
						{
							parameterVector.push_back(parse(functionParameters.substr(0, parameterSplit), variables));
							functionParameters = functionParameters.substr(parameterSplit+1);
						}
					}
					valueStack.push(function(parameterVector, currentFunction));
				}
			}
			else
			{
				//push equivalent to value stack
				valueStack.push(variables.find(expression.substr(valueStart, i + 1 - valueStart))->second);
			}
		}
		else if (validOperationChars.find(expression[i]) != std::string::npos)//found to be an operation
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
							//error, not enough values
							return XdmfArray::New();
						}
						else
						{
							shared_ptr<XdmfArray> val2 = valueStack.top();
							valueStack.pop();
							shared_ptr<XdmfArray> val1 = valueStack.top();
							valueStack.pop();
							valueStack.push(calculation(val1, val2, operationStack.top()));
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
					size_t operationLocation = validOperationChars.find(expression[i]);
					size_t topOperationLocation = validOperationChars.find(operationStack.top());
					//see order of operations to determine importance
					while (operationStack.size() > 0 && operationPriority[operationLocation] < operationPriority[topOperationLocation])
					{
						if (valueStack.size() < 2)//must be at least two values for this loop to work properly
						{
							//error, not enough values
							std::cout << "Error: Not Enough Values" << std::endl;
							return XdmfArray::New();
						}
						else
						{
							shared_ptr<XdmfArray> val2 = valueStack.top();
							valueStack.pop();
							shared_ptr<XdmfArray> val1 = valueStack.top();
							valueStack.pop();
							valueStack.push(calculation(val1, val2, operationStack.top()));
							operationStack.pop();
							if (operationStack.size() == 0)
							{
								break;
							}
							topOperationLocation = validOperationChars.find(operationStack.top());
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
		if(operationStack.top() == '(')
		{
			std::cout << "Warning: Unpaired Parenthesis" << std::endl;
			operationStack.pop();
		}
		else if (valueStack.size() < 2)//must be at least two values for this loop to work properly
		{
			//error, not enough values
			std::cout << "Error: Not Enough Values" << std::endl;
			return XdmfArray::New();
		}
		else
		{
			shared_ptr<XdmfArray> val2 = valueStack.top();
			valueStack.pop();
			shared_ptr<XdmfArray> val1 = valueStack.top();
			valueStack.pop();
			if (operationStack.size() == 0)
			{
				//error, not enough operations
				std::cout << "Error: Not Enough Operators" << std::endl;
				return XdmfArray::New();
			}
			else
			{
				valueStack.push(calculation(val1, val2, operationStack.top()));
				operationStack.pop();
			}
		}
	}

	//throw error if there's extra operations
	if (operationStack.size() > 0)
	{
		std::cout << "Warning: Left Over Operators" << std::endl;
	}

	if (valueStack.size() > 1)
	{
		std::cout << "Warning: Left Over Values" << std::endl;
	}

	return valueStack.top();
}

shared_ptr<XdmfArray> calculation(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2, char operation)
{
	//perform the appropriate operation as designated by the string supplied
	if (operation == '|')//join chunk (add the new array to the end of the first one)
	{
		//joins into new array and returns it
		shared_ptr<XdmfArray> returnArray = XdmfArray::New();
		returnArray->insert(0, val1, 0, val1->getSize(),  1, 1);
		returnArray->insert(val1->getSize(), val2, 0, val2->getSize(), 1, 1);
		return returnArray;
	}
	else if (operation == '@')//inverse chunk
	{
		//joins into new array and returns it
		shared_ptr<XdmfArray> returnArray = XdmfArray::New();
		returnArray->insert(0, val2, 0, val2->getSize(),  1, 1);
		returnArray->insert(val2->getSize(), val1, 0, val1->getSize(), 1, 1);
		return returnArray;
	}
	else if (operation == '#')//join interlace (evenly space the second array within the first one)
	{
		//builds a new array
		shared_ptr<XdmfArray> returnArray = XdmfArray::New();
		//resize to the combined size of both arrays
		//might not be required?
		//what type to class it as?
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
		int arrayRatio1 = (int)floor(static_cast<double>(val1->getSize())/val2->getSize());
		int arrayRatio2 = (int)floor(static_cast<double>(val2->getSize())/val1->getSize());
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
				if (((amountWritten * arrayRatio1) + i) < (int)val1->getSize())
				{
					amountWritten++;
				}
				if (amountWritten > (int)floor(val2->getSize()/arrayRatio2))
				{
					arrayExcess1 += amountWritten - (int)floor(val2->getSize()/arrayRatio2);
					amountWritten = (int)floor(val2->getSize()/arrayRatio2);
				}
				returnArray->insert(i, val1, i, amountWritten, stride, arrayRatio1);
			}
			else //second array takes the rest
			{
				int amountWritten = val2->getSize()/arrayRatio2;
				if (((amountWritten * arrayRatio2) + i) < (int)val2->getSize())
				{
					amountWritten++;
				}
				if (amountWritten > (int)floor(val1->getSize()/arrayRatio1))
				{
					arrayExcess2 += amountWritten - (int)floor(val1->getSize()/arrayRatio1);
					amountWritten = (int)floor(val1->getSize()/arrayRatio1);
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
		//after all inserts are done, add the excess values to the end of the array
		return returnArray;
	}
	//insert new operations into this huge if statement
	return XdmfArray::New();//return a blank array as a default
}

shared_ptr<XdmfArray> invChunk(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2)
{
	//joins into new array and returns it
	shared_ptr<XdmfArray> returnArray = XdmfArray::New();
	returnArray->insert(0, val2, 0, val2->getSize(),  1, 1);
	returnArray->insert(val2->getSize(), val1, 0, val1->getSize(), 1, 1);
	return returnArray;
}

//this is how you use references to functions
shared_ptr<XdmfArray> function(std::vector<shared_ptr<XdmfArray> > valueVector, std::string functionName)
{
	if (arrayFunctions.find(functionName) == arrayFunctions.end())
	{
		return XdmfArray::New();
	}
	else
	{
		return (*arrayFunctions[functionName])(valueVector);
	}
}

shared_ptr<XdmfArray> sum(std::vector<shared_ptr<XdmfArray> > values)
{
        double total = 0.0;
        for (unsigned int i = 0; i < values.size(); i++)
        {
		for (unsigned int j = 0; j < values[i]->getSize(); j++)
		{
                	total += values[i]->getValue<double>(j);
		}
        }
	shared_ptr<XdmfArray> returnArray = XdmfArray::New();
	returnArray->insert(0, total);
        return returnArray;
}

shared_ptr<XdmfArray> ave(std::vector<shared_ptr<XdmfArray> > values)
{
	double total = sum(values)->getValue<double>(0);;
	int totalSize = 0;
	for (unsigned int i = 0; i < values.size(); i++)
	{
		totalSize += values[i]->getSize();
	}
	shared_ptr<XdmfArray> returnArray = XdmfArray::New();
	returnArray->insert(0, total/totalSize);
	return returnArray;
}

shared_ptr<XdmfArray> maximum(std::vector<shared_ptr<XdmfArray> > values)
{
	if (values[0]->getArrayType() == XdmfArrayType::String())
	{
		shared_ptr<XdmfArray> returnArray = XdmfArray::New();
		returnArray->pushBack(values[0]->getValue<std::string>(0));
		return returnArray;
	}
	else
	{
		double maxVal = values[0]->getValue<double>(0);
		for (unsigned int i = 0; i < values.size(); i++)
		{
			for (unsigned int j = 0; j < values[i]->getSize(); j++)
			{
				if (maxVal < values[i]->getValue<double>(j))
				{
					maxVal = values[i]->getValue<double>(j);
				}
			}
		}
		shared_ptr<XdmfArray> returnArray = XdmfArray::New();
		returnArray->pushBack(maxVal);
		return returnArray;
	}
}
