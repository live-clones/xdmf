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
#ifndef __XdmfHeavyData_h
#define __XdmfHeavyData_h

#include "XdmfArray.h"

//! Container class for Heavy Data Access
/*!
This is an abstract convenience object for reading and writing
HeavyData Files. 
Datasets in HeavyDat are specified by :
\verbatim
  Domain:Filename:Pathname
where
  Domain = NDGM | FILE | CORE | GASS
    if Domain is not specified,
    FILE is assumed
  Filename = UNIX style Pathname of HeavyDat file
  Pathname = HeavyData Pathname inside HeavyData File
\endverbatim
*/
class XDMF_EXPORT XdmfHeavyData : public XdmfDataDesc {

public:
  XdmfHeavyData();
  ~XdmfHeavyData();

  XdmfConstString GetClassName() { return ( "XdmfHeavyData" ) ; };

//! Get the default NDGM Host for NDGM:File:/Dataset
        XdmfGetValueMacro(NdgmHost, XdmfString);
//! Set the default NDGM Host for NDGM:File:/Dataset
        void SetNdgmHost( XdmfString String ) { strcpy( this->NdgmHost, String ); }

//! Get the default Pathname for File:/Dataset
        XdmfGetValueMacro(WorkingDirectory, XdmfString);
//! Set the default Pathname for File:/Dataset
        void SetWorkingDirectory( XdmfConstString String );


//! Get the current domain
  XdmfGetValueMacro(Domain, XdmfString);
//! Set the current domain
  void SetDomain( XdmfString Domain ) {
    strcpy( this->Domain, Domain );
    } ;

//! Get the current filename
  XdmfGetValueMacro(FileName, XdmfString);
//! Set the current filename
  void SetFileName( XdmfString File );

//! Get the current HeavyData Dataset path
  XdmfGetValueMacro(Path, XdmfString);
//! Set the current HeavyData Dataset path
  void SetPath( XdmfString Path ) {
    strcpy( this->Path, Path );
    } ;

/*!
Get the current read/write access
values can be :
  "r"
  "w"
  "rw"
*/
  XdmfGetValueMacro(Access, XdmfString);
//! Set the access permissions
  void SetAccess( XdmfString Access ) {
    strcpy( this->Access, Access );
    } ;

protected:

  char    NdgmHost[XDMF_MAX_STRING_LENGTH];
  XdmfString WorkingDirectory;
  char    Access[XDMF_MAX_STRING_LENGTH];
  char    Domain[XDMF_MAX_STRING_LENGTH];
  XdmfString FileName;
  char    Path[XDMF_MAX_STRING_LENGTH];
};

/*
extern "C" {
extern XdmfString XdmfGetNdgmEntries( void );
extern void XdmfDeleteAllNdgmEntries( void );
extern XdmfInt64 XdmfAddNdgmEntry( XdmfString Name, XdmfInt64 Length );
  }
*/
#endif // __XdmfHeavyData_h
