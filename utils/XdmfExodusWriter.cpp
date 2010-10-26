#include <exodusII.h>
#include <sstream>
#include "XdmfArrayType.hpp"
#include "XdmfAttribute.hpp"
#include "XdmfAttributeCenter.hpp"
#include "XdmfExodusWriter.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGeometryType.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfGridCollectionType.hpp"
#include "XdmfGridUnstructured.hpp"
#include "XdmfSet.hpp"
#include "XdmfSetType.hpp"
#include "XdmfTopology.hpp"
#include "XdmfTopologyType.hpp"

/**
 * PIMPL
 */
class XdmfExodusWriter::XdmfExodusWriterImpl {

public:

	XdmfExodusWriterImpl()
	{
	};

	~XdmfExodusWriterImpl()
	{
	};

	/**
	 * Constructs attribute names for ExodusII files since exodus cannot store vectors.  Deals with MAX_STR_LENGTH
	 * limitation.
	 *
	 * @param attributeName the attribute name in Xdmf.
	 * @param names a vector of names to add the constructed attribute names to.
	 * @param numComponents the number of components in the attribute (e.g. for an xyz vector this is 3)
	 */
	void constructAttributeNames(std::string attributeName, std::vector<std::string> & names, const int numComponents)
	{
		if(numComponents == 1)
		{
			if(attributeName.length() > MAX_STR_LENGTH)
			{
				attributeName = attributeName.substr(0, MAX_STR_LENGTH);
			}
			names.push_back(attributeName);
		}
		else if(numComponents > 1)
		{
			int numComponentDigits = int(numComponents / 10);
			if(attributeName.length() + numComponentDigits > MAX_STR_LENGTH)
			{
				attributeName = attributeName.substr(0, MAX_STR_LENGTH - numComponentDigits);
			}
			for(int i=0; i<numComponents; ++i)
			{
				std::stringstream toAdd;
				toAdd << attributeName << "-" << i+1;
				names.push_back(toAdd.str());
			}
		}
	}
};

boost::shared_ptr<XdmfExodusWriter> XdmfExodusWriter::New()
{
	boost::shared_ptr<XdmfExodusWriter> p(new XdmfExodusWriter());
	return p;
}

XdmfExodusWriter::XdmfExodusWriter() :
	mImpl(new XdmfExodusWriterImpl())
{
}

XdmfExodusWriter::~XdmfExodusWriter()
{
	delete mImpl;
}

