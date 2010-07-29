/*
 * XdmfSet.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfArray.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfSet.hpp"
#include "XdmfSetType.hpp"

boost::shared_ptr<XdmfSet> XdmfSet::New()
{
	boost::shared_ptr<XdmfSet> p(new XdmfSet());
	return p;
}

XdmfSet::XdmfSet() :
	mHDF5Controller(boost::shared_ptr<XdmfHDF5Controller>()),
	mName(""),
	mSetType(XdmfSetType::NoSetType())
{
}

XdmfSet::~XdmfSet()
{
}

const std::string XdmfSet::ItemTag = "Set";

boost::shared_ptr<XdmfHDF5Controller> XdmfSet::getHDF5Controller()
{
	return boost::const_pointer_cast<XdmfHDF5Controller>(static_cast<const XdmfSet &>(*this).getHDF5Controller());
}

boost::shared_ptr<const XdmfHDF5Controller> XdmfSet::getHDF5Controller() const
{
	return mHDF5Controller;
}

std::map<std::string, std::string> XdmfSet::getItemProperties() const
{
	std::map<std::string, std::string> setProperties;
	setProperties["Name"] = mName;
	mSetType->getProperties(setProperties);
	return setProperties;
}

std::string XdmfSet::getItemTag() const
{
	return ItemTag;
}

std::string XdmfSet::getName() const
{
	return mName;
}

boost::shared_ptr<const XdmfSetType> XdmfSet::getSetType() const
{
	return mSetType;
}

bool XdmfSet::isInitialized() const
{
	return this->size() > 0;
}

void XdmfSet::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems)
{
	std::map<std::string, std::string>::const_iterator name = itemProperties.find("Name");
	if(name != itemProperties.end())
	{
		mName = name->second;
	}
	else
	{
		assert(false);
	}
	for(std::vector<boost::shared_ptr<XdmfItem> >::const_iterator iter = childItems.begin(); iter != childItems.end(); ++iter)
	{
		if(boost::shared_ptr<XdmfArray> array = boost::shared_dynamic_cast<XdmfArray>(*iter))
		{
			if(array->isInitialized())
			{
				for(unsigned int i=0; i<array->getSize(); ++i)
				{
					this->insert(array->getValueCopy<unsigned int>(i));
				}
			}
			else
			{
				mHDF5Controller = array->getHDF5Controller();
			}
		}
	}
	mSetType = XdmfSetType::New(itemProperties);
}

void XdmfSet::read()
{
	if(mHDF5Controller)
	{
		boost::shared_ptr<XdmfArray> setValues = XdmfArray::New();
		mHDF5Controller->read(setValues.get());
		for(unsigned int i=0; i<setValues->getSize(); ++i)
		{
			this->insert(setValues->getValueCopy<unsigned int>(i));
		}
	}
}

void XdmfSet::release()
{
	std::set<unsigned int>::clear();
}

void XdmfSet::setHDF5Controller(const boost::shared_ptr<XdmfHDF5Controller> hdf5Controller)
{
	mHDF5Controller = hdf5Controller;
}

void XdmfSet::setName(const std::string & name)
{
	mName= name;
}

void XdmfSet::setSetType(const boost::shared_ptr<const XdmfSetType> setType)
{
	mSetType = setType;
}

std::size_t XdmfSet::size() const
{
	if(std::set<unsigned int>::size() != 0)
	{
		return std::set<unsigned int>::size();
	}
	return mHDF5Controller->getSize();
}
