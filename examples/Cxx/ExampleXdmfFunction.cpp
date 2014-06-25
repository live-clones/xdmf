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

        //#abs begin

        shared_ptr<XdmfArray> toBeAbs = XdmfArray::New();
        toBeAbs->pushBack<double>(-5.5);
        std::vector<shared_ptr<XdmfArray> > absVector;
        absVector.push_back(toBeAbs);
        shared_ptr<XdmfArray> absResult = XdmfFunction::abs(absVector);
        assert(absResult->getValuesString().compare("5.5") == 0);

        //#abs end

        //#arcsin begin

        shared_ptr<XdmfArray> toBeArcsin = XdmfArray::New();
        toBeArcsin->pushBack<double>(-0.5);
        std::vector<shared_ptr<XdmfArray> > arcsinVector;
        arcsinVector.push_back(toBeArcsin);
        shared_ptr<XdmfArray> arcsinResult = XdmfFunction::arcsin(arcsinVector);
        assert(arcsinResult->getValuesString().compare("-0.52359877559829893") == 0);

        //#arcsin end

        //#arccos begin

        shared_ptr<XdmfArray> toBeArccos = XdmfArray::New();
        toBeArccos->pushBack<double>(-0.5);
        std::vector<shared_ptr<XdmfArray> > arccosVector;
        arccosVector.push_back(toBeArccos);
        shared_ptr<XdmfArray> arccosResult = XdmfFunction::arccos(arccosVector);
        assert(arccosResult->getValuesString().compare("2.0943951023931957") == 0);

        //#arccos end

        //#cos begin

        shared_ptr<XdmfArray> toBeCos = XdmfArray::New();
        toBeCos->pushBack<double>(-0.5);
        std::vector<shared_ptr<XdmfArray> > cosVector;
        cosVector.push_back(toBeCos);
        shared_ptr<XdmfArray> cosResult = XdmfFunction::cos(cosVector);
        assert(cosResult->getValuesString().compare("0.87758256189037276") == 0);

        //#cos end

        //#sin begin

        shared_ptr<XdmfArray> toBeSin = XdmfArray::New();
        toBeSin->pushBack<double>(-0.5);
        std::vector<shared_ptr<XdmfArray> > sinVector;
        sinVector.push_back(toBeSin);
        shared_ptr<XdmfArray> sinResult = XdmfFunction::sin(sinVector);
        assert(sinResult->getValuesString().compare("-0.47942553860420301") == 0);

        //#sin end

        //#tan begin

        shared_ptr<XdmfArray> toBeTan = XdmfArray::New();
        toBeTan->pushBack<double>(-0.5);
        std::vector<shared_ptr<XdmfArray> > tanVector;
        tanVector.push_back(toBeTan);
        shared_ptr<XdmfArray> tanResult = XdmfFunction::tan(tanVector);
        assert(tanResult->getValuesString().compare("-0.54630248984379048") == 0);

        //#tan end

        //#sqrt begin

        shared_ptr<XdmfArray> toBeSqrt = XdmfArray::New();
        toBeSqrt->pushBack<double>(2);
        std::vector<shared_ptr<XdmfArray> > sqrtVector;
        sqrtVector.push_back(toBeSqrt);
        shared_ptr<XdmfArray> sqrtResult = XdmfFunction::sqrt(sqrtVector);
        assert(sqrtResult->getValuesString().compare("1.4142135623730951") == 0);

        //#sqrt end

        //#log begin

        shared_ptr<XdmfArray> toBeLog = XdmfArray::New();
        toBeLog->pushBack<double>(2);
        shared_ptr<XdmfArray> logBase = XdmfArray::New();
        logBase->pushBack<double>(4);
        std::vector<shared_ptr<XdmfArray> > logVector;
        logVector.push_back(toBeLog);
        logVector.push_back(logBase);
        shared_ptr<XdmfArray> logResult = XdmfFunction::log(logVector);
        assert(logResult->getValuesString().compare("0.5") == 0);

        //#log end

        //#exp begin

        shared_ptr<XdmfArray> powerBase = XdmfArray::New();
        powerBase->pushBack<double>(-0.5);
        shared_ptr<XdmfArray> powerFactor = XdmfArray::New();
        powerFactor->pushBack<double>(2);
        std::vector<shared_ptr<XdmfArray> > expVector;
        expVector.push_back(powerBase);
        expVector.push_back(powerFactor);
        shared_ptr<XdmfArray> expResult = XdmfFunction::exponent(expVector);
        assert(expResult->getValuesString().compare("0.25") == 0);

        //#exp end

        //#join begin

        shared_ptr<XdmfArray> array1 = XdmfArray::New();
        array1->pushBack<double>(-0.5);
        shared_ptr<XdmfArray> array2 = XdmfArray::New();
        array2->pushBack<double>(2);
        std::vector<shared_ptr<XdmfArray> > joinVector;
        joinVector.push_back(array1);
        joinVector.push_back(array2);
        shared_ptr<XdmfArray> joinResult = XdmfFunction::join(joinVector);
        assert(joinResult->getValuesString().compare("-0.5 2") == 0);

        //#join end

        //#addition begin

        shared_ptr<XdmfArray> addArray1 = XdmfArray::New();
        addArray1->pushBack<double>(-0.5);
        shared_ptr<XdmfArray> addArray2 = XdmfArray::New();
        addArray2->pushBack<double>(2);
        shared_ptr<XdmfArray> addResult = XdmfFunction::addition(addArray1, addArray2);
        assert(addResult->getValuesString().compare("1.5") == 0);

        //#addition end

        //#division begin

        shared_ptr<XdmfArray> divArray1 = XdmfArray::New();
        divArray1->pushBack<double>(-0.5);
        shared_ptr<XdmfArray> divArray2 = XdmfArray::New();
        divArray2->pushBack<double>(2);
        shared_ptr<XdmfArray> divResult = XdmfFunction::division(divArray1, divArray2);
        assert(divResult->getValuesString().compare("-0.25") == 0);

        //#division end

        //#multiplication begin

        shared_ptr<XdmfArray> mulArray1 = XdmfArray::New();
        mulArray1->pushBack<double>(-0.5);
        shared_ptr<XdmfArray> mulArray2 = XdmfArray::New();
        mulArray2->pushBack<double>(2);
        shared_ptr<XdmfArray> mulResult = XdmfFunction::multiplication(mulArray1, mulArray2);
        assert(mulResult->getValuesString().compare("-1") == 0);

        //#multiplication end

        //#subtraction begin

        shared_ptr<XdmfArray> subArray1 = XdmfArray::New();
        subArray1->pushBack<double>(-0.5);
        shared_ptr<XdmfArray> subArray2 = XdmfArray::New();
        subArray2->pushBack<double>(2);
        shared_ptr<XdmfArray> subResult = XdmfFunction::subtraction(subArray1, subArray2);
        assert(subResult->getValuesString().compare("-2.5") == 0);

        //#subtraction end

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