void XdmfExodusWriter::write(const std::string & filePath, const boost::shared_ptr<XdmfGridUnstructured> gridToWrite) const
{

	// Open Exodus File
	int wordSize = 8;
	int storeSize = 8;
	int exodusHandle = ex_create(filePath.c_str(), EX_CLOBBER, &wordSize, &storeSize);

	// Initialize Exodus File
	std::string title = gridToWrite->getName();
	if(title.size() > MAX_STR_LENGTH)
	{
		title = title.substr(0, MAX_STR_LENGTH);
	}

	boost::shared_ptr<XdmfGridCollection> gridCollection = boost::shared_ptr<XdmfGridCollection>();
	boost::shared_ptr<XdmfGridUnstructured> currGrid = gridToWrite;

	// Check if they are temporal collections and use the first grid to determine geometry and topology.
	if(boost::shared_ptr<XdmfGridCollection> tmpGrid = boost::shared_dynamic_cast<XdmfGridCollection>(gridToWrite))
	{
		if(tmpGrid->getType() == XdmfGridCollectionType::Temporal() && tmpGrid->getNumberGridUnstructureds() > 0)
		{
			currGrid = tmpGrid->getGridUnstructured(0);
			gridCollection = tmpGrid;
		}
		else
		{
			// Only Temporal Collections are currently supported.
			assert(false);
		}
	}
	else
	{
		// This is expected to fail when we've already found a grid.
	}

	// Make sure geometry and topology are non null
	assert(currGrid->getGeometry() && currGrid->getTopology());

	int num_dim = currGrid->getGeometry()->getType()->getDimensions();
	int num_nodes = currGrid->getGeometry()->getNumberPoints();
	int num_elem = currGrid->getTopology()->getNumberElements();
	int num_elem_blk = 1;
	int num_node_sets = 0;
	int num_side_sets = 0;

	for (unsigned int i=0; i<currGrid->getNumberSets(); ++i)
	{
		if(currGrid->getSet(i)->getType() == XdmfSetType::Cell())
		{
			num_side_sets++;
		}
		else if(currGrid->getSet(i)->getType() == XdmfSetType::Node())
		{
			num_node_sets++;
		}
	}

	ex_put_init(exodusHandle, title.c_str(), num_dim, num_nodes, num_elem, num_elem_blk, num_node_sets, num_side_sets);


	double * x = new double[num_nodes];
	double * y = new double[num_nodes];
	double * z = new double[num_nodes];
	// Write nodal coordinate values to exodus
	if(currGrid->getGeometry()->getType() == XdmfGeometryType::XYZ() || currGrid->getGeometry()->getType() == XdmfGeometryType::XY())
	{
		currGrid->getGeometry()->getValues(0, x, num_nodes, 3);
		currGrid->getGeometry()->getValues(1, y, num_nodes, 3);
		if(currGrid->getGeometry()->getType() == XdmfGeometryType::XYZ())
		{
			currGrid->getGeometry()->getValues(2, z, num_nodes, 3);
		}
		ex_put_coord(exodusHandle, x ,y ,z);
	}
	else if(currGrid->getGeometry()->getType() == XdmfGeometryType::X_Y_Z() || currGrid->getGeometry()->getType() == XdmfGeometryType::X_Y())
	{
		currGrid->getGeometry()->getValues(0, x, num_nodes);
		currGrid->getGeometry()->getValues(num_nodes, y, num_nodes);
		if(currGrid->getGeometry()->getType() == XdmfGeometryType::X_Y_Z())
		{
			currGrid->getGeometry()->getValues(num_nodes * 2, z, num_nodes);
		}
	}
	delete [] x;
	delete [] y;
	delete [] z;

	// Write Element block parameters
	std::string exodusTopologyType = this->xdmfToExodusTopologyType(currGrid->getTopology()->getType());
	if (exodusTopologyType.compare("") == 0)
	{
		// Topology Type not supported by ExodusII
		assert(false);
	}
	ex_put_elem_block(exodusHandle, 10, exodusTopologyType.c_str(), num_elem, currGrid->getTopology()->getType()->getNodesPerElement(), num_side_sets);

	// Write Element Connectivity
	int * elem_connectivity = new int[num_elem * currGrid->getTopology()->getType()->getNodesPerElement()];
	currGrid->getTopology()->getValues(0, elem_connectivity, num_elem * currGrid->getTopology()->getType()->getNodesPerElement());
	for(unsigned int i=0; i<num_elem * currGrid->getTopology()->getType()->getNodesPerElement(); ++i)
	{
		// Add 1 to connectivity array since exodus indices start at 1
		elem_connectivity[i]++;
	}

	if(currGrid->getTopology()->getType() == XdmfTopologyType::Hexahedron_20() || currGrid->getTopology()->getType() == XdmfTopologyType::Hexahedron_27())
	{
		int * ptr = elem_connectivity;
		int itmp[4];

		// Exodus Node ordering does not match Xdmf, we must convert.
		for(int i=0; i<num_elem; ++i)
		{
			ptr += 12;

			for (unsigned int j=0; j<4; ++j, ++ptr)
			{
				itmp[j] = *ptr;
				*ptr = ptr[4];
			}

			for(unsigned int j=0; j<4; ++j, ++ptr)
			{
				*ptr = itmp[j];
			}

			if(currGrid->getTopology()->getType() == XdmfTopologyType::Hexahedron_27())
			{
				itmp[0] = *ptr;
				*(ptr++) = ptr[6];
				itmp[1] = *ptr;
				*(ptr++) = ptr[3];
				itmp[2] = *ptr;
				*(ptr++) = ptr[3];
				itmp[3] = *ptr;
				for (unsigned int j=0; j<4; ++j, ++ptr)
				{
					*ptr = itmp[j];
				}
			}
		}
	}
	else if(currGrid->getTopology()->getType() == XdmfTopologyType::Wedge_15() || currGrid->getTopology()->getType() == XdmfTopologyType::Wedge_18())
	{
		int * ptr = elem_connectivity;
		int itmp[3];

		// Exodus Node ordering does not match Xdmf, we must convert.
		for(int i=0; i<num_elem; ++i)
		{
			ptr += 9;

			for(unsigned int j=0; j<3; ++j, ++ptr)
			{
				itmp[j] = *ptr;
				*ptr = ptr[3];
			}

			for(unsigned int j=0; j<3; ++j, ++ptr)
			{
				*ptr = itmp[j];
			}

			if(currGrid->getTopology()->getType() == XdmfTopologyType::Wedge_18())
			{
				itmp[0] = *(ptr);
				itmp[1] = *(ptr+1);
				itmp[2] = *(ptr+2);
				*(ptr++) = itmp[2];
				*(ptr++) = itmp[0];
				*(ptr++) = itmp[1];
			}
		}
	}

	ex_put_elem_conn(exodusHandle, 10, elem_connectivity);
	delete [] elem_connectivity;

	// Write Attributes
	int numGlobalAttributes = 0;
	int numNodalAttributes = 0;
	int numElementAttributes = 0;

	std::vector<int> globalComponents;
	std::vector<int> nodalComponents;
	std::vector<int> elementComponents;
	std::vector<std::string> globalAttributeNames;
	std::vector<std::string> nodalAttributeNames;
	std::vector<std::string> elementAttributeNames;

	for(unsigned int i=0; i<currGrid->getNumberAttributes(); ++i)
	{
		boost::shared_ptr<XdmfAttribute> currAttribute = currGrid->getAttribute(i);
		if(currAttribute->getCenter() == XdmfAttributeCenter::Grid())
		{
			int numComponents = currAttribute->getSize();
			globalComponents.push_back(numComponents);
			numGlobalAttributes += numComponents;
			mImpl->constructAttributeNames(currAttribute->getName(), globalAttributeNames, numComponents);
		}
		else if(currAttribute->getCenter() == XdmfAttributeCenter::Node())
		{
			int numComponents = currAttribute->getSize() / num_nodes;
			nodalComponents.push_back(numComponents);
			numNodalAttributes += numComponents;
			mImpl->constructAttributeNames(currAttribute->getName(), nodalAttributeNames, numComponents);
		}
		else if(currAttribute->getCenter() == XdmfAttributeCenter::Cell())
		{
			int numComponents = currAttribute->getSize() / num_elem;
			elementComponents.push_back(numComponents);
			numElementAttributes += numComponents;
			mImpl->constructAttributeNames(currAttribute->getName(), elementAttributeNames, numComponents);
		}
	}

	ex_put_var_param(exodusHandle, "g", numGlobalAttributes);
	ex_put_var_param(exodusHandle, "n", numNodalAttributes);
	ex_put_var_param(exodusHandle, "e", numElementAttributes);

	char ** globalNames = new char*[numGlobalAttributes];
	char ** nodalNames = new char*[numNodalAttributes];
	char ** elementNames = new char*[numElementAttributes];

	for(int i=0; i<numGlobalAttributes; ++i)
	{
		globalNames[i] = (char*)globalAttributeNames[i].c_str();
	}

	for(int i=0; i<numNodalAttributes; ++i)
	{
		nodalNames[i] = (char*)nodalAttributeNames[i].c_str();
	}

	for(int i=0; i<numElementAttributes; ++i)
	{
		elementNames[i] = (char*)elementAttributeNames[i].c_str();
	}

	ex_put_var_names(exodusHandle, "g", numGlobalAttributes, globalNames);
	ex_put_var_names(exodusHandle, "n", numNodalAttributes, nodalNames);
	ex_put_var_names(exodusHandle, "e", numElementAttributes, elementNames);

	delete [] globalNames;
	delete [] nodalNames;
	delete [] elementNames;

	int numGrids = 1;
	if(gridCollection)
	{
		numGrids = gridCollection->getNumberGridUnstructureds();
	}

	for(int i=0; i<numGrids; ++i)
	{
		double * globalAttributeVals = new double[numGlobalAttributes];

		int globalIndex = 0;
		int globalComponentIndex = 0;
		int nodalIndex = 0;
		int nodalComponentIndex = 0;
		int elementIndex = 0;
		int elementComponentIndex = 0;

		if(gridCollection)
		{
			currGrid = gridCollection->getGridUnstructured(i);
		}

		for(unsigned int j=0; j<currGrid->getNumberAttributes(); ++j)
		{
			boost::shared_ptr<XdmfAttribute> currAttribute = currGrid->getAttribute(j);
			if(currAttribute->getCenter() == XdmfAttributeCenter::Grid())
			{
				for(int k=0; k<globalComponents[globalComponentIndex]; ++k)
				{
					currAttribute->getValues(k, globalAttributeVals + globalIndex, 1);
					globalIndex++;
				}
				globalComponentIndex++;
			}
			else if(currAttribute->getCenter() == XdmfAttributeCenter::Node())
        	{
				for(int k=0; k<nodalComponents[nodalComponentIndex]; ++k)
				{
					double * nodalValues = new double[num_nodes];
					currAttribute->getValues(k, nodalValues, num_nodes, nodalComponents[nodalComponentIndex]);
					ex_put_nodal_var(exodusHandle, i+1, nodalIndex+1, num_nodes, nodalValues);
					ex_update(exodusHandle);
					delete [] nodalValues;
					nodalIndex++;
				}
				nodalComponentIndex++;
        	}
			else if(currAttribute->getCenter() == XdmfAttributeCenter::Cell())
        	{
				for(int k=0; k<elementComponents[elementComponentIndex]; ++k)
				{
					double * elementValues = new double[num_elem];
					currAttribute->getValues(k, elementValues, num_elem, elementComponents[elementComponentIndex]);
					ex_put_elem_var(exodusHandle, i+1, elementIndex+1, 10, num_elem, elementValues);
					ex_update(exodusHandle);
					delete [] elementValues;
					elementIndex++;
				}
				elementComponentIndex++;
        	}
		}
		ex_put_glob_vars(exodusHandle, i+1, numGlobalAttributes, globalAttributeVals);
		ex_update(exodusHandle);
		delete [] globalAttributeVals;

		// Write Sets
		int setId = 20;
		for(unsigned int j=0; j<currGrid->getNumberSets(); ++j)
		{
			boost::shared_ptr<XdmfSet> currSet = currGrid->getSet(j);
			int numValues = currSet->getSize();
			std::string name = currSet->getName();
			if(name.size() > MAX_STR_LENGTH)
			{
				name = name.substr(0, MAX_STR_LENGTH);
			}
			if(currSet->getType() == XdmfSetType::Cell())
			{
				ex_put_side_set_param(exodusHandle, setId + i, numValues, 0);
				int * values = new int[numValues];
				currSet->getValues(0, values, numValues);
				for(int k=0; k<numValues; ++k)
				{
					// Add 1 to xdmf ids because exodus ids begin at 1
					values[k]++;
				}
				ex_put_side_set(exodusHandle, setId + i, values, NULL);
				ex_put_name(exodusHandle, EX_SIDE_SET, setId + i, name.c_str());
				delete [] values;
			}
			else if(currSet->getType() == XdmfSetType::Node())
			{
				ex_put_node_set_param(exodusHandle, setId + i, numValues, 0);
				int * values = new int[numValues];
				currSet->getValues(0, values, numValues);
				for(int k=0; k<numValues; ++k)
				{
					// Add 1 to xdmf ids because exodus ids begin at 1
					values[k]++;
				}
				ex_put_node_set(exodusHandle, setId + i, values);
				ex_put_name(exodusHandle, EX_NODE_SET, setId + i, name.c_str());
				delete [] values;
			}
		}
	}

	// Close Exodus File
	ex_close(exodusHandle);
}

