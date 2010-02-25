// Kenneth Leiter
// Xdmf Smart Pointer Test

#include "XdmfAttribute.hpp"
#include "XdmfDataItem.hpp"
#include "XdmfDomain.hpp"
#include "XdmfGeometry.hpp"
#include "XdmfGrid.hpp"
#include "XdmfTopology.hpp"
#include "XdmfVisitor.hpp"

#include <iomanip>

XdmfVisitor::XdmfVisitor() :
	mLightDataLimit(100),
	mHeavyFileName("output.h5"),
	hdf5Handle(H5Fcreate("output.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT)),
	xmlDocument(xmlNewDoc((xmlChar*)"1.0")),
	xmlCurrentNode(xmlNewNode(NULL, (xmlChar*)"Xdmf"))
{
	xmlDocSetRootElement(xmlDocument, xmlCurrentNode);
	std::cout << "Created Visitor " << this << std::endl;
}

XdmfVisitor::~XdmfVisitor()
{
	xmlSaveFormatFile("output.xmf", xmlDocument, 1);
	xmlFreeDoc(xmlDocument);
	herr_t status = H5Fclose(hdf5Handle);
	std::cout << "Deleted Visitor " << this << std::endl;
}

void XdmfVisitor::visit(const XdmfAttribute * const attribute)
{
	xmlNodePtr parentNode = xmlCurrentNode;
	xmlCurrentNode = xmlNewChild(xmlCurrentNode, NULL, (xmlChar*)"Attribute", NULL);
	xmlNewProp(xmlCurrentNode, (xmlChar*)"Name", (xmlChar*)attribute->getName().c_str());
	xmlNewProp(xmlCurrentNode, (xmlChar*)"AttributeType", (xmlChar*)attribute->getAttributeTypeAsString().c_str());
	xmlNewProp(xmlCurrentNode, (xmlChar*)"Center", (xmlChar*)attribute->getAttributeCenterAsString().c_str());

	dataHierarchy.push_back(attribute->getName());
	visit((XdmfDataItem*)attribute);
	dataHierarchy.pop_back();

	xmlCurrentNode = parentNode;
}

void XdmfVisitor::visit(const XdmfDataItem * const dataItem)
{
	xmlNodePtr parentNode = xmlCurrentNode;
	xmlCurrentNode = xmlNewChild(xmlCurrentNode, NULL, (xmlChar*)"DataItem", NULL);

	std::string format = "XML";
	if(dataItem->getNumberValues() > mLightDataLimit)
	{
		format = "HDF";
	}

	xmlNewProp(xmlCurrentNode, (xmlChar*)"Format", (xmlChar*)format.c_str());
	xmlNewProp(xmlCurrentNode, (xmlChar*)"DataType", (xmlChar*)dataItem->getName().c_str());
	std::stringstream precisionString;
	precisionString << dataItem->getPrecision();
	xmlNewProp(xmlCurrentNode, (xmlChar*)"Precision", (xmlChar*)precisionString.str().c_str());
	std::stringstream dimensionString;
	dimensionString << dataItem->getNumberValues();
	xmlNewProp(xmlCurrentNode, (xmlChar*)"Dimensions", (xmlChar*)dimensionString.str().c_str());

	const void* const pointer = dataItem->getValues();
	std::stringstream xmlTextValues;
	if(dataItem->getNumberValues() > mLightDataLimit)
	{
		herr_t status;
		hsize_t size = dataItem->getNumberValues();
		hid_t dataspace = H5Screate_simple(1, &size, NULL);
		hid_t handle = hdf5Handle;
		std::string groupName = getHDF5GroupName();
		// Need to make sure this group exists before we add to it.
		if(dataHierarchy.size() > 1)
		{
			/* Save old error handler */
			H5E_auto_t old_func;
			void* old_client_data;
			H5Eget_auto(0, &old_func, &old_client_data);

			/* Turn off error handling */
			H5Eset_auto2(0, NULL, NULL);

			/* Probe. May fail, but that's okay */
			handle = H5Gopen(hdf5Handle, groupName.c_str(), H5P_DEFAULT);

			/* Restore previous error handler */
			H5Eset_auto2(0, old_func, old_client_data);
			if(handle < 0)
			{
				handle = H5Gcreate(hdf5Handle, groupName.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
			}
		}
		hid_t dataset = H5Dcreate(handle, dataHierarchy.back().c_str(), dataItem->getHDF5DataType(), dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		xmlTextValues << mHeavyFileName << ":" << groupName << "/" << dataHierarchy.back();
		status = H5Dwrite(dataset, dataItem->getHDF5DataType(), H5S_ALL, H5S_ALL, H5P_DEFAULT, dataItem->getValues());
		status = H5Dclose(dataset);
		status = H5Sclose(dataspace);
	}
	else
	{
		for (unsigned int i=0; i<dataItem->getNumberValues(); ++i)
		{
			if (i % 10 == 9)
			{
				xmlTextValues << ((int*)pointer)[i] << "\n";
			}
			else
			{
				xmlTextValues << ((int*)pointer)[i] << " ";
			}
		}
	}

	xmlAddChild(xmlCurrentNode, xmlNewText((xmlChar*)xmlTextValues.str().c_str()));
	xmlCurrentNode = parentNode;
}

void XdmfVisitor::visit(const XdmfDomain * const domain)
{
	xmlNodePtr parentNode = xmlCurrentNode;
	xmlCurrentNode = xmlNewChild(xmlCurrentNode, NULL, (xmlChar*)"Domain", NULL);

	for(unsigned int i=0; i<domain->getNumberOfGrids(); ++i)
	{
		visit(domain->getGrid(i).get());
	}

	xmlCurrentNode = parentNode;
}

void XdmfVisitor::visit(const XdmfGeometry * const geometry)
{
	xmlNodePtr parentNode = xmlCurrentNode;
	xmlCurrentNode = xmlNewChild(xmlCurrentNode, NULL, (xmlChar*)"Geometry", NULL);
	xmlNewProp(xmlCurrentNode, (xmlChar*)"GeometryType", (xmlChar*)geometry->getGeometryTypeAsString().c_str());

	dataHierarchy.push_back("XYZ");
	visit((XdmfDataItem*)geometry);
	dataHierarchy.pop_back();

	xmlCurrentNode = parentNode;
}

void XdmfVisitor::visit(const XdmfGrid * const grid)
{
	xmlNodePtr parentNode = xmlCurrentNode;
    xmlCurrentNode = xmlNewChild(xmlCurrentNode, NULL, (xmlChar*)"Grid", NULL);
	xmlNewProp(xmlCurrentNode, (xmlChar*)"Name", (xmlChar*)grid->getName().c_str());

	dataHierarchy.push_back(grid->getName());
	visit(grid->getGeometry().get());
	visit(grid->getTopology().get());
	for(unsigned int i=0; i<grid->getNumberOfAttributes(); ++i)
	{
		visit(grid->getAttribute(i).get());
	}
	dataHierarchy.pop_back();

	xmlCurrentNode = parentNode;
}

void XdmfVisitor::visit(const XdmfTopology * const topology)
{
	xmlNodePtr parentNode = xmlCurrentNode;
	xmlCurrentNode = xmlNewChild(xmlCurrentNode, NULL, (xmlChar*)"Topology", NULL);
	xmlNewProp(xmlCurrentNode, (xmlChar*)"TopologyType", (xmlChar*)topology->getTopologyTypeAsString().c_str());
	std::stringstream numberElementsString;
	numberElementsString << topology->getNumberElements();
	xmlNewProp(xmlCurrentNode, (xmlChar*)"NumberOfElements", (xmlChar*)numberElementsString.str().c_str());

	dataHierarchy.push_back("Connectivity");
	visit((XdmfDataItem*)topology);
	dataHierarchy.pop_back();

	xmlCurrentNode = parentNode;
}

std::string XdmfVisitor::printSelf() const
{
	return "XdmfVisitor";
}

int XdmfVisitor::getLightDataLimit() const
{
	return mLightDataLimit;
}

void XdmfVisitor::setLightDataLimit(int numValues)
{
	mLightDataLimit = numValues;
}

std::string XdmfVisitor::getHDF5GroupName()
{
	std::stringstream datasetName;
	for(unsigned int i=0; i<dataHierarchy.size() - 1; ++i)
	{
		datasetName << "/" << dataHierarchy[i];
	}
	return datasetName.str();
}
