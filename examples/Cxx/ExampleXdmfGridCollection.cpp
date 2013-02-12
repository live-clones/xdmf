#include "XdmfGridCollection.hpp"
#include "XdmfGridCollectionType.hpp"
#include "XdmfInformation.hpp"

int main(int, char **)
{
	//Assuming that exampleCollection is a shared pointer to an XdmfGridCollection object

	shared_ptr<XdmfGridCollection> exampleCollection = XdmfGridCollection::New();

	exampleCollection->setType(XdmfGridCollectionType::Temporal());

	shared_ptr<const XdmfGridCollectionType> exampleType = exampleCollection->getType();

	if (exampleType == XdmfGridCollectionType::Temporal())
	{
		//do whatever is to be done if the grid collection is temporal
	}

	shared_ptr<XdmfInformation> exampleInformation = XdmfInformation::New();
	std::string newKey = "New Key";
	std::string newValue = "New Value";
	exampleInformation->setKey(newKey);
	exampleInformation->setValue(newValue);
	exampleCollection->insert(exampleInformation);

	return 0;
}