std::string XdmfExodusWriter::xdmfToExodusTopologyType(boost::shared_ptr<const XdmfTopologyType> topologyType) const
{
	if(topologyType == XdmfTopologyType::Polyvertex())
	{
		return "SUP";
	}
	else if(topologyType == XdmfTopologyType::Triangle() || topologyType == XdmfTopologyType::Triangle_6())
	{
		return "TRIANGLE";
	}
	else if(topologyType == XdmfTopologyType::Quadrilateral() || topologyType == XdmfTopologyType::Quadrilateral_8() ||
			topologyType == XdmfTopologyType::Quadrilateral_9())
	{
		return "QUAD";
	}
	else if(topologyType == XdmfTopologyType::Tetrahedron() || topologyType == XdmfTopologyType::Tetrahedron_10())
	{
		return "TETRA";
	}
	else if(topologyType == XdmfTopologyType::Pyramid())
	{
		return "PYRAMID";
	}
	else if(topologyType == XdmfTopologyType::Wedge() || topologyType == XdmfTopologyType::Wedge_15())
	{
		return "WEDGE";
	}
	else if(topologyType == XdmfTopologyType::Hexahedron() || topologyType == XdmfTopologyType::Hexahedron_20() ||
			topologyType == XdmfTopologyType::Hexahedron_27())
	{
		return "HEX";
	}
	else if(topologyType == XdmfTopologyType::Edge_3())
	{
		return "EDGE";
	}
	else if(topologyType == XdmfTopologyType::Quadrilateral())
	{
		return "QUAD";
	}
	else if(topologyType == XdmfTopologyType::Quadrilateral())
	{
		return "QUAD";
	}
	else if(topologyType == XdmfTopologyType::Quadrilateral())
	{
		return "QUAD";
	}
	else if(topologyType == XdmfTopologyType::Quadrilateral())
	{
		return "QUAD";
	}
	else if(topologyType == XdmfTopologyType::Quadrilateral())
	{
		return "QUAD";
	}
	else if(topologyType == XdmfTopologyType::Quadrilateral())
	{
		return "QUAD";
	}
	return "";
}
