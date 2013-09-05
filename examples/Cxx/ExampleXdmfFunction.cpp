#include "XdmfArray.hpp"
#include "XdmfArrayType.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfFunction.hpp"
#include <vector>
#include <map>

//#declarefunction begin

shared_ptr<XdmfArray> maximum(std::vector<shared_ptr<XdmfArray> > values);

//#declarefunction end

//#declareoperation begin

shared_ptr<XdmfArray> prepend(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2);

//#declareoperation end

//#declareoperationclass begin

class OperationHolder : public XdmfFunction::XdmfOperationInternal
{
        public:
                static shared_ptr<OperationHolder> New()
                {
                        shared_ptr<OperationHolder> obj (new OperationHolder());
                        return obj;
                }

                virtual shared_ptr<XdmfArray> execute(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2)
                {
                        // returns whichever array has more values
                        if (val1->getSize() >= val2->getSize())
                        {
                                return val1;
                        }
                        else
                        {
                                return val2;
                        }
                }

        private:
                OperationHolder()
                {
                }

}

//#declareoperationclass end

//#declarefunctionclass begin

class FunctionHolder : public XdmfFunction::XdmfFunctionInternal
{
        public:
                static shared_ptr<FunctionHolder> New()
                {
                        shared_ptr<FunctionHolder> obj (new FunctionHolder());
                        return obj;
                }

                virtual shared_ptr<XdmfArray> execute(std::vector<shared_ptr<XdmfArray> > valueVector)
                {
                        // Simply returns an array containing 0
                        shared_ptr<XdmfArray> zeroArray = XdmfArray::New();
                        zeroArray.pushBack<int>(0);
                }

        private:
                FunctionHolder()
                {
                }
}

//#declarefunctionclass end

//#programstart begin

