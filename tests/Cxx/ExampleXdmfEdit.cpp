#include "XdmfDomain.hpp"
#include "XdmfSystemUtils.hpp"
#include "XdmfWriter.hpp"
#include "XdmfReader.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfInformation.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfCurvilinearGrid.hpp"
#include "XdmfRectilinearGrid.hpp"
#include "XdmfRegularGrid.hpp"
#include "XdmfMap.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfGridCollectionType.hpp"
#include "XdmfAttributeType.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfSet.hpp"
#include "XdmfArray.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfTopology.hpp"
#include "string.h"

#include "XdmfTestDataGenerator.hpp"

int main(int, char **)
{
        shared_ptr<XdmfReader> exampleReader = XdmfReader::New();

        /*
        This is assuming that the read item is an XdmfDomain object
        */
        shared_ptr<XdmfDomain> primaryDomain = shared_dynamic_cast<XdmfDomain>(exampleReader->read("testoutput.xmf"));
        shared_ptr<XdmfInformation> outputInformation = primaryDomain->getInformation(0);
        std::cout << "Key:" << outputInformation->getKey().c_str() << "\nValue: " << outputInformation->getValue().c_str() << std::endl;
        std::cout << "The Domain's tag is: " << primaryDomain->getItemTag().c_str() << std::endl;

        shared_ptr<XdmfGridCollection> gridHolder = primaryDomain->getGridCollection(0);

        std::cout << "The Grid Collection's tag is: " << gridHolder->getItemTag().c_str() << std::endl;
        std::cout << "The Grid Collection's name is: " << gridHolder->getName().c_str()  << std::endl;
        outputInformation = gridHolder->getInformation(0);
        std::cout << "Key:" << outputInformation->getKey().c_str() <<"\nValue:" << outputInformation->getValue().c_str() << std::endl;

	std::map<std::string, std::string>::iterator outputwalker = gridHolder->getItemProperties().begin();
	for (;outputwalker!=gridHolder->getItemProperties().end(); ++outputwalker)
	{
		std::cout << (*outputwalker).first.c_str() << ": " << (*outputwalker).second.c_str() << std::endl;
	}
 
        if (gridHolder->getType() == XdmfGridCollectionType::Spatial())
	{
                std::cout << "This is a spatial grid collection" << std::endl;
	}
        else
	{
                std::cout << "This is not a spatial grid collection" << std::endl;
	}


	//loop controlling integers
	int i = 0;
	int j = 0;
	int k = 0;
	int m = 0;
	int task;
	int node;
	int remote;
	std::string blankstring = "";
	char* outstring = strdup(blankstring.c_str());
	shared_ptr<XdmfMap> readMap;
	std::map<int, std::map<int, std::set<int> > > taskIDMap;
	std::map<int, std::map<int, std::set<int> > >::iterator taskWalker;
	std::map<int, std::set<int> > nodeIDMap;
	std::map<int, std::set<int> >::iterator nodeWalker;
	std::set<int> remoteIDset;
	std::set<int>::iterator remoteWalker;

	for (i=0; i<gridHolder->getNumberMaps(); ++i)
	{
		readMap = gridHolder->getMap(i);
		if (!readMap->isInitialized())
		{
			readMap->read();
		}
		std::cout << "Map # " << i << std::endl;
		taskIDMap = readMap->getMap();
		for (taskWalker = taskIDMap.begin(); taskWalker!= taskIDMap.end(); ++taskWalker)
		{
			task = (*taskWalker).first;
			nodeIDMap = (*taskWalker).second;
			for (nodeWalker = nodeIDMap.begin(); nodeWalker != nodeIDMap.end(); ++nodeWalker)
			{
				node = (*nodeWalker).first;
				remoteIDset = (*nodeWalker).second;
				for (remoteWalker = remoteIDset.begin(); remoteWalker != remoteIDset.end(); ++remoteWalker)
				{
					remote = (*remoteWalker);
					std::cout << "taskID: " << task << "\tlocalnodeID: " << node << "\tremotenodeID: " << remote << std::endl;
				}
			}
		}
	}

        std::cout << "Unstructured Grid" << std::endl;
        shared_ptr<XdmfUnstructuredGrid> ungrid = gridHolder->getUnstructuredGrid(0);
        std::cout << "The Unstructured Grid's tag is: " << ungrid->getItemTag().c_str() << std::endl;
        std::cout << "The Unstructured Grid's name is: " << ungrid->getName().c_str() << std::endl;

	outputwalker = ungrid->getItemProperties().begin();
        for (;outputwalker!=ungrid->getItemProperties().end(); ++outputwalker)
        {
                std::cout << (*outputwalker).first.c_str() << ": " << (*outputwalker).second.c_str() << std::endl;
        }

        std::cout << "The Unstructured Grid's time is: " << ungrid->getTime()->getValue() << std::endl;
        i=0;
        for (i=0; i<ungrid->getNumberMaps(); ++i)
        {
                readMap = ungrid->getMap(i);
		if (!readMap->isInitialized())
		{
			readMap->read();
		}
                std::cout << "Map # " << i << std::endl;
                taskIDMap = readMap->getMap();
                for (taskWalker = taskIDMap.begin(); taskWalker!= taskIDMap.end(); ++taskWalker)
                {
                        task = (*taskWalker).first;
                        nodeIDMap = (*taskWalker).second;
                        for (nodeWalker = nodeIDMap.begin(); nodeWalker != nodeIDMap.end(); ++nodeWalker)
                        {
                                node = (*nodeWalker).first;
                                remoteIDset = (*nodeWalker).second;
                                for (remoteWalker = remoteIDset.begin(); remoteWalker != remoteIDset.end(); ++remoteWalker)
                                {
                                        remote = (*remoteWalker);
                                        std::cout << "taskID: " << task << "\tlocalnodeID: " << node << "\tremotenodeID: " << remote << std::endl;
                                }
                        }
                }
        }

	shared_ptr<XdmfSet> readSet;
	shared_ptr<XdmfAttribute> readAttribute;
	for (i=0; i < ungrid->getNumberSets(); ++i)
	{
		readSet = ungrid->getSet(i);
		if (!readSet->isInitialized())
		{
			readSet->read();
		}
		std::cout << "Set # " << i << std::endl;
		std::cout << readSet->getName().c_str() << std::endl;
		if (readSet->getType() == XdmfSetType::Node())
		{
			std::cout << "This set is a node" << std::endl;
		}
		else
		{
			std::cout << "This set is not a node" << std::endl;
		}
		outputInformation = readSet->getInformation(0);
		std::cout << "Key: " << outputInformation->getKey().c_str() << "\nValue: %s\n" << outputInformation->getValue().c_str() << std::endl;
		std::cout << readSet->getValuesString().c_str() << std::endl;
		for (j=0; j < readSet->getNumberAttributes(); ++j)
		{
			readAttribute = readSet->getAttribute(j);
			if (!readAttribute->isInitialized())
			{
				readAttribute->read();
			}
			std::cout << "Set Attribute # " << j << std::endl;
			std::cout << readAttribute->getName().c_str() << std::endl;
			if (readAttribute->getType() == XdmfAttributeType::Scalar())
			{
				std::cout << "This attribute is a scalar" << std::endl;
			}
			else
			{
				std::cout << "This attribute is not a scalar\n" << std::endl;
			}
			if (readAttribute->getCenter() == XdmfAttributeCenter::Node())
			{
				std::cout << "This attribute is a node\n" << std::endl;
			}
			else
			{
				std::cout << "This attribute is not a node\n" << std::endl;
			}
			std::cout << readAttribute->getValuesString().c_str() << std::endl;
		}
	}
	for (i = 0; i < ungrid->getNumberAttributes(); ++i)
	{
		readAttribute = ungrid->getAttribute(i);
		if (!readAttribute->isInitialized())
		{
			readAttribute->read();
		}
		std::cout << "Attribute # " << i << std::endl;
		std::cout << readAttribute->getName().c_str() << std::endl;
		if (readAttribute->getType() == XdmfAttributeType::Scalar())
		{
			std::cout << "This attribute is a scalar" << std::endl;
		}
		else
		{
			std::cout << "This attribute is not a scalar" << std::endl;
		}
		if (readAttribute->getCenter() == XdmfAttributeCenter::Node())
		{
			std::cout << "This attribute is a node" << std::endl;
		}
		else
		{
			std::cout << "This attribute is not a node" << std::endl;
		}
		std::cout << readAttribute->getValuesString().c_str() << std::endl;
	}

	std::cout << "Unstructured Topology" << std::endl;
	shared_ptr<XdmfTopology> untopology = ungrid->getTopology();
	if (!untopology->isInitialized())
	{
		untopology->read();
	}
	std::cout << "The topology's tag: " << untopology->getItemTag().c_str()  << std::endl;
	if (untopology->getType() == XdmfTopologyType::Hexahedron())
	{
		std::cout << "This topology is a hexahedron" << std::endl;
        }
	else
	{
		std::cout << "This topology is not a hexahedron" << std::endl;
	}
	std::cout << "Contains " << untopology->getNumberElements() << " elements" << std::endl;
	std::cout << "Contains the values: " << untopology->getValuesString().c_str() << std::endl;

	std::cout << "Unstructured Geometry" << std::endl;
	shared_ptr<XdmfGeometry> ungeometry = ungrid->getGeometry();
	if (!ungeometry->isInitialized())
	{
		ungeometry->read();
	}
	std::cout << "The geometry's tag: " << ungeometry->getItemTag().c_str() << std::endl;
	if (ungeometry->getType() == XdmfGeometryType::XYZ())
	{
		std::cout << "This geometry is XYZ" << std::endl;
	}
	else
	{
		std::cout << "This geometry is not XYZ" << std::endl;
	}
	outputInformation = ungeometry->getInformation(0);
	std::cout << "Key: " << outputInformation->getKey().c_str() << "\nValue: " << outputInformation->getValue().c_str() << std::endl;
	std::cout << "Contains " << ungeometry->getNumberPoints() << " points" << std::endl;
	std::cout << "Contains the values: " << ungeometry->getValuesString().c_str() << std::endl;





        std::cout << "Curvilinear Grid" << std::endl;
        shared_ptr<XdmfCurvilinearGrid> curvgrid = gridHolder->getCurvilinearGrid(0);
        std::cout << "The Curvilinear Grid's tag is: " << curvgrid->getItemTag().c_str() << std::endl;
        std::cout << "The Curvilinear Grid's name is: " << curvgrid->getName().c_str() << std::endl;
        outputwalker = curvgrid->getItemProperties().begin();
        for (;outputwalker!=curvgrid->getItemProperties().end(); ++outputwalker)
        {
                std::cout << (*outputwalker).first.c_str() << ": " << (*outputwalker).second.c_str() << std::endl;
        }
        outputInformation = curvgrid->getInformation(0);
        std::cout << "Key: " << outputInformation->getKey().c_str() << "\nValue: " << outputInformation->getValue().c_str() << std::endl;
        std::cout << "The Curvilinear Grid's time is: " << curvgrid->getTime()->getValue() << std::endl;
        for (i=0; i<curvgrid->getNumberMaps(); ++i)
        {
                readMap = curvgrid->getMap(i);
		if (!readMap->isInitialized())
		{
			readMap->read();
		}
                std::cout << "Map # " << i << std::endl;
                taskIDMap = readMap->getMap();
                for (taskWalker = taskIDMap.begin(); taskWalker!= taskIDMap.end(); ++taskWalker)
                {
                        task = (*taskWalker).first;
                        nodeIDMap = (*taskWalker).second;
                        for (nodeWalker = nodeIDMap.begin(); nodeWalker != nodeIDMap.end(); ++nodeWalker)
                        {
                                node = (*nodeWalker).first;
                                remoteIDset = (*nodeWalker).second;
                                for (remoteWalker = remoteIDset.begin(); remoteWalker != remoteIDset.end(); ++remoteWalker)
                                {
                                        remote = (*remoteWalker);
					std::cout << "taskID: " << task << "\tlocalnodeID: " << node << "\tremotenodeID: " << remote << std::endl;
                                }
                        }
                }
        }
        for (i=0; i < curvgrid->getNumberSets(); ++i)
        {
                readSet = curvgrid->getSet(i);
		if (!readSet->isInitialized());
		{
			readSet->read();
		}
                std::cout << "Set # " << i << std::endl;
                std::cout << readSet->getName().c_str() << std::endl;
                if (readSet->getType() == XdmfSetType::Node())
                {
                        std::cout << "This set is a node" << std::endl;
                }
                else
                {
                        std::cout << "This set is not a node" << std::endl;
                }
                outputInformation = readSet->getInformation(0);
                std::cout << "Key: " << outputInformation->getKey().c_str() << "\nValue: %s\n" << outputInformation->getValue().c_str() << std::endl;
                std::cout << readSet->getValuesString().c_str() << std::endl;
                for (j=0; j < readSet->getNumberAttributes(); ++j)
                {
                        readAttribute = readSet->getAttribute(j);
			if (!readAttribute->isInitialized())
			{
				readAttribute->read();
			}
                        std::cout << "Set Attribute # " << j << std::endl;
                        std::cout << readAttribute->getName().c_str() << std::endl;
                        if (readAttribute->getType() == XdmfAttributeType::Scalar())
                        {
                                std::cout << "This attribute is a scalar" << std::endl;
                        }
                        else
                        {
                                std::cout << "This attribute is not a scalar" << std::endl;
                        }
                        if (readAttribute->getCenter() == XdmfAttributeCenter::Node())
                        {
                                std::cout << "This attribute is a node" << std::endl;
                        }
                        else
                        {
                                std::cout << "This attribute is not a node" << std::endl;
                        }
                        std::cout << readAttribute->getValuesString().c_str() << std::endl;
                }
        }
        for (i = 0; i < curvgrid->getNumberAttributes(); ++i)
        {
                readAttribute = curvgrid->getAttribute(i);
		if (!readAttribute->isInitialized())
		{
			readAttribute->read();
		}
                std::cout << "Attribute # " << i << std::endl;
                std::cout << readAttribute->getName().c_str() << std::endl;
                if (readAttribute->getType() == XdmfAttributeType::Scalar())
                {
                        std::cout << "This attribute is a scalar" << std::endl;
                }
                else
                {
                        std::cout << "This attribute is not a scalar" << std::endl;
                }
                if (readAttribute->getCenter() == XdmfAttributeCenter::Node())
                {
                        std::cout << "This attribute is a node" << std::endl;
                }
                else
                {
                        std::cout << "This attribute is not a node" << std::endl;
                }
                std::cout << readAttribute->getValuesString().c_str() << std::endl;
        }

        std::cout << "Curvilinear Dimensions" << std::endl;
        shared_ptr<XdmfArray> curvdimensions = curvgrid->getDimensions();
	if (!curvdimensions->isInitialized())
	{
		curvdimensions->read();
	}
        std::cout << "The dimensions' tag: " << curvdimensions->getItemTag().c_str() << std::endl;
        std::cout << "Contains the values: " << curvdimensions->getValuesString().c_str() << std::endl;

        std::cout << "Curvilinear Geometry" << std::endl;
        shared_ptr<XdmfGeometry> curvgeometry = curvgrid->getGeometry();
	if (!curvgeometry->isInitialized())
	{
		curvgeometry->read();
	}
        std::cout << "The geometry's tag: " << curvgeometry->getItemTag().c_str() << std::endl;
        if (curvgeometry->getType() == XdmfGeometryType::XYZ())
	{
                std::cout << "This geometry is XYZ" << std::endl;
        }
	else
	{
                std::cout << "This geometry is not XYZ" << std::endl;
	}
        outputInformation = curvgeometry->getInformation(0);
        std::cout << "Key: " << outputInformation->getKey().c_str() << "\nValue: " << outputInformation->getValue().c_str() << std::endl;
        std::cout << "Contains " << curvgeometry->getNumberPoints() << " points" << std::endl;
        std::cout << "Contains the values: " << curvgeometry->getValuesString().c_str() << std::endl;


        std::cout << "Rectilinear Grid" << std::endl;
        shared_ptr<XdmfRectilinearGrid> rectgrid = gridHolder->getRectilinearGrid(0);
        std::cout << "The Rectilinear Grid's tag is: " << rectgrid->getItemTag().c_str() << std::endl;
        std::cout << "The Rectilinear Grid's name is: " << rectgrid->getName().c_str() << std::endl;
        std::cout << "The Rectilinear Grid's time is: " << rectgrid->getTime()->getValue() << std::endl;

	outputwalker = rectgrid->getItemProperties().begin();
	for (; outputwalker!=rectgrid->getItemProperties().end(); ++outputwalker)
	{
		std::cout << (*outputwalker).first.c_str() << ": " << (*outputwalker).second.c_str() << std::endl;
	}

        for (i=0; i<rectgrid->getNumberMaps(); ++i)
        {
                readMap = rectgrid->getMap(i);
		if (!readMap->isInitialized())
		{
			readMap->read();
		}
                std::cout << "Map # " << i << std::endl;
                taskIDMap = readMap->getMap();
                for (taskWalker = taskIDMap.begin(); taskWalker!= taskIDMap.end(); ++taskWalker)
                {
                        task = (*taskWalker).first;
                        nodeIDMap = (*taskWalker).second;
                        for (nodeWalker = nodeIDMap.begin(); nodeWalker != nodeIDMap.end(); ++nodeWalker)
                        {
                                node = (*nodeWalker).first;
                                remoteIDset = (*nodeWalker).second;
                                for (remoteWalker = remoteIDset.begin(); remoteWalker != remoteIDset.end(); ++remoteWalker)
                                {
                                        remote = (*remoteWalker);
					std::cout << "taskID: " << task << "\tlocalnodeID: " << node << "\tremotenodeID: " << remote << std::endl;
                                }
                        }
                }
        }
	for (i=0; i < rectgrid->getNumberSets(); ++i)
        {
                readSet = rectgrid->getSet(i);
		if (!readSet->isInitialized())
		{
			readSet->read();
		}
		std::cout << "Set # " << i << std::endl;
                std::cout << readSet->getName().c_str() << std::endl;
                if (readSet->getType() == XdmfSetType::Node())
                {
                        std::cout << "This set is a node" << std::endl;
                }
                else
                {
                        std::cout << "This set is not a node" << std::endl;
                }
                outputInformation = readSet->getInformation(0);
                std::cout << "Key: " << outputInformation->getKey().c_str() <<"\nValue: " << outputInformation->getValue().c_str() << std::endl;
                std::cout << readSet->getValuesString().c_str() << std::endl;
                for (j=0; j < readSet->getNumberAttributes(); ++j)
                {
                        readAttribute = readSet->getAttribute(j);
			if (!readAttribute->isInitialized())
			{
				readAttribute->read();
			}
                        std::cout << "Set Attribute # " << j << std::endl;
                        std::cout << readAttribute->getName().c_str() << std::endl;
                        if (readAttribute->getType() == XdmfAttributeType::Scalar())
                        {
                                std::cout << "This attribute is a scalar" << std::endl;
                        }
                        else
                        {
                                std::cout << "This attribute is not a scalar" << std::endl;
                        }
                        if (readAttribute->getCenter() == XdmfAttributeCenter::Node())
                        {
                                std::cout << "This attribute is a node" << std::endl;
                        }
                        else
                        {
                                std::cout << "This attribute is not a node" << std::endl;
                        }
                        std::cout << readAttribute->getValuesString().c_str() << std::endl;
                }
        }
        for (i = 0; i < rectgrid->getNumberAttributes(); ++i)
        {
                readAttribute = rectgrid->getAttribute(i);
		if (!readAttribute->isInitialized())
		{
			readAttribute->read();
		}
                std::cout << "Attribute # " << i << std::endl;
                std::cout << readAttribute->getName().c_str() << std::endl;
                if (readAttribute->getType() == XdmfAttributeType::Scalar())
                {
                        std::cout << "This attribute is a scalar" << std::endl;
                }
                else
                {
                        std::cout << "This attribute is not a scalar" << std::endl;
                }
                if (readAttribute->getCenter() == XdmfAttributeCenter::Node())
                {
                        std::cout << "This attribute is a node" << std::endl;
                }
                else
                {
                        std::cout << "This attribute is not a node" << std::endl;
                }
                std::cout << readAttribute->getValuesString().c_str() << std::endl;
        }
        std::cout << "Rectilinear Dimensions" << std::endl;
        shared_ptr<XdmfArray> rectdimensions = rectgrid->getDimensions();
	if (!rectdimensions->isInitialized())
	{
		rectdimensions->read();
	}
        std::cout << "The dimensions' tag: " << rectdimensions->getItemTag().c_str() << std::endl;
        std::cout << "Contains the values: " << rectdimensions->getValuesString().c_str() << std::endl;

        std::cout << "Rectilinear Coordinates" << std::endl;
        std::vector<shared_ptr<XdmfArray> > rectcoordinates = rectgrid->getCoordinates();
        std::cout << "Contains the values: " << std::endl;
	for (i=0;i<rectcoordinates.size(); ++i)
	{
		if (!rectcoordinates[i]->isInitialized())
		{
			rectcoordinates[i]->read();
		}
		std::cout << rectcoordinates[i]->getValuesString().c_str() << std::endl;
	}



        std::cout << "Regular Grid" << std::endl;
        shared_ptr<XdmfRegularGrid> reggrid = gridHolder->getRegularGrid(0);
        std::cout << "The Regular Grid's tag is: " << reggrid->getItemTag().c_str() << std::endl;
        std::cout << "The Regular Grid's name is: " << reggrid->getName().c_str() << std::endl;
        outputwalker = reggrid->getItemProperties().begin();
        for (;outputwalker!=reggrid->getItemProperties().end(); ++outputwalker)
        {
                std::cout << (*outputwalker).first.c_str() << ": " << (*outputwalker).second.c_str()  << std::endl;
        }
        std::cout << "The Regular Grid's time is: " << reggrid->getTime()->getValue() << std::endl;
        for (i=0; i<reggrid->getNumberMaps(); ++i)
        {
                readMap = reggrid->getMap(i);
		if (!readMap->isInitialized())
		{
			readMap->read();
		}
                std::cout << "Map # " << i << std::endl;
                taskIDMap = readMap->getMap();
                for (taskWalker = taskIDMap.begin(); taskWalker!= taskIDMap.end(); ++taskWalker)
                {
                        task = (*taskWalker).first;
                        nodeIDMap = (*taskWalker).second;
                        for (nodeWalker = nodeIDMap.begin(); nodeWalker != nodeIDMap.end(); ++nodeWalker)
                        {
                                node = (*nodeWalker).first;
                                remoteIDset = (*nodeWalker).second;
                                for (remoteWalker = remoteIDset.begin(); remoteWalker != remoteIDset.end(); ++remoteWalker)
                                {
                                        remote = (*remoteWalker);
					std::cout << "taskID: " << task << "\tlocalnodeID: " << node << "\tremotenodeID: " << remote << std::endl;
                                }
                        }
                }
        }
        for (i=0; i < reggrid->getNumberSets(); ++i)
        {
                readSet = reggrid->getSet(i);
		if (!readSet->isInitialized())
		{
			readSet->read();
		}
		std::cout << "Set # " << i << std::endl;
                std::cout << readSet->getName().c_str() << std::endl;
                if (readSet->getType() == XdmfSetType::Node())
                {
                        std::cout << "This set is a node" << std::endl;
                }
                else
                {
                        std::cout << "This set is not a node" << std::endl;
                }
                outputInformation = readSet->getInformation(0);
                std::cout << "Key: " << outputInformation->getKey().c_str() << "\nValue: " << outputInformation->getValue().c_str() << std::endl;
                std::cout << readSet->getValuesString().c_str() << std::endl;
                for (j=0; j < readSet->getNumberAttributes(); ++j)
                {
                        readAttribute = readSet->getAttribute(j);
			if (!readAttribute->isInitialized())
			{
				readAttribute->read();
			}
                        std::cout << "Set Attribute # " << j << std::endl;
                        std::cout << readAttribute->getName().c_str() << std::endl;
                        if (readAttribute->getType() == XdmfAttributeType::Scalar())
                        {
                                std::cout << "This attribute is a scalar" << std::endl;
                        }
                        else
                        {
                                std::cout << "This attribute is not a scalar" << std::endl;
                        }
                        if (readAttribute->getCenter() == XdmfAttributeCenter::Node())
                        {
                                std::cout << "This attribute is a node" << std::endl;
                        }
                        else
                        {
                                std::cout << "This attribute is not a node"  << std::endl;
                        }
                        std::cout <<  readAttribute->getValuesString().c_str()  << std::endl;
                }
        }
        for (i = 0; i < reggrid->getNumberAttributes(); ++i)
        {
                readAttribute = reggrid->getAttribute(i);
		if (!readAttribute->isInitialized())
		{
			readAttribute->read();
		}
                std::cout << "Attribute # " << i << std::endl;
                std::cout << readAttribute->getName().c_str() << std::endl;
                if (readAttribute->getType() == XdmfAttributeType::Scalar())
                {
                        std::cout << "This attribute is a scalar" << std::endl;
                }
                else
                {
                        std::cout << "This attribute is not a scalar" << std::endl;
                }
                if (readAttribute->getCenter() == XdmfAttributeCenter::Node())
                {
                        std::cout << "This attribute is a node" << std::endl;
                }
                else
                {
                        std::cout << "This attribute is not a node" << std::endl;
                }
                std::cout << readAttribute->getValuesString().c_str() << std::endl;
        }

        std::cout << "Regular Brick Size" << std::endl;
        shared_ptr<XdmfArray> regbricksize = reggrid->getBrickSize();
	if (!regbricksize->isInitialized())
	{
		regbricksize->read();
	}
        std::cout << "The brick's tag: " << regbricksize->getItemTag().c_str() << std::endl;
        std::cout << "Contains the values: " << regbricksize->getValuesString().c_str() << std::endl;

        std::cout << "Regular Number of Points" << std::endl;
        shared_ptr<XdmfArray> regnumpoints = reggrid->getDimensions();
	if (!regnumpoints->isInitialized())
	{
		regnumpoints->read();
	}
        std::cout << "The dimensions' tag: " << regnumpoints->getItemTag().c_str() << std::endl;
        std::cout << "Contains the values: " << regnumpoints->getValuesString().c_str() << std::endl;

        std::cout << "Regular Origin" << std::endl;
        shared_ptr<XdmfArray> regorigin = reggrid->getOrigin();
	if (!regorigin->isInitialized())
	{
		regorigin->read();
	}
        std::cout << "The origin's tag: " << regorigin->getItemTag().c_str() << std::endl;
        std::cout << "Contains the values: " << regorigin->getValuesString().c_str() << std::endl;






        primaryDomain->getInformation(0)->setKey("Edited");
        primaryDomain->getInformation(0)->setValue("This file is the edited version");
	std::cout << "edited domain information" << std::endl;

        shared_ptr<XdmfAttribute> unglobalIDs = ungrid->getAttribute(0);
        int newIDs1 [] = {5,2,8,7,9,1};
        unglobalIDs->insert(0, newIDs1, 6, 1, 1);
	std::cout << "edited unstructured attribute: " << unglobalIDs->getValuesString() << std::endl;

        shared_ptr<XdmfSet> unset = ungrid->getSet(0);
	double newunsetdata [] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 0.23};
	unset->insert(0, newunsetdata, 10, 1, 1);
	std::cout << "edited unstructured set: " << unset->getValuesString() << std::endl;

        untopology = ungrid->getTopology();
	int untoposize = untopology->getSize();
        int untopologydata [untoposize];
	untopology->getValues(0, untopologydata, untopology->getSize(), 1, 1);
        for (i=0; i < untopology->getSize(); ++i)
	{
		untopologydata[i] = untopologydata[i] + 1;
	}
        untopology->insert(0, untopologydata, untopology->getSize(), 1, 1);
	std::cout << "edited unstructured topology: " << untopology->getValuesString() << std::endl;

        ungeometry = ungrid->getGeometry();
	int ungeosize = ungeometry->getSize();
        double ungeometrydata [ungeosize];
	ungeometry->getValues(0, ungeometrydata, ungeometry->getSize(), 1, 1);
        for (i=0; i<ungeometry->getSize(); ++i)
	{
		ungeometrydata[i] = ungeometrydata[i] + 1;
	}
        ungeometry->insert(0, ungeometrydata, ungeometry->getSize(), 1, 1);


        shared_ptr<XdmfAttribute> curvglobalIDs = curvgrid->getAttribute(0);
        int newIDs2 [] = {3, 6, 2, 8, 1, 7, 5};
        curvglobalIDs->insert(0, newIDs2, 7, 1, 1);

        curvgeometry = curvgrid->getGeometry();
	int curvgeosize = curvgeometry->getSize();
        double curvgeometrydata [curvgeosize];
	curvgeometry->getValues(0, curvgeometrydata, curvgeometry->getSize(), 1, 1);
        for (i = 0; i<curvgeometry->getSize(); ++i)
	{
		curvgeometrydata[i] = curvgeometrydata[i] + 1;
	}
        curvgeometry->insert(0, curvgeometrydata, curvgeometry->getSize(), 1, 1);
	for (i = 0; i < 3; ++i)
	{
		curvgeometry->pushBack(curvgeometry->getSize());
	}

        curvdimensions = curvgrid->getDimensions();
	int curvdimensionsize = curvdimensions->getSize();
        int curvdimensiondata [curvdimensionsize];
	curvdimensions->getValues(0, curvdimensiondata, curvdimensions->getSize(), 1, 1);
        for (i = 0;  i< curvdimensions->getSize(); ++i)
	{
		curvdimensiondata[i] = curvdimensiondata[i] + 1;
	}
        curvdimensions->insert(0, curvdimensiondata, curvdimensions->getSize(), 1, 1);


        shared_ptr<XdmfAttribute> rectglobalIDs = rectgrid->getAttribute(0);
        int newIDs3 [] = {6, 4, 3, 7, 9, 8};
        rectglobalIDs->insert(0, newIDs3, 6, 1, 1);

        rectdimensions = rectgrid->getDimensions();
	int rectdimensionsize = rectdimensions->getSize();
        int rectdimensiondata [rectdimensionsize];
	rectdimensions->getValues(0, rectdimensiondata, rectdimensions->getSize(), 1, 1);
        for (i = 0; i < rectdimensions->getSize(); ++i)
	{
		rectdimensiondata[i] = rectdimensiondata[i] + 1;
	}
        rectdimensions->insert(0, rectdimensiondata, rectdimensions->getSize(), 1, 1);

        std::vector<shared_ptr<XdmfArray> > rectcoordinatevector = rectgrid->getCoordinates();
	shared_ptr<XdmfArray> coordinateaxis;
	for (i=0;i<rectcoordinatevector.size(); ++i)
	{
		coordinateaxis = rectcoordinatevector[i];
		int coordinatesize = coordinateaxis->getSize();
		int coordinatedata [coordinatesize];
		coordinateaxis->getValues(0, coordinatedata, coordinateaxis->getSize(), 1, 1);
		for (j = 0; j < coordinateaxis->getSize(); j++)
		{
			coordinatedata[i] = coordinatedata[i] + 1;
		}
		coordinateaxis->insert(0, coordinatedata, coordinateaxis->getSize(), 1, 1);
		coordinateaxis->pushBack(coordinateaxis->getSize());
		rectcoordinatevector[i] = coordinateaxis;
	}
	rectgrid->setCoordinates(rectcoordinatevector);

        shared_ptr<XdmfAttribute> regglobalIDs = reggrid->getAttribute(0);
        int newIDs4 [] = {3, 5, 1, 2, 4, 8, 0};
        regglobalIDs->insert(0, newIDs4, 7, 1, 1);

        regbricksize = reggrid->getBrickSize();
	int regbrickdatasize = regbricksize->getSize();
        double brickdata [regbrickdatasize];
	regbricksize->getValues(0, brickdata, regbricksize->getSize(), 1, 1);
	for (i = 0; i < regbricksize->getSize(); ++i)
	{
		brickdata[i] = brickdata[i] + 1;
	}
        regbricksize->insert(0, brickdata, regbricksize->getSize(), 1, 1);
        reggrid->setBrickSize(regbricksize);

        shared_ptr<XdmfArray> regdimensions = reggrid->getDimensions();
	int regdimensionssize = regdimensions->getSize();
        int regdimensiondata [regdimensionssize];
	regdimensions->getValues(0, regdimensiondata, regdimensions->getSize(), 1, 1);
	for (i = 0; i < regdimensions->getSize(); ++i)
	{
		regdimensiondata[i] = regdimensiondata[i] + 1;
	}
        regdimensions->insert(0, regdimensiondata, regdimensions->getSize(), 1, 1);
        reggrid->setDimensions(regdimensions);

        regorigin = reggrid->getOrigin();
	int regoriginsize = regorigin->getSize();
        double regorigindata [regoriginsize];
	regorigin->getValues(0, regorigindata, regorigin->getSize(), 1, 1);
	for (i = 0; i < regorigin->getSize(); ++i)
	{
		regorigindata[i] = regorigindata[i] + 1;
	}
        regorigin->insert(0, regorigindata, regorigin->getSize(), 1, 1);
        reggrid->setOrigin(regorigin);

	shared_ptr<XdmfHDF5Writer> exampleHeavyWriter = XdmfHDF5Writer::New("editedtestoutput.h5");
        shared_ptr<XdmfWriter> exampleWriter = XdmfWriter::New("editedtestoutput.xmf", exampleHeavyWriter);
	//exampleHeavyWriter->setFileSizeLimit(1);
	primaryDomain->accept(exampleHeavyWriter);
	exampleHeavyWriter->setMode(XdmfHeavyDataWriter::Overwrite);
        primaryDomain->accept(exampleWriter);

	return 0;
}
