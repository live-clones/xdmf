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
#ifndef __XdmfHDF_h
#define __XdmfHDF_h

#include "XdmfHeavyData.h"
#include "XdmfArray.h"

#define XDMF_H5_DIRECTORY  H5G_GROUP
#define XDMF_H5_DATASET    H5G_DATASET
#define XDMF_H5_UNKNOWN    H5G_UNKNOWN
#define XDMF_H5_OTHER    0xFF

/*!
This is a convenience object for reading and writing
HDF5 Files. Use this to remain XDMF compliant.
Datasets in HDF5 are specified by :
  Domain:Filename:Pathname
where
  Domain = NDGM | FILE | CORE | GASS
    if Domain is not specified,
    FILE is assumed
  Filename = UNIX style Pathname of HDF5 file
  Pathname = HDF5 Pathname inside HDF5 File
*/
class XdmfHDF : public XdmfHeavyData {

public:
  XdmfHDF();
  ~XdmfHDF();

  const char * GetClassName() { return ( "XdmfHDF" ) ; };

//! Set the current internal HDF "Group" for creation
  XdmfInt32 SetCwdName( XdmfString Directory );
//! Get the current internal HDF "Group"
  XdmfGetValueMacro(CwdName, XdmfString );
//! Go to another HDF5 "Group"
  XdmfInt32 Cd( XdmfString Directory = "/"  ) {
    return( this->SetCwdName( Directory ) );
    };
//! Create an HDF5 Gourp
  XdmfInt32 Mkdir( XdmfString Name );
//! Get the number of members in the current HDF5 Group
  XdmfGetValueMacro( NumberOfChildren, XdmfInt64);
//! Get the n'th child in the current group
  XdmfString GetChild( XdmfInt64 Index ) {
    if ( Index >= this->NumberOfChildren ) {
      return( "" );
    }
    return( this->Child[ Index ] );
    };

//! Internal Call to set the name of the next child in the list
  void SetNextChild( const char *Name );

//! Internal HDF5 info
  XdmfInt32 Info( hid_t Group, const char *Name );

//! Get The Type of the Child : Directory, Dataset, ot Other
  XdmfInt32 GetChildType( XdmfInt64 Index ) {
    switch( this->Info( this->Cwd, this->GetChild( Index ) ) ) {
      case H5G_GROUP :
        return ( XDMF_H5_DIRECTORY );
      case H5G_DATASET :
        return ( XDMF_H5_DATASET );
      case XDMF_FAIL :
        return( XDMF_H5_UNKNOWN );
      default :
        break;
      }
  return( XDMF_H5_OTHER );
  };
//! Get The Type of the Child as a String 
  XdmfString GetChildTypeAsString( XdmfInt64 Index ) {
    switch( this->GetChildType( Index ) ) {
      case XDMF_H5_DIRECTORY :
        return("XDMF_H5_DIRECTORY");
      case XDMF_H5_DATASET :
        return("XDMF_H5_DATASET");
      case XDMF_H5_UNKNOWN :
        return("XDMF_H5_UNKNOWN");
      }  
  return("XDMF_H5_OTHER");
  };
//! Create a new dataset in the current Group
  XdmfInt32 CreateDataset( XdmfString Path = NULL );

//! Open an existing Dataset in a currently open HDF5 file
  XdmfInt32 OpenDataset();
//! Open an HDF5 file and OpenDataset = DataSetName
  XdmfInt32 Open( XdmfString DataSetName  = NULL ,
    XdmfString Access = NULL );
/*!
Read the cueently open dataset into and Array.
*/
  XdmfArray *Read( XdmfArray *Array = NULL );
/*!
Write to the cueently open dataset from and Array.
*/
  XdmfArray *Write( XdmfArray *Array );

//! Close the HDF5  File
  XdmfInt32 Close();

protected:
  hid_t    File;
  hid_t    Cwd;
  hid_t    Dataset;
  hid_t    CreatePlist;
  hid_t    AccessPlist;

  char    CwdName[XDMF_MAX_STRING_LENGTH];
  XdmfInt64  NumberOfChildren;
  XdmfString  Child[1024];

};

/*
extern XdmfArray *CreateArrayFromType( XdmfType *Type,
  XdmfInt64 NumberOfElements = 10 );
*/
extern XdmfArray *CopyArray( XdmfArray *Source, XdmfArray *Target = NULL );

#endif // __XdmfHDF_h
