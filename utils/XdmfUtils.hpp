/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : Xdmf.hpp                                                            */
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

#ifndef _XDMFUTILS_HPP
#define _XDMFUTILS_HPP

/* Keep all our Win32 Conversions here */
#ifdef _WIN32
/* Used to export/import from the dlls */
#undef XDMFCORE_EXPORT
#define XDMFCORE_EXPORT __declspec(dllimport)
#undef XDMFCORE_TEMPLATE
#define XDMFCORE_TEMPLATE extern

#undef XDMF_EXPORT
#define XDMF_EXPORT __declspec(dllimport)
#undef XDMF_TEMPLATE
#define XDMF_TEMPLATE extern

#ifdef XdmfUtils_EXPORTS
#define XDMFUTILS_EXPORT __declspec(dllexport)
#define XDMFUTILS_TEMPLATE
#else /* XdmfUtils_EXPORTS */
#define XDMFUTILS_EXPORT __declspec(dllimport)
#define XDMFUTILS_TEMPLATE extern
#endif /* XdmfUtils_EXPORTS */

/* Compiler Warnings */
#ifndef XDMF_DEBUG
#pragma warning( disable : 4231 ) /* nonstandard extension used : 'extern' before template explicit instantiation */
#pragma warning( disable : 4251 ) /* needs to have dll-interface to be used by clients (Most of these guys are in private */
#pragma warning( disable : 4275 ) /* non dll-interface class 'std::_Container_base_aux' used as base for dll-interface class */
#pragma warning( disable : 4373 ) /* virtual function overrides,  parameters only differed by const/volatile qualifiers */
#pragma warning( disable : 4748 ) /* /GS can not protect parameters and local variables from local buffer overrun (turned off op)*/
#endif /* XDMF_DEBUG */

/* Compiler Optimizations will result in an 'internal compiler error', so turn them off */
#pragma optimize("g", off)

#else /* _WIN32 */
/* We don't need to export/import since there are no dlls */
#define XDMFCORE_EXPORT
#define XDMF_EXPORT
#define XDMFUTILS_EXPORT
#define XDMFCORE_TEMPLATE
#define XDMF_TEMPLATE
#define XDMFUTILS_TEMPLATE

#endif /* _WIN32 */
#endif /* _XDMFUTILS_HPP */
