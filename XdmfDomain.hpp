/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfDomain.hpp                                                      */
/*                                                                           */
/*  Author:                                                                  */
/*     Kenneth Leiter                                                        */
/*     kenneth.leiter@arl.army.mil                                           */
/*     US Army Research Laboratory                                           */
/*     Aberdeen Proving Ground, MD                                           */
/*                                                                           */
/*     Copyright @ 2011 US Army Research Laboratory                          */
/*     All Rights Reserved                                                   */
/*     See Copyright.txt for details                                         */
/*                                                                           */
/*     This software is distributed WITHOUT ANY WARRANTY; without            */
/*     even the implied warranty of MERCHANTABILITY or FITNESS               */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice             */
/*     for more information.                                                 */
/*                                                                           */
/*****************************************************************************/

#ifndef XDMFDOMAIN_HPP_
#define XDMFDOMAIN_HPP_

// Forward Declarations
class XdmfCurvilinearGrid;
class XdmfGridCollection;
class XdmfRectilinearGrid;
class XdmfRegularGrid;
class XdmfUnstructuredGrid;

// Includes
#include "Xdmf.hpp"
#include "XdmfItem.hpp"

/**
 * @brief The root XdmfItem that holds XdmfGrids.
 *
 * XdmfDomain is the top XdmfItem in an Xdmf structure.  It can store
 * a number of grids and provides methods to insert, retrieve, and
 * remove these grids.
 */
class XDMF_EXPORT XdmfDomain : public virtual XdmfItem {

 public:

  /**
   * Create a new XdmfDomain.
   *
   * @return constructed XdmfDomain.
   */
  static boost::shared_ptr<XdmfDomain> New();

  virtual ~XdmfDomain();

  LOKI_DEFINE_VISITABLE(XdmfDomain, XdmfItem);
  XDMF_CHILDREN(XdmfGridCollection, GridCollection, Name);
  XDMF_CHILDREN(XdmfCurvilinearGrid, CurvilinearGrid, Name);
  XDMF_CHILDREN(XdmfRectilinearGrid, RectilinearGrid, Name);
  XDMF_CHILDREN(XdmfRegularGrid, RegularGrid, Name);
  XDMF_CHILDREN(XdmfUnstructuredGrid, UnstructuredGrid, Name);
  static const std::string ItemTag;

  std::map<std::string, std::string> getItemProperties() const;

  virtual std::string getItemTag() const;

  using XdmfItem::insert;

  virtual void traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor);

 protected:

  XdmfDomain();
  virtual void
  populateItem(const std::map<std::string, std::string> & itemProperties,
               std::vector<boost::shared_ptr<XdmfItem> > & childItems,
               const XdmfCoreReader * const reader);

 private:

  XdmfDomain(const XdmfDomain &);  // Not implemented.
  void operator=(const XdmfDomain &);  // Not implemented.

};

#ifdef _WIN32
XDMF_TEMPLATE template class XDMF_EXPORT
std::allocator<boost::shared_ptr<XdmfGridCollection> >;
XDMF_TEMPLATE template class XDMF_EXPORT
std::vector<boost::shared_ptr<XdmfGridCollection>,
            std::allocator<boost::shared_ptr<XdmfGridCollection> > >;
XDMF_TEMPLATE template class XDMF_EXPORT
std::allocator<boost::shared_ptr<XdmfCurvilinearGrid> >;
XDMF_TEMPLATE template class XDMF_EXPORT
std::vector<boost::shared_ptr<XdmfCurvilinearGrid>,
            std::allocator<boost::shared_ptr<XdmfCurvilinearGrid> > >;
XDMF_TEMPLATE template class XDMF_EXPORT
std::allocator<boost::shared_ptr<XdmfRectilinearGrid> >;
XDMF_TEMPLATE template class XDMF_EXPORT
std::vector<boost::shared_ptr<XdmfRectilinearGrid>,
            std::allocator<boost::shared_ptr<XdmfRectilinearGrid> > >;
XDMF_TEMPLATE template class XDMF_EXPORT
std::allocator<boost::shared_ptr<XdmfRegularGrid> >;
XDMF_TEMPLATE template class XDMF_EXPORT
std::vector<boost::shared_ptr<XdmfRegularGrid>,
            std::allocator<boost::shared_ptr<XdmfRegularGrid> > >;
XDMF_TEMPLATE template class XDMF_EXPORT
std::allocator<boost::shared_ptr<XdmfUnstructuredGrid> >;
XDMF_TEMPLATE template class XDMF_EXPORT
std::vector<boost::shared_ptr<XdmfUnstructuredGrid>,
            std::allocator<boost::shared_ptr<XdmfUnstructuredGrid> > >;
#endif

#endif /* XDMFDOMAIN_HPP_ */
