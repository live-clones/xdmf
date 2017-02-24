#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfFunction.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfSet.hpp"
#include "XdmfWriter.hpp"
#include "XdmfReader.hpp"
#include <map>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <assert.h>
#include "string.h"
#include "stdio.h"

shared_ptr<XdmfArray> maximum(std::vector<shared_ptr<XdmfArray> > values);

shared_ptr<XdmfArray> prepend(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2);

int main(int, char **)
{
	std::string functionExpression = "MAX(A,B)@(A#B)";

	int numOperations = XdmfFunction::addOperation('@', (shared_ptr<XdmfArray>(*)(shared_ptr<XdmfArray>, shared_ptr<XdmfArray>))prepend, 2);
	std::cout << "Number of operations after addition: " << numOperations << std::endl;
	int numFunctions = XdmfFunction::addFunction("MAX", (shared_ptr<XdmfArray>(*)(std::vector<shared_ptr<XdmfArray> >))maximum);
	std::cout << "Number of functions after addition: " << numFunctions << std::endl;

	std::map<std::string, shared_ptr<XdmfArray> > functionVariables;

	shared_ptr<XdmfArray> variable1 = XdmfArray::New();



	for (unsigned int i = 0; i < 15; ++i)
	{
		variable1->pushBack(i*3);
	}

	functionVariables["A"] = variable1;



        shared_ptr<XdmfArray> variable2 = XdmfArray::New();

        for (unsigned int i = 0; i < 15; ++i)
        {
                variable2->pushBack(i*5);
        }

        functionVariables["B"] = variable2;



	shared_ptr<XdmfFunction> testFunction = XdmfFunction::New(functionExpression, functionVariables);

	shared_ptr<XdmfAttribute> functionHolder = XdmfAttribute::New();

	functionHolder->setReference(testFunction);

	functionHolder->setReadMode(XdmfArray::Reference);



	shared_ptr<XdmfWriter> functionWriter = XdmfWriter::New("function.xmf");

	functionHolder->accept(functionWriter);



	functionHolder->readReference();

	std::cout << functionHolder->getValuesString() << std::endl;

	shared_ptr<XdmfReader> functionReader = XdmfReader::New();

	shared_ptr<XdmfItem> readItem = functionReader->read("function.xmf");

        std::cout << readItem->getItemTag() << " ?= Attribute" << std::endl;

	assert(readItem->getItemTag().compare("Attribute") == 0);

	shared_ptr<XdmfAttribute> readFunctionHolder = shared_dynamic_cast<XdmfAttribute>(readItem);

	readFunctionHolder->release();

        std::cout << "before reading: " << readFunctionHolder->getValuesString() << std::endl;

	readFunctionHolder->readReference();

	std::cout << "check: " << readFunctionHolder->getValuesString() << " = " << functionHolder->getValuesString() << std::endl;

	assert(readFunctionHolder->getValuesString().compare(functionHolder->getValuesString()) == 0);


        shared_ptr<XdmfSet> functionHolder2 = XdmfSet::New();

        functionHolder2->setReference(testFunction);

        functionHolder2->setReadMode(XdmfArray::Reference);

        functionHolder2->accept(functionWriter);

        functionHolder2->readReference();

        std::cout << functionHolder2->getValuesString() << std::endl;

        readItem = functionReader->read("function.xmf");

        std::cout << readItem->getItemTag() << " ?= Set" << std::endl;

        assert(readItem->getItemTag().compare("Set") == 0);

        shared_ptr<XdmfSet> readFunctionHolder2 = shared_dynamic_cast<XdmfSet>(readItem);

        readFunctionHolder2->release();

        std::cout << "before reading: " << readFunctionHolder2->getValuesString() << std::endl;

        readFunctionHolder2->readReference();

        std::cout << "check: " << readFunctionHolder2->getValuesString() << " = " << functionHolder2->getValuesString() << std::endl;

        assert(readFunctionHolder2->getValuesString().compare(functionHolder2->getValuesString()) == 0);



        shared_ptr<XdmfArray> functionHolder3 = XdmfArray::New();

        functionHolder3->setReference(testFunction);

        functionHolder3->setReadMode(XdmfArray::Reference);

        functionHolder3->accept(functionWriter);

        functionHolder3->readReference();

        std::cout << functionHolder3->getValuesString() << std::endl;

        readItem = functionReader->read("function.xmf");

        std::cout << readItem->getItemTag() << " ?= DataItem" << std::endl;

        assert(readItem->getItemTag().compare("DataItem") == 0);

        shared_ptr<XdmfArray> readFunctionHolder3 = shared_dynamic_cast<XdmfArray>(readItem);

        readFunctionHolder3->release();

        std::cout << "before reading: " << readFunctionHolder3->getValuesString() << std::endl;

        readFunctionHolder3->readReference();

        std::cout << "check: " << readFunctionHolder3->getValuesString() << " = " << functionHolder3->getValuesString() << std::endl;

        assert(readFunctionHolder3->getValuesString().compare(functionHolder3->getValuesString()) == 0);



        // Testing individual Functions/Operations

        shared_ptr<XdmfArray> valArray1 = XdmfArray::New();
        valArray1->pushBack(-0.5);
        shared_ptr<XdmfArray> valArray2 = XdmfArray::New();
        valArray2->pushBack(-0.25);
        shared_ptr<XdmfArray> valArray3 = XdmfArray::New();
        valArray3->pushBack((double)2);

        std::map<std::string, shared_ptr<XdmfArray> > testVals;

        testVals["A"] = valArray1;
        testVals["B"] = valArray2;
        testVals["C"] = valArray3;

        shared_ptr<XdmfArray> functionResult;
        double checkResult;
        double compareResult;

        // Trig functions have issues with being consistent to the degree of 10^-16
        // Ensure that the functions are accurate to the degree of 10^-15

        printf("Accepted Tolerance 10^15\n");

        printf("ABS(-0.5) = %lf\n", std::abs((double)-0.5));
        checkResult = std::abs((double)-0.5);
        functionResult = XdmfFunction::evaluateExpression("ABS(A)", testVals);
        compareResult = functionResult->getValue<double>(0);
        checkResult = floor(checkResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        compareResult = floor(compareResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        printf("array contains: %s\n", functionResult->getValuesString().c_str());
        printf("%.15lf ?= %.15lf\n", checkResult, compareResult);
        assert(checkResult == compareResult);

        printf("ARCCOS(-0.5) = %lf\n", std::acos((double)-0.5));
        checkResult = std::acos((double)-0.5);
        functionResult = XdmfFunction::evaluateExpression("ACOS(A)", testVals);
        compareResult = functionResult->getValue<double>(0);
        checkResult = floor(checkResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        compareResult = floor(compareResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        printf("array contains: %s\n", functionResult->getValuesString().c_str());
        printf("%.15lf ?= %.15lf\n", checkResult, compareResult);
        assert(checkResult == compareResult);

        printf("ARCSIN(-0.5) = %lf\n", std::asin((double)-0.5));
        checkResult = std::asin((double)-0.5);
        functionResult = XdmfFunction::evaluateExpression("ASIN(A)", testVals);
        compareResult = functionResult->getValue<double>(0);
        checkResult = floor(checkResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        compareResult = floor(compareResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        printf("array contains: %s\n", functionResult->getValuesString().c_str());
        printf("%.15lf ?= %.15lf\n", checkResult, compareResult);
        assert(checkResult == compareResult);

        printf("ARCTAN(-0.5) = %lf\n", std::atan((double)-0.5));
        checkResult = std::atan((double)-0.5);
        functionResult = XdmfFunction::evaluateExpression("ATAN(A)", testVals);
        compareResult = functionResult->getValue<double>(0);
        checkResult = floor(checkResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        compareResult = floor(compareResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        printf("array contains: %s\n", functionResult->getValuesString().c_str());
        printf("%.15lf ?= %.15lf\n", checkResult, compareResult);
        assert(checkResult == compareResult);

        printf("EXP(-0.5, 2) = %lf\n", std::pow((double)-0.5, 2));
        checkResult = std::pow((double)-0.5, 2);
        functionResult = XdmfFunction::evaluateExpression("EXP(A, C)", testVals);
        compareResult = functionResult->getValue<double>(0);
        checkResult = floor(checkResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        compareResult = floor(compareResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        printf("array contains: %s\n", functionResult->getValuesString().c_str());
        printf("%.15lf ?= %.15lf\n", checkResult, compareResult);
        assert(checkResult == compareResult);

        printf("COS(-0.5) = %lf\n", std::cos((double)-0.5));
        checkResult = std::cos((double)-0.5);
        functionResult = XdmfFunction::evaluateExpression("COS(A)", testVals);
        compareResult = functionResult->getValue<double>(0);
        checkResult = floor(checkResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        compareResult = floor(compareResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        printf("array contains: %s\n", functionResult->getValuesString().c_str());
        printf("%.15lf ?= %.15lf\n", checkResult, compareResult);
        assert(checkResult == compareResult);

        printf("LOG(2) = %lf\n", std::log((double)2));
        checkResult = std::log((double)2);
        functionResult = XdmfFunction::evaluateExpression("LOG(C)", testVals);
        compareResult = functionResult->getValue<double>(0);
        checkResult = floor(checkResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        compareResult = floor(compareResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        printf("array contains: %s\n", functionResult->getValuesString().c_str());
        printf("%.15lf ?= %.15lf\n", checkResult, compareResult);
        assert(checkResult == compareResult);
        printf("LOG(2, 4) = %lf\n", std::log((double)2)/std::log((double)4));
        checkResult = std::log((double)2)/std::log((double)4);
        functionResult = XdmfFunction::evaluateExpression("LOG(C, 4)", testVals);
        compareResult = functionResult->getValue<double>(0);
        checkResult = floor(checkResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        compareResult = floor(compareResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        printf("array contains: %s\n", functionResult->getValuesString().c_str());
        printf("%.15lf ?= %.15lf\n", checkResult, compareResult);
        assert(checkResult == compareResult);

        printf("SQRT(2) = %lf\n", std::sqrt((double)2));
        checkResult = std::sqrt((double)2);
        functionResult = XdmfFunction::evaluateExpression("SQRT(C)", testVals);
        compareResult = functionResult->getValue<double>(0);
        checkResult = floor(checkResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        compareResult = floor(compareResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        printf("array contains: %s\n", functionResult->getValuesString().c_str());
        printf("%.15lf ?= %.15lf\n", checkResult, compareResult);
        assert(checkResult == compareResult);

        printf("SIN(-0.5) = %lf\n", std::sin((double)-0.5));
        checkResult = std::sin((double)-0.5);
        functionResult = XdmfFunction::evaluateExpression("SIN(A)", testVals);
        compareResult = functionResult->getValue<double>(0);
        checkResult = floor(checkResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        compareResult = floor(compareResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        printf("array contains: %s\n", functionResult->getValuesString().c_str());
        printf("%.15lf ?= %.15lf\n", checkResult, compareResult);
        assert(checkResult == compareResult);

        printf("TAN(-0.5) = %lf\n", std::tan((double)-0.5));
        checkResult = std::tan((double)-0.5);
        functionResult = XdmfFunction::evaluateExpression("TAN(A)", testVals);
        compareResult = functionResult->getValue<double>(0);
        checkResult = floor(checkResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        compareResult = floor(compareResult * pow((double)10, 15) + 0.5) / pow((double)10, 15);
        printf("array contains: %s\n", functionResult->getValuesString().c_str());
        printf("%.15lf ?= %.15lf\n", checkResult, compareResult);
        assert(checkResult == compareResult);

        printf("JOIN(A, B, C)\n");
        printf("array contains: %s\n", XdmfFunction::evaluateExpression("JOIN(A, B, C)", testVals)->getValuesString().c_str());
        assert(strcmp(XdmfFunction::evaluateExpression("JOIN(A, B, C)", testVals)->getValuesString().c_str(), "-0.5 -0.25 2") == 0);

        printf("A + B = %lf\n", -0.5 + -0.25);
        printf("array contains: %s\n", XdmfFunction::evaluateExpression("A+B", testVals)->getValuesString().c_str());
        assert(strcmp(XdmfFunction::evaluateExpression("A+B", testVals)->getValuesString().c_str(), "-0.75") == 0);

        printf("A - B = %lf\n", -0.5 - -0.25);
        printf("array contains: %s\n", XdmfFunction::evaluateExpression("A-B", testVals)->getValuesString().c_str());
        assert(strcmp(XdmfFunction::evaluateExpression("A-B", testVals)->getValuesString().c_str(), "-0.25") == 0);

        printf("A * B = %lf\n", -0.5 * -0.25);
        printf("array contains: %s\n", XdmfFunction::evaluateExpression("A*B", testVals)->getValuesString().c_str());
        assert(strcmp(XdmfFunction::evaluateExpression("A*B", testVals)->getValuesString().c_str(), "0.125") == 0);

        printf("A / B = %lf\n", -0.5 / -0.25);
        printf("array contains: %s\n", XdmfFunction::evaluateExpression("A/B", testVals)->getValuesString().c_str());
        assert(strcmp(XdmfFunction::evaluateExpression("A/B", testVals)->getValuesString().c_str(), "2") == 0);

	return 0;
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
		for (unsigned int i = 0; i < values.size(); ++i)
		{
			for (unsigned int j = 0; j < values[i]->getSize(); ++j)
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

shared_ptr<XdmfArray> prepend(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2)
{
	//joins into new array and returns it
	shared_ptr<XdmfArray> returnArray = XdmfArray::New();
	returnArray->insert(0, val2, 0, val2->getSize(),  1, 1);
	returnArray->insert(val2->getSize(), val1, 0, val1->getSize(), 1, 1);
	return returnArray;
}
