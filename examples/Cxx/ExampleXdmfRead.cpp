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
	printf("Program Started\n");
        shared_ptr<XdmfReader> exampleReader = XdmfReader::New();

        /*
        This is assuming that the read item is an XdmfDomain object
        */
        shared_ptr<XdmfDomain> primaryDomain = shared_dynamic_cast<XdmfDomain>(exampleReader->read("testoutput.xmf"));
	//shared_ptr<XdmfDomain> primaryDomain = shared_dynamic_cast<XdmfDomain>(exampleReader->read("editedtestoutput.xmf"));
        shared_ptr<XdmfInformation> outputInformation = primaryDomain->getInformation(0);
        printf("Key: %s\nValue: %s\n", outputInformation->getKey().c_str(), outputInformation->getValue().c_str());
        printf("The Domain's tag is: %s\n", primaryDomain->getItemTag().c_str());

        shared_ptr<XdmfGridCollection> gridHolder = primaryDomain->getGridCollection(0);

        printf("The Grid Collection's tag is: %s\n", gridHolder->getItemTag().c_str());
        printf("The Grid Collection's name is: %s\n", gridHolder->getName().c_str());
        outputInformation = gridHolder->getInformation(0);
        printf("Key: %s\nValue: %s\n", outputInformation->getKey().c_str(), outputInformation->getValue().c_str());

	std::map<std::string, std::string>::iterator outputwalker = gridHolder->getItemProperties().begin();
	for (;outputwalker!=gridHolder->getItemProperties().end(); outputwalker++)
	{
		printf("%s: %s\n", (*outputwalker).first.c_str(), (*outputwalker).second.c_str());
	}
 
        if (gridHolder->getType() == XdmfGridCollectionType::Spatial())
	{
                printf("This is a spatial grid collection\n");
	}
        else
	{
                printf("This is not a spatial grid collection\n");
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

	for (i=0; i<gridHolder->getNumberMaps(); i++)
	{
		readMap = gridHolder->getMap(i);
		if (!readMap->isInitialized())
		{
			readMap->read();
		}
		printf("Map # %d\n", i);
		taskIDMap = readMap->getMap();
		for (taskWalker = taskIDMap.begin(); taskWalker!= taskIDMap.end(); taskWalker++)
		{
			task = (*taskWalker).first;
			nodeIDMap = (*taskWalker).second;
			for (nodeWalker = nodeIDMap.begin(); nodeWalker != nodeIDMap.end(); nodeWalker++)
			{
				node = (*nodeWalker).first;
				remoteIDset = (*nodeWalker).second;
				for (remoteWalker = remoteIDset.begin(); remoteWalker != remoteIDset.end();remoteWalker++)
				{
					remote = (*remoteWalker);
					printf("taskID: %d\tlocalnodeID: %d\tremotenodeID: %d\n", task, node, remote);
				}
			}
		}
	}

        printf("Unstructured Grid\n");
        shared_ptr<XdmfUnstructuredGrid> ungrid = gridHolder->getUnstructuredGrid(0);
        printf("The Unstructured Grid's tag is: %s\n", ungrid->getItemTag().c_str());
        printf("The Unstructured Grid's name is: %s\n", ungrid->getName().c_str());

	outputwalker = ungrid->getItemProperties().begin();
        for (;outputwalker!=ungrid->getItemProperties().end(); outputwalker++)
        {
                printf("%s: %s\n", (*outputwalker).first.c_str(), (*outputwalker).second.c_str());
        }

        printf("The Unstructured Grid's time is: %f\n", ungrid->getTime()->getValue());
        i=0;
        for (i=0; i<ungrid->getNumberMaps(); i++)
        {
                readMap = ungrid->getMap(i);
		if (!readMap->isInitialized())
		{
			readMap->read();
		}
                printf("Map # %d\n", i);
                taskIDMap = readMap->getMap();
                for (taskWalker = taskIDMap.begin(); taskWalker!= taskIDMap.end(); taskWalker++)
                {
                        task = (*taskWalker).first;
                        nodeIDMap = (*taskWalker).second;
                        for (nodeWalker = nodeIDMap.begin(); nodeWalker != nodeIDMap.end(); nodeWalker++)
                        {
                                node = (*nodeWalker).first;
                                remoteIDset = (*nodeWalker).second;
                                for (remoteWalker = remoteIDset.begin(); remoteWalker != remoteIDset.end();remoteWalker++)
                                {
                                        remote = (*remoteWalker);
                                        printf("taskID: %d\tlocalnodeID: %d\tremotenodeID: %d\n", task, node, remote);
                                }
                        }
                }
        }

	shared_ptr<XdmfSet> readSet;
	shared_ptr<XdmfAttribute> readAttribute;
	for (i=0; i < ungrid->getNumberSets(); i++)
	{
		readSet = ungrid->getSet(i);
		if (!readSet->isInitialized())
		{
			readSet->read();
		}
		printf("Set # %d\n", i);
		printf("%s\n", readSet->getName().c_str());
		if (readSet->getType() == XdmfSetType::Node())
		{
			printf("This set is a node\n");
		}
		else
		{
			printf("This set is not a node\n");
		}
		outputInformation = readSet->getInformation(0);
		printf("Key: %s\nValue: %s\n", outputInformation->getKey().c_str(), outputInformation->getValue().c_str());
		printf("%s\n", readSet->getValuesString().c_str());
		for (j=0; j < readSet->getNumberAttributes(); j++)
		{
			readAttribute = readSet->getAttribute(j);
			if (!readAttribute->isInitialized())
			{
				readAttribute->read();
			}
			printf("Set Attribute # %d\n", j);
			printf("%s\n", readAttribute->getName().c_str());
			if (readAttribute->getType() == XdmfAttributeType::Scalar())
			{
				printf("This attribute is a scalar\n");
			}
			else
			{
				printf("This attribute is not a scalar\n");
			}
			if (readAttribute->getCenter() == XdmfAttributeCenter::Node())
			{
				printf("This attribute is a node\n");
			}
			else
			{
				printf("This attribute is not a node\n");
			}
			printf("%s\n", readAttribute->getValuesString().c_str());
		}
	}
	for (i = 0; i < ungrid->getNumberAttributes(); i++)
	{
		readAttribute = ungrid->getAttribute(i);
		if (!readAttribute->isInitialized())
		{
			readAttribute->read();
		}
		printf("Attribute # %d\n", i);
		printf("%s\n", readAttribute->getName().c_str());
		if (readAttribute->getType() == XdmfAttributeType::Scalar())
		{
			printf("This attribute is a scalar\n");
		}
		else
		{
			printf("This attribute is not a scalar\n");
		}
		if (readAttribute->getCenter() == XdmfAttributeCenter::Node())
		{
			printf("This attribute is a node\n");
		}
		else
		{
			printf("This attribute is not a node\n");
		}
		printf("%s\n", readAttribute->getValuesString().c_str());
	}

	printf("Unstructured Topology\n");
	shared_ptr<XdmfTopology> untopology = ungrid->getTopology();
	if (!untopology->isInitialized())
	{
		untopology->read();
	}
	printf("The topology's tag: %s\n", untopology->getItemTag().c_str());
	if (untopology->getType() == XdmfTopologyType::Hexahedron())
	{
		printf("This topology is a hexahedron\n");
        }
	else
	{
		printf("This topology is not a hexahedron\n");
	}
	printf("Contains %d elements\n", untopology->getNumberElements());
	printf("Contains the values: %s\n", untopology->getValuesString().c_str());

	printf("Unstructured Geometry\n");
	shared_ptr<XdmfGeometry> ungeometry = ungrid->getGeometry();
	if (!ungeometry->isInitialized())
	{
		ungeometry->read();
	}
	printf("The geometry's tag: %s\n", ungeometry->getItemTag().c_str());
	if (ungeometry->getType() == XdmfGeometryType::XYZ())
	{
		printf("This geometry is XYZ\n");
	}
	else
	{
		printf("This geometry is not XYZ\n");
	}
	outputInformation = ungeometry->getInformation(0);
	printf("Key: %s\nValue: %s\n", outputInformation->getKey().c_str(), outputInformation->getValue().c_str());
	printf("Contains %d points\n", ungeometry->getNumberPoints());
	printf("Contains the values: %s\n", ungeometry->getValuesString().c_str());





        printf("Curvilinear Grid\n");
        shared_ptr<XdmfCurvilinearGrid> curvgrid = gridHolder->getCurvilinearGrid(0);
        printf("The Curvilinear Grid's tag is: %s\n", curvgrid->getItemTag().c_str());
        printf("The Curvilinear Grid's name is: %s\n", curvgrid->getName().c_str());
        outputwalker = curvgrid->getItemProperties().begin();
        for (;outputwalker!=curvgrid->getItemProperties().end(); outputwalker++)
        {
                printf("%s: %s\n", (*outputwalker).first.c_str(), (*outputwalker).second.c_str());
        }
        outputInformation = curvgrid->getInformation(0);
        printf("Key: %s\nValue: %s\n", outputInformation->getKey().c_str(), outputInformation->getValue().c_str());
        printf("The Curvilinear Grid's time is: %f\n", curvgrid->getTime()->getValue());
        for (i=0; i<curvgrid->getNumberMaps(); i++)
        {
                readMap = curvgrid->getMap(i);
		if (!readMap->isInitialized())
		{
			readMap->read();
		}
                printf("Map # %d\n", i);
                taskIDMap = readMap->getMap();
                for (taskWalker = taskIDMap.begin(); taskWalker!= taskIDMap.end(); taskWalker++)
                {
                        task = (*taskWalker).first;
                        nodeIDMap = (*taskWalker).second;
                        for (nodeWalker = nodeIDMap.begin(); nodeWalker != nodeIDMap.end(); nodeWalker++)
                        {
                                node = (*nodeWalker).first;
                                remoteIDset = (*nodeWalker).second;
                                for (remoteWalker = remoteIDset.begin(); remoteWalker != remoteIDset.end();remoteWalker++)
                                {
                                        remote = (*remoteWalker);
                                        printf("taskID: %d\tlocalnodeID: %d\tremotenodeID: %d\n", task, node, remote);
                                }
                        }
                }
        }
        for (i=0; i < curvgrid->getNumberSets(); i++)
        {
                readSet = curvgrid->getSet(i);
		if (!readSet->isInitialized())
		{
			readSet->read();
		}
                printf("Set # %d\n", i);
                printf("%s\n", readSet->getName().c_str());
                if (readSet->getType() == XdmfSetType::Node())
                {
                        printf("This set is a node\n");
                }
                else
                {
                        printf("This set is not a node\n");
                }
                outputInformation = readSet->getInformation(0);
                printf("Key: %s\nValue: %s\n", outputInformation->getKey().c_str(), outputInformation->getValue().c_str());
                printf("%s\n", readSet->getValuesString().c_str());
                for (j=0; j < readSet->getNumberAttributes(); j++)
                {
                        readAttribute = readSet->getAttribute(j);
			if (!readAttribute->isInitialized())
			{
				readAttribute->read();
			}
                        printf("Set Attribute # %d\n", j);
                        printf("%s\n", readAttribute->getName().c_str());
                        if (readAttribute->getType() == XdmfAttributeType::Scalar())
                        {
                                printf("This attribute is a scalar\n");
                        }
                        else
                        {
                                printf("This attribute is not a scalar\n");
                        }
                        if (readAttribute->getCenter() == XdmfAttributeCenter::Node())
                        {
                                printf("This attribute is a node\n");
                        }
                        else
                        {
                                printf("This attribute is not a node\n");
                        }
                        printf("%s\n", readAttribute->getValuesString().c_str());
                }
        }
        for (i = 0; i < curvgrid->getNumberAttributes(); i++)
        {
                readAttribute = curvgrid->getAttribute(i);
		if (!readAttribute->isInitialized())
		{
			readAttribute->read();
		}
                printf("Attribute # %d\n", i);
                printf("%s\n", readAttribute->getName().c_str());
                if (readAttribute->getType() == XdmfAttributeType::Scalar())
                {
                        printf("This attribute is a scalar\n");
                }
                else
                {
                        printf("This attribute is not a scalar\n");
                }
                if (readAttribute->getCenter() == XdmfAttributeCenter::Node())
                {
                        printf("This attribute is a node\n");
                }
                else
                {
                        printf("This attribute is not a node\n");
                }
                printf("%s\n", readAttribute->getValuesString().c_str());
        }

        printf("Curvilinear Dimensions\n");
        shared_ptr<XdmfArray> curvdimensions = curvgrid->getDimensions();
	if (!curvdimensions->isInitialized())
	{
		curvdimensions->read();
	}
        printf("The dimensions' tag: %s\n", curvdimensions->getItemTag().c_str());
        printf("Contains the values: %s\n", curvdimensions->getValuesString().c_str());

        printf("Curvilinear Geometry\n");
        shared_ptr<XdmfGeometry> curvgeometry = curvgrid->getGeometry();
	if (!curvgeometry->isInitialized())
	{
		curvgeometry->read();
	}
        printf("The geometry's tag: %s\n", curvgeometry->getItemTag().c_str());
        if (curvgeometry->getType() == XdmfGeometryType::XYZ())
	{
                printf("This geometry is XYZ\n");
        }
	else
	{
                printf("This geometry is not XYZ\n");
	}
        outputInformation = curvgeometry->getInformation(0);
        printf("Key: %s\nValue: %s\n", outputInformation->getKey().c_str(), outputInformation->getValue().c_str());
        printf("Contains %d points\n", curvgeometry->getNumberPoints());
        printf("Contains the values: %s\n", curvgeometry->getValuesString().c_str());


        printf("Rectilinear Grid\n");
        shared_ptr<XdmfRectilinearGrid> rectgrid = gridHolder->getRectilinearGrid(0);
        printf("The Rectilinear Grid's tag is: %s\n", rectgrid->getItemTag().c_str());
        printf("The Rectilinear Grid's name is: %s\n", rectgrid->getName().c_str());
        printf("The Rectilinear Grid's time is: %f\n", rectgrid->getTime()->getValue());

	outputwalker = rectgrid->getItemProperties().begin();
	for (; outputwalker!=rectgrid->getItemProperties().end(); outputwalker++)
	{
		printf("%s: %s\n", (*outputwalker).first.c_str(), (*outputwalker).second.c_str());
	}

        for (i=0; i<rectgrid->getNumberMaps(); i++)
        {
                readMap = rectgrid->getMap(i);
		if (!readMap->isInitialized())
		{
			readMap->read();
		}
                printf("Map # %d\n", i);
                taskIDMap = readMap->getMap();
                for (taskWalker = taskIDMap.begin(); taskWalker!= taskIDMap.end(); taskWalker++)
                {
                        task = (*taskWalker).first;
                        nodeIDMap = (*taskWalker).second;
                        for (nodeWalker = nodeIDMap.begin(); nodeWalker != nodeIDMap.end(); nodeWalker++)
                        {
                                node = (*nodeWalker).first;
                                remoteIDset = (*nodeWalker).second;
                                for (remoteWalker = remoteIDset.begin(); remoteWalker != remoteIDset.end();remoteWalker++)
                                {
                                        remote = (*remoteWalker);
                                        printf("taskID: %d\tlocalnodeID: %d\tremotenodeID: %d\n", task, node, remote);
                                }
                        }
                }
        }
	for (i=0; i < rectgrid->getNumberSets(); i++)
        {
                readSet = rectgrid->getSet(i);
		if (!readSet->isInitialized())
		{
			readSet->read();
		}
                printf("Set # %d\n", i);
                printf("%s\n", readSet->getName().c_str());
                if (readSet->getType() == XdmfSetType::Node())
                {
                        printf("This set is a node\n");
                }
                else
                {
                        printf("This set is not a node\n");
                }
                outputInformation = readSet->getInformation(0);
                printf("Key: %s\nValue: %s\n", outputInformation->getKey().c_str(), outputInformation->getValue().c_str());
                printf("%s\n", readSet->getValuesString().c_str());
                for (j=0; j < readSet->getNumberAttributes(); j++)
                {
                        readAttribute = readSet->getAttribute(j);
			if (!readAttribute->isInitialized())
			{
				readAttribute->read();
			}
                        printf("Set Attribute # %d\n", j);
                        printf("%s\n", readAttribute->getName().c_str());
                        if (readAttribute->getType() == XdmfAttributeType::Scalar())
                        {
                                printf("This attribute is a scalar\n");
                        }
                        else
                        {
                                printf("This attribute is not a scalar\n");
                        }
                        if (readAttribute->getCenter() == XdmfAttributeCenter::Node())
                        {
                                printf("This attribute is a node\n");
                        }
                        else
                        {
                                printf("This attribute is not a node\n");
                        }
                        printf("%s\n", readAttribute->getValuesString().c_str());
                }
        }
        for (i = 0; i < rectgrid->getNumberAttributes(); i++)
        {
                readAttribute = rectgrid->getAttribute(i);
		if (!readAttribute->isInitialized())
		{
			readAttribute->read();
		}
                printf("Attribute # %d\n", i);
                printf("%s\n", readAttribute->getName().c_str());
                if (readAttribute->getType() == XdmfAttributeType::Scalar())
                {
                        printf("This attribute is a scalar\n");
                }
                else
                {
                        printf("This attribute is not a scalar\n");
                }
                if (readAttribute->getCenter() == XdmfAttributeCenter::Node())
                {
                        printf("This attribute is a node\n");
                }
                else
                {
                        printf("This attribute is not a node\n");
                }
                printf("%s\n", readAttribute->getValuesString().c_str());
        }
        printf("Rectilinear Dimensions\n");
        shared_ptr<XdmfArray> rectdimensions = rectgrid->getDimensions();
	if (!rectdimensions->isInitialized())
	{
		rectdimensions->read();
	}
        printf("The dimensions' tag: %s\n", rectdimensions->getItemTag().c_str());
        printf("Contains the values: %s\n", rectdimensions->getValuesString().c_str());

        printf("Rectilinear Coordinates\n");
        std::vector<shared_ptr<XdmfArray> > rectcoordinates = rectgrid->getCoordinates();
        printf("Contains the values: \n");
	for (i=0;i<rectcoordinates.size();i++)
	{
		if (!rectcoordinates[i]->isInitialized())
		{
			rectcoordinates[i]->read();
		}
		printf("%s\n", rectcoordinates[i]->getValuesString().c_str());
	}

        printf("Regular Grid\n");
        shared_ptr<XdmfRegularGrid> reggrid = gridHolder->getRegularGrid(0);
        printf("The Regular Grid's tag is: %s\n", reggrid->getItemTag().c_str());
        printf("The Regular Grid's name is: %s\n", reggrid->getName().c_str());
        outputwalker = reggrid->getItemProperties().begin();
        for (;outputwalker!=reggrid->getItemProperties().end(); outputwalker++)
        {
                printf("%s: %s\n", (*outputwalker).first.c_str(), (*outputwalker).second.c_str());
        }
        printf("The Regular Grid's time is: %f\n", reggrid->getTime()->getValue());
        for (i=0; i<reggrid->getNumberMaps(); i++)
        {
                readMap = reggrid->getMap(i);
		if (!readMap->isInitialized())
		{
			readMap->read();
		}
                printf("Map # %d\n", i);
                taskIDMap = readMap->getMap();
                for (taskWalker = taskIDMap.begin(); taskWalker!= taskIDMap.end(); taskWalker++)
                {
                        task = (*taskWalker).first;
                        nodeIDMap = (*taskWalker).second;
                        for (nodeWalker = nodeIDMap.begin(); nodeWalker != nodeIDMap.end(); nodeWalker++)
                        {
                                node = (*nodeWalker).first;
                                remoteIDset = (*nodeWalker).second;
                                for (remoteWalker = remoteIDset.begin(); remoteWalker != remoteIDset.end();remoteWalker++)
                                {
                                        remote = (*remoteWalker);
                                        printf("taskID: %d\tlocalnodeID: %d\tremotenodeID: %d\n", task, node, remote);
                                }
                        }
                }
        }
        for (i=0; i < reggrid->getNumberSets(); i++)
        {
                readSet = reggrid->getSet(i);
		if (!readSet->isInitialized())
		{
			readSet->read();
		}
                printf("Set # %d\n", i);
                printf("%s\n", readSet->getName().c_str());
                if (readSet->getType() == XdmfSetType::Node())
                {
                        printf("This set is a node");
                }
                else
                {
                        printf("This set is not a node");
                }
                outputInformation = readSet->getInformation(0);
                printf("Key: %s\nValue: %s\n", outputInformation->getKey().c_str(), outputInformation->getValue().c_str());
                printf("%s\n", readSet->getValuesString().c_str());
                for (j=0; j < readSet->getNumberAttributes(); j++)
                {
                        readAttribute = readSet->getAttribute(j);
			if (!readAttribute->isInitialized())
			{
				readAttribute->read();
			}
                        printf("Set Attribute # %d\n", j);
                        printf("%s\n", readAttribute->getName().c_str());
                        if (readAttribute->getType() == XdmfAttributeType::Scalar())
                        {
                                printf("This attribute is a scalar");
                        }
                        else
                        {
                                printf("This attribute is not a scalar");
                        }
                        if (readAttribute->getCenter() == XdmfAttributeCenter::Node())
                        {
                                printf("This attribute is a node");
                        }
                        else
                        {
                                printf("This attribute is not a node");
                        }
                        printf("%s\n", readAttribute->getValuesString().c_str());
                }
        }
        for (i = 0; i < reggrid->getNumberAttributes(); i++)
        {
                readAttribute = reggrid->getAttribute(i);
		if (!readAttribute->isInitialized())
		{
			readAttribute->read();
		}
                printf("Attribute # %d\n", i);
                printf("%s\n", readAttribute->getName().c_str());
                if (readAttribute->getType() == XdmfAttributeType::Scalar())
                {
                        printf("This attribute is a scalar\n");
                }
                else
                {
                        printf("This attribute is not a scalar\n");
                }
                if (readAttribute->getCenter() == XdmfAttributeCenter::Node())
                {
                        printf("This attribute is a node\n");
                }
                else
                {
                        printf("This attribute is not a node\n");
                }
                printf("%s\n", readAttribute->getValuesString().c_str());
        }

        printf("Regular Brick Size\n");
        shared_ptr<XdmfArray> regbricksize = reggrid->getBrickSize();
	if (!regbricksize->isInitialized())
	{
		regbricksize->read();
	}
        printf("The brick's tag: %s\n", regbricksize->getItemTag().c_str());
        printf("Contains the values: %s\n", regbricksize->getValuesString().c_str());

        printf("Regular Number of Points\n");
        shared_ptr<XdmfArray> regnumpoints = reggrid->getDimensions();
        printf("The dimensions' tag: %s\n", regnumpoints->getItemTag().c_str());
        printf("Contains the values: %s\n", regnumpoints->getValuesString().c_str());

        printf("Regular Origin\n");
        shared_ptr<XdmfArray> regorigin = reggrid->getOrigin();
	if (!regorigin->isInitialized())
	{
		regorigin->read();
	}
        printf("The origin's tag: %s\n", regorigin->getItemTag().c_str());
        printf("Contains the values: %s\n", regorigin->getValuesString().c_str());

	return 0;
}
