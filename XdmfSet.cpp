/*
 * XdmfSet.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: kleiter
 */

#include "XdmfAttribute.hpp"
#include "XdmfHDF5Controller.hpp"
#include "XdmfSet.hpp"
#include "XdmfSetType.hpp"

boost::shared_ptr<XdmfSet> XdmfSet::New()
{
	boost::shared_ptr<XdmfSet> p(new XdmfSet());
	return p;
}

XdmfSet::XdmfSet() :
	mName(""),
	mType(XdmfSetType::NoSetType())
{
}

XdmfSet::~XdmfSet()
{
}

const std::string XdmfSet::ItemTag = "Set";

boost::shared_ptr<XdmfAttribute> XdmfSet::getAttribute(const unsigned int index)
{
	return boost::const_pointer_cast<XdmfAttribute>(static_cast<const XdmfSet &>(*this).getAttribute(index));
}

boost::shared_ptr<const XdmfAttribute> XdmfSet::getAttribute(const unsigned int index) const
{
	if(index < mAttributes.size())
	{
		return mAttributes[index];
	}
	return boost::shared_ptr<XdmfAttribute>();
}

boost::shared_ptr<XdmfAttribute> XdmfSet::getAttribute(const std::string & name)
{
	return boost::const_pointer_cast<XdmfAttribute>(static_cast<const XdmfSet &>(*this).getAttribute(name));
}

boost::shared_ptr<const XdmfAttribute> XdmfSet::getAttribute(const std::string & name) const
{
	for(std::vector<boost::shared_ptr<XdmfAttribute> >::const_iterator iter = mAttributes.begin(); iter != mAttributes.end(); ++iter)
	{
		if((*iter)->getName().compare(name) == 0)
		{
			return *iter;
		}
	}
	return boost::shared_ptr<XdmfAttribute>();
}

std::map<std::string, std::string> XdmfSet::getItemProperties() const
{
	std::map<std::string, std::string> setProperties;
	setProperties["Name"] = mName;
	mType->getProperties(setProperties);
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

unsigned int XdmfSet::getNumberAttributes() const
{
	return mAttributes.size();
}

boost::shared_ptr<const XdmfSetType> XdmfSet::getType() const
{
	return mType;
}

void XdmfSet::insert(const boost::shared_ptr<XdmfAttribute> attribute)
{
	mAttributes.push_back(attribute);
}

void XdmfSet::populateItem(const std::map<std::string, std::string> & itemProperties, std::vector<boost::shared_ptr<XdmfItem> > & childItems, const XdmfCoreReader * const reader)
{
	XdmfItem::populateItem(itemProperties, childItems, reader);
	std::map<std::string, std::string>::const_iterator name = itemProperties.find("Name");
	if(name != itemProperties.end())
	{
		mName = name->second;
	}
	else
	{
		assert(false);
	}
	mType = XdmfSetType::New(itemProperties);
	for(std::vector<boost::shared_ptr<XdmfItem> >::const_iterator iter = childItems.begin(); iter != childItems.end(); ++iter)
	{
		if(boost::shared_ptr<XdmfArray> array = boost::shared_dynamic_cast<XdmfArray>(*iter))
		{
			this->swap(array);
			// TODO: If multiple dataitems.
		}
		else if(boost::shared_ptr<XdmfAttribute> attribute = boost::shared_dynamic_cast<XdmfAttribute>(*iter))
		{
			this->insert(attribute);
		}
	}
}

void XdmfSet::removeAttribute(const unsigned int index)
{
	if(index < mAttributes.size())
	{
		mAttributes.erase(mAttributes.begin() + index);
	}
}

void XdmfSet::removeAttribute(const std::string & name)
{
	for(std::vector<boost::shared_ptr<XdmfAttribute> >::iterator iter = mAttributes.begin(); iter != mAttributes.end(); ++iter)
	{
		if((*iter)->getName().compare(name) == 0)
		{
			mAttributes.erase(iter);
			return;
		}
	}
	return;
}

void XdmfSet::setName(const std::string & name)
{
	mName = name;
}

void XdmfSet::setType(const boost::shared_ptr<const XdmfSetType> type)
{
	mType = type;
}

void XdmfSet::traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
	XdmfItem::traverse(visitor);
	for(std::vector<boost::shared_ptr<XdmfAttribute> >::const_iterator iter = mAttributes.begin(); iter != mAttributes.end(); ++iter)
	{
		(*iter)->accept(visitor);
	}
}
