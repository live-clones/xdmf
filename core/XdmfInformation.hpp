#ifndef XDMFINFORMATION_HPP_
#define XDMFINFORMATION_HPP_

// Includes
#include "XdmfCore.hpp"
#include "XdmfItem.hpp"

/**
 * @brief Holds a key/value pair that can be attached to an Xdmf structure.
 *
 * XdmfInformation stores two strings as a key value pair. These can be used
 * to store input parameters to a code or for simple result data like wall
 * time.
 */
class XDMFCORE_EXPORT XdmfInformation : public XdmfItem {

 public:

  /**
   * Create a new XdmfInformation.
   *
   * @return constructed XdmfInformation.
   */
  static boost::shared_ptr<XdmfInformation> New();

  /**
   * Create a new XdmfInformation.
   *
   * @param key a string containing the key of the XdmfInformation to create.
   * @param value a string containing the value of the XdmfInformation to
   * create.
   *
   * @return constructed XdmfInformation
   */
  static boost::shared_ptr<XdmfInformation> New(const std::string & key,
                                                const std::string & value);

  virtual ~XdmfInformation();

  LOKI_DEFINE_VISITABLE(XdmfInformation, XdmfItem);
  static const std::string ItemTag;

  std::map<std::string, std::string> getItemProperties() const;

  virtual std::string getItemTag() const;

  /**
   * Get the key for this information item.
   *
   * @return string containing the key.
   */
  std::string getKey() const;

  /**
   * Get the value for this information item.
   *
   * @return string containing the value.
   */
  std::string getValue() const;

  /**
   * Set the key for this information item.
   *
   * @param key a string containing the key to set.
   */
  void setKey(const std::string & key);

  /**
   * Set the value for this information item.
   *
   * @param value a string containing the value to set.
   */
  void setValue(const std::string & value);

 protected:

  XdmfInformation(const std::string & key = "",
                  const std::string & value = "");

  virtual void
  populateItem(const std::map<std::string, std::string> & itemProperties,
               std::vector<boost::shared_ptr<XdmfItem> > & childItems,
               const XdmfCoreReader * const reader);

 private:

  XdmfInformation(const XdmfInformation &);  // Not implemented.
  void operator=(const XdmfInformation &);  // Not implemented.

  std::string mKey;
  std::string mValue;
};

#ifdef _WIN32
XDMFCORE_TEMPLATE template class XDMFCORE_EXPORT
boost::shared_ptr<Loki::BaseVisitor>;
XDMFCORE_TEMPLATE template class XDMFCORE_EXPORT
Loki::Visitor<boost::shared_ptr<XdmfInformation>,
              boost::shared_ptr<XdmfItem> >;
#endif

#endif /* XDMFINFORMATION_HPP_ */
