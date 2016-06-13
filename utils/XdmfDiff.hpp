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

// C Compatible Includes
#include "XdmfUtils.hpp"

#ifdef __cplusplus

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

  XdmfDiff(const XdmfDiff &);

protected:

  XdmfDiff();

private:

  void operator=(const XdmfDiff &);  // Not implemented.

  double mAbsoluteTolerance;

};

#endif

#ifdef __cplusplus
extern "C" {
#endif

// C wrappers go here

struct XDMFDIFF; // Simply as a typedef to ensure correct typing
typedef struct XDMFDIFF XDMFDIFF;

XDMFUTILS_EXPORT XDMFDIFF * XdmfDiffNew();

XDMFUTILS_EXPORT int XdmfDiffCompare(XDMFDIFF * diff, XDMFITEM * item1, XDMFITEM * item2);

XDMFUTILS_EXPORT double XdmfDiffGetAbsoluteTolerance(XDMFDIFF * diff);

XDMFUTILS_EXPORT void XdmfDiffSetAbsoluteTolerance(XDMFDIFF * diff, double tolerance);

XDMFUTILS_EXPORT void XdmfDiffFree(XDMFDIFF * diff);

#ifdef __cplusplus
}
#endif

#endif /* XDMFDIFF_HPP_ */

