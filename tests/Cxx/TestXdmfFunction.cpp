#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfFunction.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfWriter.hpp"
#include "XdmfReader.hpp"
#include <map>
#include <iostream>
#include <cmath>
#include <string.h>
#include <stdio.h>

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

	assert(readItem->getItemTag().compare("Attribute") == 0);

	shared_ptr<XdmfAttribute> readFunctionHolder = shared_dynamic_cast<XdmfAttribute>(readItem);

	readFunctionHolder->readReference();

	assert(readFunctionHolder->getValuesString().compare(functionHolder->getValuesString()) == 0);

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

        assert(strcmp(XdmfFunction::evaluateExpression("ABS(A)", testVals)->getValuesString().c_str(), "0.5") == 0);

        printf("ARCCOS(-0.5) = %lf\n", std::acos((double)-0.5));
        printf("array contains: %s\n", XdmfFunction::evaluateExpression("ARCCOS(A)", testVals)->getValuesString().c_str());
        assert(strcmp(XdmfFunction::evaluateExpression("ARCCOS(A)", testVals)->getValuesString().c_str(), "2.0943951023931957") == 0);

        printf("ARCSIN(-0.5) = %lf\n", std::asin((double)-0.5));
        printf("array contains: %s\n", XdmfFunction::evaluateExpression("ARCSIN(A)", testVals)->getValuesString().c_str());
        assert(strcmp(XdmfFunction::evaluateExpression("ARCSIN(A)", testVals)->getValuesString().c_str(), "-0.52359877559829893") == 0);

        printf("ARCTAN(-0.5) = %lf\n", std::atan((double)-0.5));
        printf("array contains: %s\n", XdmfFunction::evaluateExpression("ARCTAN(A)", testVals)->getValuesString().c_str());
        assert(strcmp(XdmfFunction::evaluateExpression("ARCTAN(A)", testVals)->getValuesString().c_str(), "-0.46364760900080609") == 0);

        printf("EXP(-0.5, 2) = %lf\n", std::pow((double)-0.5, 2));
        printf("array contains: %s\n", XdmfFunction::evaluateExpression("EXP(A, C)", testVals)->getValuesString().c_str());
        assert(strcmp(XdmfFunction::evaluateExpression("EXP(A, C)", testVals)->getValuesString().c_str(), "0.25") == 0);

        printf("COS(-0.5) = %lf\n", std::cos((double)-0.5));
        printf("array contains: %s\n", XdmfFunction::evaluateExpression("COS(A)", testVals)->getValuesString().c_str());
        assert(strcmp(XdmfFunction::evaluateExpression("COS(A)", testVals)->getValuesString().c_str(), "0.87758256189037276") == 0);

        printf("LOG(2) = %lf\n", std::log((double)2));
        printf("array contains: %s\n", XdmfFunction::evaluateExpression("LOG(C)", testVals)->getValuesString().c_str());
        assert(strcmp(XdmfFunction::evaluateExpression("LOG(C)", testVals)->getValuesString().c_str(), "0.69314718055994529") == 0);
        printf("LOG(2, 4) = %lf\n", std::log((double)2)/std::log(4));
        printf("array contains: %s\n", XdmfFunction::evaluateExpression("LOG(C, 4)", testVals)->getValuesString().c_str());
        assert(strcmp(XdmfFunction::evaluateExpression("LOG(C, 4)", testVals)->getValuesString().c_str(), "0.5") == 0);

        printf("SQRT(2) = %lf\n", std::sqrt((double)2));
        printf("array contains: %s\n", XdmfFunction::evaluateExpression("SQRT(C)", testVals)->getValuesString().c_str());
        assert(strcmp(XdmfFunction::evaluateExpression("SQRT(C)", testVals)->getValuesString().c_str(), "1.4142135623730951") == 0);

        printf("SIN(-0.5) = %lf\n", std::sin((double)-0.5));
        printf("array contains: %s\n", XdmfFunction::evaluateExpression("SIN(A)", testVals)->getValuesString().c_str());
        assert(strcmp(XdmfFunction::evaluateExpression("SIN(A)", testVals)->getValuesString().c_str(), "-0.47942553860420301") == 0);

        printf("TAN(-0.5) = %lf\n", std::tan((double)-0.5));
        printf("array contains: %s\n", XdmfFunction::evaluateExpression("TAN(A)", testVals)->getValuesString().c_str());
        assert(strcmp(XdmfFunction::evaluateExpression("TAN(A)", testVals)->getValuesString().c_str(), "-0.54630248984379048") == 0);

        printf("JOIN(A, B, C)");
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
