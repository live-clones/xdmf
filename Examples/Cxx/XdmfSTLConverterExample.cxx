#include "XdmfSTLConverter.txx"

#include <XdmfDOM.h>
#include <XdmfDomain.h>
#include <XdmfRoot.h>

#include <string>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <stack>

int main( int argc, const char* argv[] )
{
	XdmfSTLConverter * mySerializer = new XdmfSTLConverter();
	XdmfDOM * myDOM = new XdmfDOM();
	myDOM->SetFileName("output.xmf");

	XdmfRoot * myRoot = new XdmfRoot();
	myRoot->SetDOM(myDOM);
	myRoot->Build();

	XdmfDomain * myDomain = new XdmfDomain();
	myRoot->Insert(myDomain);

	std::vector<char> charVector;
	std::deque<char> charDeque;
	std::list<char> charList;
	std::set<char> charSet;
	char charTotal = 1;
	for (int i=1; i<6; i++)
	{
		charTotal += charTotal;
		charVector.push_back(charTotal);
		charDeque.push_back(charTotal);
		charList.push_back(charTotal);
		charSet.insert(charTotal);
	}

	std::vector<short> shortVector;
	std::deque<short> shortDeque;
	std::list<short> shortList;
	std::set<short> shortSet;
	short shortTotal = 10 ;
	for (int i=1; i<6; i++)
	{
		shortTotal += shortTotal;
		shortVector.push_back(shortTotal);
		shortDeque.push_back(shortTotal);
		shortList.push_back(shortTotal);
		shortSet.insert(shortTotal);
	}

	std::vector<int> intVector;
	std::deque<int> intDeque;
	std::list<int> intList;
	std::set<int> intSet;
	int intTotal = 100 ;
	for (int i=1; i<6; i++)
	{
		intTotal += intTotal;
		intVector.push_back(intTotal);
		intDeque.push_back(intTotal);
		intList.push_back(intTotal);
		intSet.insert(intTotal);
	}

	std::vector<long long> longlongVector;
	std::deque<long long> longlongDeque;
	std::list<long long> longlongList;
	std::set<long long> longlongSet;
	long long longlongTotal = 1000 ;
	for (int i=1; i<6; i++)
	{
		longlongTotal += longlongTotal;
		longlongVector.push_back(longlongTotal);
		longlongDeque.push_back(longlongTotal);
		longlongList.push_back(longlongTotal);
		longlongSet.insert(longlongTotal);
	}

	std::vector<float> floatVector;
	std::deque<float> floatDeque;
	std::list<float> floatList;
	std::set<float> floatSet;
	float floatTotal = 3.14159265358595 ;
	for (int i=1; i<6; i++)
	{
		floatTotal += floatTotal;
		floatVector.push_back(floatTotal);
		floatDeque.push_back(floatTotal);
		floatList.push_back(floatTotal);
		floatSet.insert(floatTotal);
	}

	std::vector<double> doubleVector;
	std::deque<double> doubleDeque;
	std::list<double> doubleList;
	std::set<double> doubleSet;
	double doubleTotal = 66.66666666666 ;
	for (int i=1; i<6; i++)
	{
		doubleTotal += doubleTotal;
		doubleVector.push_back(doubleTotal);
		doubleDeque.push_back(doubleTotal);
		doubleList.push_back(doubleTotal);
		doubleSet.insert(doubleTotal);
	}

	std::vector<unsigned char> unsignedcharVector;
	std::deque<unsigned char> unsignedcharDeque;
	std::list<unsigned char> unsignedcharList;
	std::set<unsigned char> unsignedcharSet;
	unsigned char unsignedcharTotal = 2;
	for (int i=1; i<6; i++)
	{
		unsignedcharTotal += unsignedcharTotal;
		unsignedcharVector.push_back(unsignedcharTotal);
		unsignedcharDeque.push_back(unsignedcharTotal);
		unsignedcharList.push_back(unsignedcharTotal);
		unsignedcharSet.insert(unsignedcharTotal);
	}

	std::vector<unsigned short> unsignedshortVector;
	std::deque<unsigned short> unsignedshortDeque;
	std::list<unsigned short> unsignedshortList;
	std::set<unsigned short> unsignedshortSet;
	unsigned short unsignedshortTotal = 20 ;
	for (int i=1; i<6; i++)
	{
		unsignedshortTotal += unsignedshortTotal;
		unsignedshortVector.push_back(unsignedshortTotal);
		unsignedshortDeque.push_back(unsignedshortTotal);
		unsignedshortList.push_back(unsignedshortTotal);
		unsignedshortSet.insert(unsignedshortTotal);
	}

	std::vector<unsigned int> unsignedintVector;
	std::deque<unsigned int> unsignedintDeque;
	std::list<unsigned int> unsignedintList;
	std::set<unsigned int> unsignedintSet;
	unsigned int unsignedintTotal = 200 ;
	for (int i=1; i<6; i++)
	{
		unsignedintTotal += unsignedintTotal;
		unsignedintVector.push_back(unsignedintTotal);
		unsignedintDeque.push_back(unsignedintTotal);
		unsignedintList.push_back(unsignedintTotal);
		unsignedintSet.insert(unsignedintTotal);
	}

	std::multiset<double> doubleMultiset;
	for (int i=1; i<6; i++)
	{
		doubleTotal += doubleTotal;
		doubleMultiset.insert(doubleTotal);
	}
	doubleMultiset.insert(doubleTotal);
	doubleMultiset.insert(doubleTotal);

	std::map<int,int> myMap;

	myMap[50] = 62;
	myMap[98] = 101;
	myMap[200] = 116;
	myMap[400] = 328;

	std::map<int, double> myMap2;
	myMap2[12]= 13.785412486;
	myMap2[60] = 45451.154987;
	myMap2[-60] = 5231.554874;

	std::map<double, int> myMap3;
	myMap3[50.1561564] = 69;
	myMap3[-52.1214487] = 99;
	myMap3[0.002346] = 1;

	std::multimap<int,int> myMultimap;
	myMultimap.insert(std::pair<int,int>(50, 62));
	myMultimap.insert(std::pair<int,int>(98, 101));
	myMultimap.insert(std::pair<int,int>(200, 116));
	myMultimap.insert(std::pair<int,int>(400, 328));
	myMultimap.insert(std::pair<int,int>(50, 86));
	myMultimap.insert(std::pair<int,int>(98, 72));
	myMultimap.insert(std::pair<int,int>(200, 85));
	myMultimap.insert(std::pair<int,int>(400, 328));

	std::stack<int> myStack;
	myStack.push(10);
	myStack.push(20);
	myStack.push(30);
	myStack.push(40);
	myStack.push(50);

	mySerializer->writeMapToXdmf(myMap, myDOM, "Map");
	mySerializer->writeMapToXdmf(myMap2, myDOM, "Map2");
	mySerializer->writeMapToXdmf(myMap3, myDOM, "Map3");
	mySerializer->writeMapToXdmf(myMultimap, myDOM, "MultiMap");

	mySerializer->writeSetToXdmf(charVector, myDOM, "charVector");
	mySerializer->writeSetToXdmf(shortVector, myDOM, "shortVector");
	mySerializer->writeSetToXdmf(intVector, myDOM, "intVector");
	mySerializer->writeSetToXdmf(longlongVector, myDOM, "longlongVector");
	mySerializer->writeSetToXdmf(floatVector, myDOM, "floatVector");
	mySerializer->writeSetToXdmf(doubleVector, myDOM, "doubleVector");
	mySerializer->writeSetToXdmf(unsignedcharVector, myDOM, "unsignedcharVector");
	mySerializer->writeSetToXdmf(unsignedshortVector, myDOM, "unsignedshortVector");
	mySerializer->writeSetToXdmf(unsignedintVector, myDOM, "unsignedintVector");

	mySerializer->writeSetToXdmf(charDeque, myDOM, "charDeque");
	mySerializer->writeSetToXdmf(shortDeque, myDOM, "shortDeque");
	mySerializer->writeSetToXdmf(intDeque, myDOM, "intDeque");
	mySerializer->writeSetToXdmf(longlongDeque, myDOM, "longlongDeque");
	mySerializer->writeSetToXdmf(floatDeque, myDOM, "floatDeque");
	mySerializer->writeSetToXdmf(doubleDeque, myDOM, "doubleDeque");
	mySerializer->writeSetToXdmf(unsignedcharDeque, myDOM, "unsignedcharDeque");
	mySerializer->writeSetToXdmf(unsignedshortDeque, myDOM, "unsignedshortDeque");
	mySerializer->writeSetToXdmf(unsignedintDeque, myDOM, "unsignedintDeque");

	mySerializer->writeSetToXdmf(charList, myDOM, "charList");
	mySerializer->writeSetToXdmf(shortList, myDOM, "shortList");
	mySerializer->writeSetToXdmf(intList, myDOM, "intList");
	mySerializer->writeSetToXdmf(longlongList, myDOM, "longlongList");
	mySerializer->writeSetToXdmf(floatList, myDOM, "floatList");
	mySerializer->writeSetToXdmf(doubleList, myDOM, "doubleList");
	mySerializer->writeSetToXdmf(unsignedcharList, myDOM, "unsignedcharList");
	mySerializer->writeSetToXdmf(unsignedshortList, myDOM, "unsignedshortList");
	mySerializer->writeSetToXdmf(unsignedintList, myDOM, "unsignedintList");

	mySerializer->writeSetToXdmf(doubleMultiset, myDOM, "doubleMultiset");

	mySerializer->writeSetToXdmf(charSet, myDOM, "charSet");
	mySerializer->writeSetToXdmf(shortSet, myDOM, "shortSet");
	mySerializer->writeSetToXdmf(intSet, myDOM, "intSet");
	mySerializer->writeSetToXdmf(longlongSet, myDOM, "longlongSet");
	mySerializer->writeSetToXdmf(floatSet, myDOM, "floatSet");
	mySerializer->writeSetToXdmf(doubleSet, myDOM, "doubleSet");
	mySerializer->writeSetToXdmf(unsignedcharSet, myDOM, "unsignedcharSet");
	mySerializer->writeSetToXdmf(unsignedshortSet, myDOM, "unsignedshortSet");
	mySerializer->writeSetToXdmf(unsignedintSet, myDOM, "unsignedintSet");

	cout << myDOM->Serialize() << endl;
	//myDOM->Write("output.xmf");

	// Now attempt to do the reverse... xdmf to stl objects:
	XdmfXmlNode domain = myDOM->FindElement("Domain");
	for (int i=0; i < myDOM->FindNumberOfElements("Set", domain); i++)
	{
	    XdmfSet * currSet = new XdmfSet();
	    currSet->SetDOM(myDOM);
	    currSet->SetElement(myDOM->FindElement("Set", i, domain));
	    currSet->Update();
	    if (currSet->GetNumberOfAttributes() == 0)
	    {
	    	std::multiset<int> myData;
	    	mySerializer->getSetFromXdmf(myData, currSet);
		    cout << myData.size() << endl;
	    }
	    else
	    {
	    	std::map<int,int> myData;
	    	mySerializer->getMapFromXdmf(myData, currSet);
		    cout << myData.size() << endl;
	    }
	}
}
