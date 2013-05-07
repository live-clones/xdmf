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
	for (int i = 0; i < 9000000; i++)
	{
		writtenArray->pushBack(i);
	}
	std::vector<unsigned int> starts;
	std::vector<unsigned int> strides;
	std::vector<unsigned int> dimensions;
	std::vector<unsigned int> dataspaces;
	starts.push_back(0);
	starts.push_back(0);
	strides.push_back(3);
	strides.push_back(3);
	dimensions.push_back(1000);
	dimensions.push_back(1000);
	dataspaces.push_back(3000);
	dataspaces.push_back(3000);
	shared_ptr<XdmfHDF5Controller> arrayController = XdmfHDF5Controller::New("arraydata.h5", "Data", XdmfArrayType::Int32(), starts, strides, dimensions, dataspaces);
	writtenArray->insert(arrayController);
	starts[0] = 1;
	arrayController = XdmfHDF5Controller::New("arraydata.h5", "Data", XdmfArrayType::Int32(), starts, strides, dimensions, dataspaces);
	writtenArray->insert(arrayController);
        starts[0] = 2;
        arrayController = XdmfHDF5Controller::New("arraydata.h5", "Data", XdmfArrayType::Int32(), starts, strides, dimensions, dataspaces);
	writtenArray->insert(arrayController);
	starts[1] = 1;
        starts[0] = 0;
        arrayController = XdmfHDF5Controller::New("arraydata.h5", "Data", XdmfArrayType::Int32(), starts, strides, dimensions, dataspaces);
	writtenArray->insert(arrayController);
        starts[0] = 1;
        arrayController = XdmfHDF5Controller::New("arraydata.h5", "Data", XdmfArrayType::Int32(), starts, strides, dimensions, dataspaces);
	writtenArray->insert(arrayController);
        starts[0] = 2;
	arrayController = XdmfHDF5Controller::New("arraydata.h5", "Data", XdmfArrayType::Int32(), starts, strides, dimensions, dataspaces);
        writtenArray->insert(arrayController);
	starts[1] = 2;
        starts[0] = 0;
        arrayController = XdmfHDF5Controller::New("arraydata.h5", "Data", XdmfArrayType::Int32(), starts, strides, dimensions, dataspaces);
        writtenArray->insert(arrayController);
        starts[0] = 1;
        arrayController = XdmfHDF5Controller::New("arraydata.h5", "Data", XdmfArrayType::Int32(), starts, strides, dimensions, dataspaces);
        writtenArray->insert(arrayController);
        starts[0] = 2;
        arrayController = XdmfHDF5Controller::New("arraydata.h5", "Data", XdmfArrayType::Int32(), starts, strides, dimensions, dataspaces);
        writtenArray->insert(arrayController);

	shared_ptr<XdmfHDF5Writer> arrayHeavyWriter = XdmfHDF5Writer::New("arraydata.h5");
	arrayHeavyWriter->setFileSizeLimit(10);
	arrayHeavyWriter->setAllowSetSplitting(true);
	arrayHeavyWriter->setMode(XdmfHDF5Writer::Hyperslab);
	shared_ptr<XdmfWriter> arrayWriter = XdmfWriter::New("arraydata.xmf", arrayHeavyWriter);
	arrayWriter->setLightDataLimit(5);
	writtenArray->accept(arrayWriter);

	writtenArray->release();
	writtenArray->read();
	std::cout << "after read" << std::endl;
	std::cout << "array size = " << writtenArray->getSize() << std::endl;
	assert(writtenArray->getSize() == 9000000);
	for (int i = 0; i < writtenArray->getSize(); i++)
        {
                if (i != writtenArray->getValue<int>(i))
		{
			std::cout << i << " doesn't match " << writtenArray->getValue<int>(i) << std::endl;
		}
		assert(i == writtenArray->getValue<int>(i));
        }

	return 0;
}
