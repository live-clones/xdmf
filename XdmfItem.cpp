
#include "XdmfItem.hpp"
#include "XdmfVisitor.hpp"

XdmfItem::XdmfItem()
{
}

XdmfItem::~XdmfItem()
{
}

void XdmfItem::traverse(boost::shared_ptr<XdmfVisitor> visitor) const
{
	return;
}
