#include <iostream>
#include <stdlib.h>
#include <XdmfArray.hpp>
#include <XdmfArrayType.hpp>
#include <XdmfWriter.hpp>
#include <XdmfDomain.hpp>
#include <XdmfUnstructuredGrid.hpp>
#include <XdmfGeometry.hpp>
#include <XdmfTopology.hpp>

int main(int, char **)
{
	
	shared_ptr<XdmfArray> writtenArray = XdmfArray::New();
	for (int i = 0; i < 20; i++)
	{
		writtenArray->pushBack(i);
	}
	shared_ptr<XdmfWriter> arrayWriter = XdmfWriter::New("arraydata.xmf");
	arrayWriter->setLightDataLimit(5);
	writtenArray->accept(arrayWriter);
	

	/*
	shared_ptr<XdmfDomain> primaryDomain = XdmfDomain::New();
	shared_ptr<XdmfUnstructuredGrid> testGrid = XdmfUnstructuredGrid::New();
	primaryDomain->insert(testGrid);
	shared_ptr<XdmfGeometry> testGeometry = XdmfGeometry::New();
	for (int i = 0; i < 12; i++)
	{
		testGeometry->pushBack(i);
	}
	testGrid->setGeometry(testGeometry);
	shared_ptr<XdmfTopology> testTopology = XdmfTopology::New();
	testGrid->setTopology(testTopology);

	shared_ptr<XdmfWriter> arrayWriter = XdmfWriter::New("array.xmf");
	primaryDomain->accept(arrayWriter);
	*/

	return 0;
}
