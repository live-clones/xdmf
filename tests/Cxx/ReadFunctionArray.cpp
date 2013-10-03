#include <iostream>
#include <stdlib.h>
#include <XdmfArray.hpp>
#include <XdmfDomain.hpp>
#include <XdmfUnstructuredGrid.hpp>
#include <XdmfRectilinearGrid.hpp>
#include <XdmfGeometry.hpp>
#include <XdmfArrayType.hpp>
#include <XdmfReader.hpp>
#include <XdmfWriter.hpp>

shared_ptr<XdmfArray> maximum(std::vector<shared_ptr<XdmfArray> > values);

int main(int, char **)
{
	XdmfArray::addFunction("MAX", maximum);
	shared_ptr<XdmfReader> testReader = XdmfReader::New();
	/*
	shared_ptr<XdmfArray> readArray = shared_dynamic_cast<XdmfArray>(testReader->read("array.xmf"));
	printf("Array ocntains %s\n", readArray->getValuesString());
	*/
	shared_ptr<XdmfDomain> readDomain = shared_dynamic_cast<XdmfDomain>(testReader->read("array.xmf"));
	printf("pulling out unstructured grid\n");
	shared_ptr<XdmfRectilinearGrid> readGrid = readDomain->getRectilinearGrid(0);
	//printf("pulling out geometry\n");
	//shared_ptr<XdmfGeometry> readGeometry = readGrid->getGeometry();
	printf("pulling out dimensions\n");
	std::vector<shared_ptr<XdmfArray> > readDimensions = readGrid->getCoordinates();
	printf("Geometry contains %s\n", readDimensions[0]->getValuesString());
	shared_ptr<XdmfWriter> testWriter = XdmfWriter::New("arrayoutput.xmf");
	readGrid->accept(testWriter);
	//readArray->accept(testWriter);
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
		for (int i = 0; i < values.size(); i++)
		{
			for (int j = 0; j < values[i]->getSize(); j++)
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
