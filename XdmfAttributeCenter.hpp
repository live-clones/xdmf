#ifndef XDMFATTRIBUTECENTER_HPP_
#define XDMFATTRIBUTECENTER_HPP_

// Includes
#include <string>

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
class XdmfAttributeCenter {

public:

	// Supported Xdmf Attribute Centers
	static XdmfAttributeCenter Grid();
	static XdmfAttributeCenter Cell();
	static XdmfAttributeCenter Face();
	static XdmfAttributeCenter Edge();
	static XdmfAttributeCenter Node();

	/**
	 * Create the name of this attribute center.
	 *
	 * @return a string containing the name.
	 */
	std::string getName() const;

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

	XdmfAttributeCenter(const XdmfAttributeCenter & attributeCenter);
	XdmfAttributeCenter& operator=(const XdmfAttributeCenter & attributeCenter);


protected:

	/**
	 * Protected constructor for XdmfAttributeCenter.  The constructor is protected because all attribute centers supported
	 * by Xdmf should be accessed through more specific static methods that construct XdmfAttributeCenters - i.e.
	 * XdmfAttributeCenter::Node().
	 *
	 * @param name the name of the XdmfAttributeCenter to construct.
	 */
	XdmfAttributeCenter(const std::string & name);

private:

	std::string mName;
};

#endif /* XDMFATTRIBUTECENTER_HPP_ */
