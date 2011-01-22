#ifndef XDMFSET_HPP_
#define XDMFSET_HPP_

// Forward Declarations
class XdmfAttribute;
class XdmfHDF5Controller;
class XdmfSetType;

// Includes
#include "Xdmf.hpp"
#include "XdmfArray.hpp"

/**
 * @brief Holds a collection of individual nodes, cells, faces, or edges that
 * are part of an XdmfGrid.
 *
 * An XdmfSet holds a collection of nodes, cells, faces, or edges that are
 * part of an XdmfGrid. For instance, a simulation may want to hold a set of
 * nodes on a boundary. The individual elements making up the set are
 * determined by their id. An XdmfSet can have XdmfAttributes attached that
 * contain extra values attached to the elements in the set.
 */
class XDMF_EXPORT XdmfSet : public XdmfArray {

 public:

  /**
   * Create a new XdmfSet.
   *
   * @return constructed XdmfSet.
   */
  static boost::shared_ptr<XdmfSet> New();

  virtual ~XdmfSet();

  LOKI_DEFINE_VISITABLE(XdmfSet, XdmfArray);
  XDMF_CHILDREN(XdmfAttribute, Attribute, Name);
  static const std::string ItemTag;

  std::map<std::string, std::string> getItemProperties() const;

  std::string getItemTag() const;

  /**
   * Get the name of the set.
   *
   * @return a string containing the name of the set.
   */
  std::string getName() const;

  /**
   * Get the XdmfSetType associated with this set.
   *
   * @return XdmfSetType of this set.
   */
  boost::shared_ptr<const XdmfSetType> getType() const;

  using XdmfArray::insert;
  using XdmfItem::insert;

  /**
   * Set the name of the set.
   *
   * @param name a string containing the name to set.
   */
  void setName(const std::string & name);

  /**
   * Set the XdmfSetType associated with this set.
   *
   * @param type the XdmfSetType to set.
   */
  void setType(const boost::shared_ptr<const XdmfSetType> type);

  void traverse(const boost::shared_ptr<XdmfBaseVisitor> visitor);

 protected:

  XdmfSet();

  virtual void
  populateItem(const std::map<std::string, std::string> & itemProperties,
               std::vector<boost::shared_ptr<XdmfItem> > & childItems,
               const XdmfCoreReader * const reader);

 private:

  XdmfSet(const XdmfSet &);  // Not implemented.
  void operator=(const XdmfSet &);  // Not implemented.

  std::string mName;
  boost::shared_ptr<const XdmfSetType> mType;
};

#ifdef _WIN32
XDMF_TEMPLATE template class XDMF_EXPORT
std::allocator<boost::shared_ptr<XdmfAttribute> >;
XDMF_TEMPLATE template class XDMF_EXPORT
std::vector<boost::shared_ptr<XdmfAttribute>,
            std::allocator<boost::shared_ptr<XdmfAttribute> > >;
XDMF_TEMPLATE template class XDMF_EXPORT
boost::shared_ptr<const XdmfSetType>;
#endif

#endif /* XDMFSET_HPP_ */
