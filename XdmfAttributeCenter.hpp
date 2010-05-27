#ifndef XDMFATTRIBUTECENTER_HPP_
#define XDMFATTRIBUTECENTER_HPP_

// Includes
#include "XdmfItemProperty.hpp"

/**
 * @brief Property describing where XdmfAttribute values are centered.
 *
 * XdmfAttributeCenter is a property used by XdmfAttribute to specify where its values are centered
 * on the XdmfGrid.  A specific XdmfAttributeCenter can be created by calling on of the static methods
 * in the class, i.e. XdmfAttributeCenter::Cell().
 *
 * Xdmf supports the following attribute centers:
 * 	Grid
 * 	Cell
 * 	Face
 * 	Edge
 * 	Node
 */
class XdmfAttributeCenter : public XdmfItemProperty {

public:

	friend class XdmfAttribute;
	template <typename T> friend void boost::checked_delete(T * x);

	// Supported Xdmf Attribute Centers
	static boost::shared_ptr<const XdmfAttributeCenter> Grid();
	static boost::shared_ptr<const XdmfAttributeCenter> Cell();
	static boost::shared_ptr<const XdmfAttributeCenter> Face();
	static boost::shared_ptr<const XdmfAttributeCenter> Edge();
	static boost::shared_ptr<const XdmfAttributeCenter> Node();

	void getProperties(std::map<std::string, std::string> & collectedProperties) const;

	/**
	 * Compare two XdmfAttributeCenters for equality.
	 *
	 * @param attributeCenter an XdmfAttributeCenter to compare equality to.
	 * @return true if the XdmfAttributeCenters are equal.
	 */
	bool operator==(const XdmfAttributeCenter & attributeCenter) const;

	/**
	 * Compare two XdmfAttributeCenters for inequality.
	 *
	 * @param attributeCenter an XdmfAttributeCenter to compare inequality to.
	 * @return true if the XdmfAttributeCenters are not equal.
	 */
	bool operator!=(const XdmfAttributeCenter & attributeCenter) const;

protected:

	/**
	 * Protected constructor for XdmfAttributeCenter.  The constructor is protected because all attribute centers supported
	 * by Xdmf should be accessed through more specific static methods that construct XdmfAttributeCenters - i.e.
	 * XdmfAttributeCenter::Node().
	 *
	 * @param name the name of the XdmfAttributeCenter to construct.
	 */
	XdmfAttributeCenter(const std::string & name);
	~XdmfAttributeCenter();

private:

	XdmfAttributeCenter(const XdmfAttributeCenter & attributeCenter); // Not implemented.
	void operator=(const XdmfAttributeCenter & attributeCenter); // Not implemented.

	static boost::shared_ptr<const XdmfAttributeCenter> New(const std::map<std::string, std::string> & itemProperties);

	std::string mName;
};

#endif /* XDMFATTRIBUTECENTER_HPP_ */
