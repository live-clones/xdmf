#include "XdmfItem.hpp"

XdmfItem::XdmfItem()
{
}

XdmfItem::~XdmfItem()
{
}

void XdmfItem::traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor) const
{
	return;
}
