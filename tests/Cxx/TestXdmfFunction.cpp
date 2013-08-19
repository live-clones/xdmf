#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfFunction.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfWriter.hpp"
#include "XdmfReader.hpp"
#include <map>
#include <iostream>

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

	functionHolder->setFunction(testFunction);

	functionHolder->setWriteAsFunction(true);



	shared_ptr<XdmfWriter> functionWriter = XdmfWriter::New("function.xmf");

	functionHolder->accept(functionWriter);




	functionHolder->readFunction();

	std::cout << functionHolder->getValuesString() << std::endl;

	shared_ptr<XdmfReader> functionReader = XdmfReader::New();

	shared_ptr<XdmfItem> readItem = functionReader->read("function.xmf");

	assert(readItem->getItemTag().compare("Attribute") == 0);

	shared_ptr<XdmfAttribute> readFunctionHolder = shared_dynamic_cast<XdmfAttribute>(readItem);

	readFunctionHolder->readFunction();

	assert(readFunctionHolder->getValuesString().compare(functionHolder->getValuesString()) == 0);

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
