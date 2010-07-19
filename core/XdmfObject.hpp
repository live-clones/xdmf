#ifndef XDMFOBJECT_HPP_
#define XDMFOBJECT_HPP_

// Includes
#include <iostream>
#include <boost/shared_ptr.hpp>

#define XdmfNewMacro(type) \
static boost::shared_ptr<type> New() \
{ \
	boost::shared_ptr<type> p(new type()); \
	return p; \
}

/**
 * @brief Root Xdmf Class.
 *
 * XdmfObject is the root Xdmf class which all Xdmf classes are derived from.
 */
class XdmfObject {

public:

	virtual ~XdmfObject();

	/**
	 * Gets the current working directory Xdmf is running in.  This is useful for reading and writing files containing relative paths to hdf5 datasets.
	 *
	 * @return the current working directory.
	 */
	static std::string getCWD();

protected:

	XdmfObject();

private:

	XdmfObject(const XdmfObject & object);  // Not implemented.
	void operator=(const XdmfObject & object);  // Not implemented.

};

#endif /* XDMFOBJECT_HPP_ */
