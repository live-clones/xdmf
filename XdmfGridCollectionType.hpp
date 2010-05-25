#ifndef XDMFGRIDCOLLECTIONTYPE_HPP_
#define XDMFGRIDCOLLECTIONTYPE_HPP_

// Includes
#include "XdmfItemProperty.hpp"

/**
 * @brief Property describing the type of an XdmfGridCollection.
 *
 * XdmfGridCollectionType is a property used by XdmfGridCollection to specify what type of collection the
 * XdmfGridCollection contains.  A specific XdmfGridCollectionType can be created by calling one of the static methods
 * in the class, i.e. XdmfGridCollectionType::Temporal().
 *
 * Xdmf supports the following collection types:
 * 	NoCollectionType
 * 	Spatial
 * 	Temporal
 */
class XdmfGridCollectionType : public XdmfItemProperty {

public:

	friend class XdmfGridCollection;

	// Supported XdmfGridCollectionTypes
	static XdmfGridCollectionType NoCollectionType();
	static XdmfGridCollectionType Spatial();
	static XdmfGridCollectionType Temporal();

	void getProperties(std::map<std::string, std::string> & collectedProperties) const;

	/*
	 * Compare two XdmfGridCollectionTypes for equality.
	 *
	 * @param collectionType an XdmfGridCollectionType to compare equality to.
	 * @return true if the XdmfGridCollectionTypes are equal.
	 */
	bool operator==(const XdmfGridCollectionType & collectionype) const;

	/**
	 * Compare two XdmfGridCollectionTypes for inequality.
	 *
	 * @param collectionType an XdmfGridCollectionType to compare inequality to.
	 * @return true if the XdmfGridCollectionTypes are not equal.
	 */
	bool operator!=(const XdmfGridCollectionType & collectionType) const;

	XdmfGridCollectionType(const XdmfGridCollectionType & collectionType);
	XdmfGridCollectionType & operator=(const XdmfGridCollectionType & collectionType);

protected:

	/**
	 * Protected constructor for XdmfGridCollectionType.  The constructor is protected because all collection types supported
	 * by Xdmf should be accessed through more specific static methods that construct XdmfGridCollectionType - i.e.
	 * XdmfGridCollectionType::Temporal().
	 *
	 * @param name the name of the XdmfGridCollectionType to construct.
	 */
	XdmfGridCollectionType(const std::string & name);

private:

	static XdmfGridCollectionType New(const std::map<std::string, std::string> & itemProperties);

	std::string mName;
};

#endif /* XDMFGRIDCOLLECTIONTYPE_HPP_ */
