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
	mTabIndex(0),
	xmlData(),
	mLightDataLimit(100),
	mHeavyFileName("output.h5"),
	hdf5Handle(H5Fcreate("output.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT))
{
	std::cout << "Created Visitor " << this << std::endl;
}

XdmfVisitor::~XdmfVisitor()
{
	herr_t status = H5Fclose(hdf5Handle);
	std::cout << "Deleted Visitor " << this << std::endl;
}

void XdmfVisitor::visit(const XdmfAttribute * const attribute)
{
	xmlData << std::setw(mTabIndex) << "" << "<Attribute Name=\"" << attribute->getName() << "\" AttributeType=\"" << attribute->getAttributeTypeAsString() << "\" Center=\"" << attribute->getAttributeCenterAsString() << "\">\n";
	mTabIndex++;
	dataHierarchy.push_back(attribute->getName());
	visit((XdmfDataItem*)attribute);
	dataHierarchy.pop_back();
	mTabIndex--;
	xmlData << std::setw(mTabIndex) << "" << "</Attribute>\n";
}

void XdmfVisitor::visit(const XdmfDataItem * const dataItem)
{
	std::string format = "XML";
	if(dataItem->getNumberValues() > mLightDataLimit)
	{
		format = "HDF";
	}
	xmlData << std::setw(mTabIndex) << "" << "<DataItem Format=\"" << format << "\" DataType=\"" << dataItem->getName() << "\" Precision=\"" << dataItem->getPrecision() << "\" Dimensions=\"" << dataItem->getNumberValues() << "\">";

	const void* const pointer = dataItem->getValues();
	mTabIndex++;
	if(dataItem->getNumberValues() > mLightDataLimit)
	{
		std::cout << hdf5Handle << std::endl;
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
		xmlData << "\n" << std::setw(mTabIndex) << "" << mHeavyFileName << ":" << groupName << "/" << dataHierarchy.back();
		status = H5Dwrite(dataset, dataItem->getHDF5DataType(), H5S_ALL, H5S_ALL, H5P_DEFAULT, dataItem->getValues());
		status = H5Dclose(dataset);
		status = H5Sclose(dataspace);
	}
	else
	{
		//for (unsigned int i=0; i<dataItem->getNumberValues(); ++i)
		//{
		//	if (i % 10 == 0)
		//	{
		//		xmlData << "\n" << std::setw(mTabIndex) << "" << pointer[i] << " ";
		//	}
		//	else
		//	{
		//		xmlData << pointer[i] << " ";
		//	}
		//}
	}
	mTabIndex--;
	xmlData << "\n" << std::setw(mTabIndex) << "" << "</DataItem>\n";
}

void XdmfVisitor::visit(const XdmfDomain * const domain)
{
	xmlData << std::setw(mTabIndex) << "" << "<Domain>\n";
	mTabIndex++;
	for(unsigned int i=0; i<domain->getNumberOfGrids(); ++i)
	{
		visit(domain->getGrid(i).get());
	}
	mTabIndex--;
	xmlData << std::setw(mTabIndex) << "" << "</Domain>\n";
}

void XdmfVisitor::visit(const XdmfGeometry * const geometry)
{
	xmlData << std::setw(mTabIndex) << "" << "<Geometry GeometryType=\"" << geometry->getGeometryTypeAsString() << "\">\n";
	mTabIndex++;
	dataHierarchy.push_back("XYZ");
	visit((XdmfDataItem*)geometry);
	dataHierarchy.pop_back();
	mTabIndex--;
	xmlData << std::setw(mTabIndex) << "" << "</Geometry>\n";
}

void XdmfVisitor::visit(const XdmfGrid * const grid)
{
	xmlData << std::setw(mTabIndex) << "" << "<Grid Name=\"" << grid->getName() <<"\">\n";
	mTabIndex++;
	dataHierarchy.push_back(grid->getName());
	visit(grid->getGeometry().get());
	visit(grid->getTopology().get());
	for(unsigned int i=0; i<grid->getNumberOfAttributes(); ++i)
	{
		visit(grid->getAttribute(i).get());
	}
	dataHierarchy.pop_back();
	mTabIndex--;
	xmlData << std::setw(mTabIndex) << "" << "</Grid>\n";
}

void XdmfVisitor::visit(const XdmfTopology * const topology)
{
	xmlData << std::setw(mTabIndex) << "" << "<Topology TopologyType=\"" << topology->getTopologyTypeAsString() << "\" NumberOfElements=\"" << topology->getNumberElements() << "\">\n";
	mTabIndex++;
	dataHierarchy.push_back("Connectivity");
	visit((XdmfDataItem*)topology);
	dataHierarchy.pop_back();
	mTabIndex--;
	xmlData << std::setw(mTabIndex) << "" << "</Topology>\n";
}

std::string XdmfVisitor::printSelf() const
{
	return "XdmfVisitor:\n" + xmlData.str();
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
	std::cout << datasetName.str() << std::endl;
	return datasetName.str();
}
