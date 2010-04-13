// Kenneth Leiter
// Xdmf Smart Pointer Test

#ifndef XDMFDOMAIN_HPP_
#define XDMFDOMAIN_HPP_

// Forward Declarations
class XdmfGrid;

// Includes
#include "XdmfItem.hpp"
#include <vector>

class XdmfDomain : public XdmfItem {

public:

	XdmfNewMacro(XdmfDomain);

	/**
	 * Insert a grid into the domain.
	 */
	void insert(boost::shared_ptr<XdmfGrid>);

	/**
	 * Get a grid attached to this domain.
	 *
	 * @param index of the grid to retrieve.
	 * @return pointer to the grid attached to this domain.
	 */
	boost::shared_ptr<XdmfGrid> getGrid(unsigned int index);

	/**
	 * Get a grid attached to this domain (const version).
	 *
	 * @param index of the grid to retrieve.
	 * @return pointer to the grid attached to this domain.
	 */
	boost::shared_ptr<const XdmfGrid> getGrid(unsigned int index) const;

	/**
	 * Get the number of grids attached to this domain
	 */
	unsigned int getNumberOfGrids() const;

	virtual std::string printSelf() const;

	virtual void traverse(boost::shared_ptr<XdmfVisitor> visitor) const;
	virtual void write(boost::shared_ptr<XdmfVisitor> visitor) const;

protected:

	XdmfDomain();
	virtual ~XdmfDomain();

private:

	XdmfDomain(const XdmfDomain&);  // Not implemented.
	void operator=(const XdmfDomain&);  // Not implemented.

	std::vector<boost::shared_ptr<XdmfGrid> > mGrids;
};

#endif /* XDMFDOMAIN_HPP_ */
