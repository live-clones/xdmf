/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfSharedPtr.hpp                                                   */
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

#ifndef XDMFSHAREDPTR_HPP_
#define XDMFSHAREDPTR_HPP_

#include "XdmfConfig.hpp"

#ifdef HAVE_CXX11_SHARED_PTR

#include <memory>
using std::shared_ptr;
using std::const_pointer_cast;

#else

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;
using boost::const_pointer_cast;
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>

#endif /*  */

/*
#ifdef HAVE_CXX11_VARIANT

using std::variant;

#else
*/
//#include <boost/variant.hpp>
//using boost::variant;

//#endif

// Tokenizer
// http://en.cppreference.com/w/cpp/string/byte/strtok

#ifdef HAVE_BOOST_SHARED_DYNAMIC_CAST

using boost::shared_dynamic_cast;

#else

template <typename T, typename U>
shared_ptr<T> shared_dynamic_cast(shared_ptr<U> const & r) 
{
  typedef typename shared_ptr<T>::element_type E;
  E * p = dynamic_cast< E* >( r.get() );
  return p? shared_ptr<T>( r, p ): shared_ptr<T>();
}

#endif /* HAVE_BOOST_SHARED_DYNAMIC_CAST */

#ifdef HAVE_CXX11_ENABLE_IF

#include <type_traits>
using std::is_floating_point;
using std::is_signed;
using std::is_unsigned;
using std::is_same;
#define enable_if_c std::enable_if

using std::remove_const;

#else

#include "boost/utility.hpp"
#include "boost/type_traits.hpp"
using boost::enable_if_c;
using boost::is_floating_point;
using boost::is_signed;
using boost::is_unsigned;
using boost::is_same;

using boost::remove_const;

#endif /* HAVE_CXX11_ENABLE_IF */

// Used by C wrappers to prevent shared pointers from prematurely deleting objects
// Normally this would be completely against the point of shared pointers,
// but the  C wrapping requires that objects be seperated from the shared pointers.
struct XdmfNullDeleter
{
template<typename T>
void operator()(T*) {}
};

#endif /* XDMFSHAREDPTR_HPP_ */
