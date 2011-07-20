/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfDiff.hpp                                                        */
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

#ifndef XDMFDIFF_HPP_
#define XDMFDIFF_HPP_

// Forward Declarations
class XdmfItem;

// Includes
#include <string>
#include "XdmfUtils.hpp"
#include "XdmfSharedPtr.hpp"

/**
 * @brief Compare differences between data stored in two Xdmf
 * structures.
 */
class XDMFUTILS_EXPORT XdmfDiff {

public:

  /**
   * Create a new XdmfDiff.
   *
   * @return constructed XdmfDiff.
   */
  static shared_ptr<XdmfDiff> New();

  virtual ~XdmfDiff();

  /**
   * Compare the contents of two Xdmf structures.
   *
   * @param item1 an Xdmf structure to compare.
   * @param item2 an Xdmf structure to compare.
   *
   * @return true if the Xdmf structures are equivalent.
   */
  bool
  compare(const shared_ptr<XdmfItem> item1,
          const shared_ptr<XdmfItem> item2) const;

  /**
   * Get the absolute tolerance used for comparing values between Xdmf
   * structures.
   *
   * @return double the absolute tolerance.
   */
  double
  getAbsoluteTolerance() const;

  /**
   * Set the absolute tolerance for comparing values between Xdmf
   * structures.
   *
   * @param absoluteTolerance the absolute tolerance to set.
   */
  void
  setAbsoluteTolerance(const double absoluteTolerance);

protected:

  XdmfDiff();

private:

  XdmfDiff(const XdmfDiff &);  // Not implemented.
  void operator=(const XdmfDiff &);  // Not implemented.

  double mAbsoluteTolerance;

};

#endif /* XDMFDIFF_HPP_ */

