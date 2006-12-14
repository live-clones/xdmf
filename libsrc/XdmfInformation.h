/*******************************************************************/
/*                               XDMF                              */
/*                   eXtensible Data Model and Format              */
/*                                                                 */
/*  Id : Id  */
/*  Date : $Date$ */
/*  Version : $Revision$ */
/*                                                                 */
/*  Author:                                                        */
/*     Jerry A. Clarke                                             */
/*     clarke@arl.army.mil                                         */
/*     US Army Research Laboratory                                 */
/*     Aberdeen Proving Ground, MD                                 */
/*                                                                 */
/*     Copyright @ 2002 US Army Research Laboratory                */
/*     All Rights Reserved                                         */
/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
/*                                                                 */
/*     This software is distributed WITHOUT ANY WARRANTY; without  */
/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
/*     for more information.                                       */
/*                                                                 */
/*******************************************************************/
#ifndef __XdmfInformation_h
#define __XdmfInformation_h

#include "XdmfElement.h"

typedef XdmfPointer XdmfXmlNode;

//! Base object for Light Data (XML)
/*!
This is an abstract convenience object for reading and writing
Information Files. Information "points" to HeavyData ; the giga-terabytes of HPC simulations.

A XdmfInformation Object is not used by itself. Rather one of the derived
classes like XdmfGrid or XdmfFormatMulti is used and these derived methods
are used from that class.
*/

class XDMF_EXPORT XdmfInformation : public XdmfElement {

public:
  XdmfInformation();
  ~XdmfInformation();

  XdmfConstString GetClassName() { return ( "XdmfInformation" ) ; };

//! Set the current name
    XdmfSetStringMacro(Name);
//! Get the current name
    XdmfGetValueMacro(Name, XdmfConstString);

//! Update From XML
    XdmfInt32 UpdateInformation();

/*! Get the Value, either the "Value" Attribute or the CDATA if there is no
    "Value" Attribute.
*/
    XdmfConstString GetValue();


protected:
};

#endif // __XdmfInformation_h
