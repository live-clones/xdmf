// Kenneth Leiter
// Xdmf Smart Pointer Test

#include "XdmfItem.hpp"
#include "XdmfVisitor.hpp"

XdmfVisitor::XdmfVisitor()
{
}

XdmfVisitor::~XdmfVisitor()
{
}

void
XdmfVisitor::visit(XdmfItem & item,
                   const boost::shared_ptr<XdmfBaseVisitor> visitor)
{
  item.traverse(visitor);
}