int main(int, char **)
{

        //#programstart end

        //#getSupportedOperations begin

        std::string exampleOperations = XdmfFunction::getSupportedOperations();

        //#getSupportedOperations end

        //#getSupportedFunctions begin

        std::vector<std::string> exampleFunctions = XdmfFunction::getSupportedFunctions();

        //#getSupportedFunctions end

        //#getValidVariableChars begin

        std::string exampleVariableChars = XdmfFunction::getValidVariableChars();

        //#getValidVariableChars end

        //#getValidDigitChars begin

        std::string exampleDigitChars = XdmfFunction::getValidDigitChars();

        //#getValidDigitChars end

        //#getOperationPriority begin

        int examplePriority = XdmfFunction::getOperationPriority('|');

        //#getOperationPriority end

        //#valueinit begin

        shared_ptr<XdmfArray> valueArray1 = XdmfArray::New();
        valueArray1->pushBack(1);
        valueArray1->pushBack(2);
        valueArray1->pushBack(3);
        valueArray1->pushBack(4);
        shared_ptr<XdmfArray> valueArray2 = XdmfArray::New();
        valueArray2->pushBack(9);
        valueArray2->pushBack(8);
        valueArray2->pushBack(7);
        valueArray2->pushBack(6);

        std::vector<shared_ptr<XdmfArray> > valueVector;
        valueVector.push_back(valueArray1);
        valueVector.push_back(valueArray2);

        shared_ptr<XdmfArray> answerArray;

        //#valueinit end

        //#sum begin

        answerArray = XdmfFunction::sum(valueVector);

        //#sum end

        //#average begin

        answerArray = XdmfFunction::average(valueVector);

        //#average end

        //#chunk begin

        answerArray = XdmfFunction::chunk(valueArray1, valueArray2);

        //#chunk end

        //#interlace begin

        answerArray = XdmfFunction::interlace(valueArray1, valueArray2);

        //#interlace end

        //#addOperation begin

        int exampleNumOperations = XdmfFunction::addOperation('@', (shared_ptr<XdmfArray>(*)(shared_ptr<XdmfArray>, shared_ptr<XdmfArray>))prepend, 2);

        //#addOperation end

        //#addOperationclass begin

        shared_ptr<OperationHolder> newOperation = OperationHolder::New();
        int numOperations = XdmfFunction::addOperation('>', newOperation);

        //#addOperationclass end

        //#evaluateOperation begin

        answerArray = XdmfFunction::evaluateOperation(valueArray1, valueArray2, '@');

        //#evaluateOperation end

        //#addFunction begin

        int exampleNumFunctions = XdmfFunction::addFunction("MAX", (shared_ptr<XdmfArray>(*)(std::vector<shared_ptr<XdmfArray> >))maximum);

        //#addFunction end

        //#addFunctionclass begin

        shared_ptr<FunctionHolder> newFunction = FunctionHolder::New();
        int numFunctions = XdmfFunction::addFunction("ZERO", newFunction);

        //#addFunctionclass end

        //#evaluateFunction begin

        answerArray = XdmfFunction::evaluateFunction(valueVector, "MAX");

        //#evaluateFunction end

        //#evaluateExpression begin

        std::map<std::string, shared_ptr<XdmfArray> > valueMap;
        valueMap["A"] = valueArray1;
        valueMap["B"] = valueArray2;

        std::string parsedExpression = "MAX(A,B)@(A#B)";
        answerArray = XdmfFunction::evaluateExpression(parsedExpression, valueMap);

        //#evaluateExpression end

        //#initialization begin

        shared_ptr<XdmfFunction> testFunction = XdmfFunction::New();

        //#initalization end

        //#initexpression begin

        std::string functionExpression = "AVE(A)";
        shared_ptr<XdmfArray> variableArray = XdmfArray::New();

        for (unsigned int i = 0; i < 10; ++i)
        {
                variableArray->pushBack(i);
        }

        std::map<std::string, shared_ptr<XdmfArray> > variableMap;
        variableMap["A"] = variableArray;

        shared_ptr<XdmfFunction> exampleFunction = XdmfFunction::New(functionExpression, variableMap);

        //#initexpression end

        //#setExpression begin

        std::string newExpression = "SUM(A)";
        exampleFunction->setExpression(newExpression);

        //#setExpression end

        //#getExpression begin

        std::string exampleExpression = exampleFunction->getExpression();

        //#getExpression end

        //#insertVariable begin

        shared_ptr<XdmfArray> secondVariableArray = XdmfArray::New();

        for (unsigned int i = 0; i < 10; ++i)
        {
                secondVariableArray->pushBack(i);
        }

        exampleFunction->insertVariable("B", secondVariableArray);

        //#insertVariable end

        //#getVariable begin

        shared_ptr<XdmfArray> variableValue = exampleFunction->getVariable("B");

        //#getVariable end

        //#getVariableList begin

        std::vector<std::string> exampleVarList = exampleFunction->getVariableList();

        //#getVariableList end

        //#removeVariable begin

        exampleFunction->removeVariable("B");

        //#removeVariable end

        //#setConstructedType begin

        shared_ptr<XdmfAttribute> typeAttribute = XdmfAttribute::New();

        exampleFunction->setConstructedType(typeAttribute->getItemTag());

        //#setConstructedType end

        //#getConstructedType begin

        std::string exampleType = exampleFunction->getConstructedType();

        //#getConstructedType end

        //#setConstructedProperties begin

        shared_ptr<XdmfAttribute> propertyAttribute = XdmfAttribute::New();
        exampleFunction->setConstructedProperties(propertyAttribute->getItemProperties());

        //#setConstructedProperties end

        //#getConstructedProperties begin

        std::map<std::string, std::string> exampleProperties = exampleFunction->getConstructedProperties();

        //#getConstructedProperties end

        //#read begin

        shared_ptr<XdmfArray> functionResult = exampleFunction->read();

        //#read end

        //#programend start

        return 0;
}

//#programend end

//#definefunction begin

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
                for (int i = 0; i < values.size(); ++i)
                {
                        for (int j = 0; j < values[i]->getSize(); ++j)
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

//#definefunction end

//#defineoperation begin

shared_ptr<XdmfArray> prepend(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2)
{
        //joins into new array and returns it
        shared_ptr<XdmfArray> returnArray = XdmfArray::New();
        returnArray->insert(0, val2, 0, val2->getSize(),  1, 1);
        returnArray->insert(val2->getSize(), val1, 0, val1->getSize(), 1, 1);
        return returnArray;
}

//#defineoperation end
