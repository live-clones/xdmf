#include "XdmfGridCollection.hpp"
#include "XdmfGridCollectionType.hpp"
#include "XdmfInformation.hpp"

int main(int, char **)
{
	//Assuming that exampleCollection is a shared pointer to an XdmfGridCollection object

	//#initalization begin

	shared_ptr<XdmfGridCollection> exampleCollection = XdmfGridCollection::New();

	//#initialization end

	//#setType begin

	exampleCollection->setType(XdmfGridCollectionType::Temporal());

	//#setType end

	//#getType begin

	shared_ptr<const XdmfGridCollectionType> exampleType = exampleCollection->getType();

	if (exampleType == XdmfGridCollectionType::Temporal())
	{
		//do whatever is to be done if the grid collection is temporal
	}

	//#getType end

	//#insert begin

	shared_ptr<XdmfInformation> exampleInformation = XdmfInformation::New();
	std::string newKey = "New Key";
	std::string newValue = "New Value";
	exampleInformation->setKey(newKey);
	exampleInformation->setValue(newValue);
	exampleCollection->insert(exampleInformation);

	//#insert end

	return 0;
}
