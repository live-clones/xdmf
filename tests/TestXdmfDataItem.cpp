#include "XdmfDataItem.hpp"

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfDataItem> dataItem = XdmfDataItem::New();
	int* intArray = new int[10];
	dataItem->setData(intArray);
}
