#include <iostream>
#include <stdlib.h>
#include <XdmfArray.hpp>
#include <XdmfArrayType.hpp>
#include <XdmfWriter.hpp>
#include <XdmfHDF5Writer.hpp>
#include <XdmfHDF5Controller.hpp>

int main(int, char **)
{
	
	shared_ptr<XdmfArray> writtenArray = XdmfArray::New();
	for (int i = 0; i < 600000; i++)
	{
		writtenArray->pushBack(i);
	}
	std::vector<unsigned int> starts;
	std::vector<unsigned int> strides;
	std::vector<unsigned int> dimensions;
	std::vector<unsigned int> dataspaces;
	starts.push_back(0);
	strides.push_back(6);
	dimensions.push_back(100000);
	dataspaces.push_back(600000);
	shared_ptr<XdmfHDF5Controller> arrayController = XdmfHDF5Controller::New("arraydata.h5", "Data", XdmfArrayType::Int32(), starts, strides, dimensions, dataspaces);
	writtenArray->insert(arrayController);
	starts[0] = 1;
	arrayController = XdmfHDF5Controller::New("arraydata.h5", "Data", XdmfArrayType::Int32(), starts, strides, dimensions, dataspaces);
	writtenArray->insert(arrayController);
        starts[0] = 2;
        arrayController = XdmfHDF5Controller::New("arraydata.h5", "Data", XdmfArrayType::Int32(), starts, strides, dimensions, dataspaces);
	writtenArray->insert(arrayController);
        starts[0] = 3;
        arrayController = XdmfHDF5Controller::New("arraydata.h5", "Data", XdmfArrayType::Int32(), starts, strides, dimensions, dataspaces);
	writtenArray->insert(arrayController);
        starts[0] = 4;
        arrayController = XdmfHDF5Controller::New("arraydata.h5", "Data", XdmfArrayType::Int32(), starts, strides, dimensions, dataspaces);
	writtenArray->insert(arrayController);
        starts[0] = 5;
	arrayController = XdmfHDF5Controller::New("arraydata.h5", "Data", XdmfArrayType::Int32(), starts, strides, dimensions, dataspaces);
        writtenArray->insert(arrayController);
	shared_ptr<XdmfHDF5Writer> arrayHeavyWriter = XdmfHDF5Writer::New("arraydata.h5");
	arrayHeavyWriter->setFileSizeLimit(1);
	//arrayHeavyWriter->setAllowSetSplitting(true);
	arrayHeavyWriter->setMode(XdmfHDF5Writer::Hyperslab);
	shared_ptr<XdmfWriter> arrayWriter = XdmfWriter::New("arraydata.xmf", arrayHeavyWriter);
	arrayWriter->setLightDataLimit(5);
	writtenArray->accept(arrayWriter);

	writtenArray->release();
	writtenArray->read();
	//printf("%s\n", writtenArray->getValuesString());
	printf("array size = %d\n", writtenArray->getSize());
	for (int i = 0; i < writtenArray->getSize(); i++)
        {
                if (i != writtenArray->getValue<int>(i))
		{
			printf("%d doesn't match %d\n", i, writtenArray->getValue<int>(i));
		}
        }

	return 0;
}
