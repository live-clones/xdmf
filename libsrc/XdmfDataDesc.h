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
#ifndef __XdmfDataDesc_h
#define __XdmfDataDesc_h

#include "XdmfObject.h"
#include "XdmfHDFSupport.h"

#define XDMF_SELECTALL    0
#define XDMF_HYPERSLAB    1
#define XDMF_COORDINATES  2


class XdmfDataDesc : public XdmfObject {


public :
  XdmfDataDesc();
  virtual ~XdmfDataDesc();

  void    Print( void );

//!  Set the type of number for homogeneous DataSets : all Float, all Int etc.
  XdmfInt32  SetNumberType( XdmfInt32 NumberType, XdmfInt64 CompoundSize = 16 );
//!  Set the type of number for homogeneous DataSets : all Float, all Int etc.
  XdmfInt32  SetNumberTypeFromString( XdmfString NumberType, XdmfInt64 CompoundSize = 16 );
//! Returns the number type
  XdmfInt32  GetNumberType( void );
//! Returns the number type as a character string
  XdmfString  GetNumberTypeAsString( void );
//! Set Rank and Dimensions of Dataset
  XdmfInt32  SetShape( XdmfInt32 Rank, XdmfInt64 *Dimensions );
  XdmfInt32  GetShape( XdmfInt64 *Dimensions );
  XdmfString  GetShapeAsString( void );
  XdmfInt32  CopySelection( XdmfDataDesc *DataDesc );
//! Convenience
  XdmfInt32  CopyShape( XdmfDataDesc *DataDesc );
  XdmfInt32  CopyType( XdmfDataDesc *DataDesc ) {
        return( this->CopyType( DataDesc->GetDataType() ) );
        }
#ifndef SWIG
#ifndef DOXYGEN_SKIP
  XdmfInt32  CopyShape( hid_t DataSpace );
  XdmfInt32  CopyType( hid_t DataType );
#endif  /* DOXYGEN_SKIP */
#endif  /* SWIG */

//! Convenience function to set shape linear
  XdmfInt32  SetNumberOfElements( XdmfInt64 Length ) {
        return( this->SetShape( 1, &Length ) );
        };
//! Set Rank and Dimensions of Dataset From a String
  XdmfInt32  SetShapeFromString( const XdmfString String  );

//! Select the Entire Dataset for Transfer
  XdmfInt32  SelectAll( void );
//! Select by Start, Stride, Count mechanism
  XdmfInt32  SelectHyperSlab( XdmfInt64 *Start, XdmfInt64 *Stride, XdmfInt64 *Count );
//! Select by Start, Stride, Count mechanism via String
  XdmfInt32  SelectHyperSlabFromString( XdmfString Start, XdmfString Stride, XdmfString Count );
  XdmfInt32  GetHyperSlab( XdmfInt64 *Start, XdmfInt64 *Stride, XdmfInt64 *Count );
  XdmfString  GetHyperSlabAsString( void );
//! Select via explicit parametric coordinates
  XdmfInt32  SelectCoordinates( XdmfInt64 NumberOfElements, XdmfInt64 *Coordinates );
//! Select via explicit parametric coordinates as a string
  XdmfInt32  SelectCoordinatesFromString( XdmfString Coordinates );
//! Return Selection coordinates 
  XdmfInt64  *GetCoordinates( XdmfInt64 Start = 0, XdmfInt64 Nelements = 0 );
//! Return Selection coordinates as a string
  XdmfString  GetCoordinatesAsString( XdmfInt64 Start =0, XdmfInt64 Nelements = 0 );
//! Get the number of selected elements
  XdmfInt64  GetSelectionSize( void );
//! Get the number of total elements in a dataset
  XdmfInt64  GetNumberOfElements( void );

//! Get the number of dimensions
  XdmfGetValueMacro( Rank, XdmfInt32 );
//! Get the length of each dimension
  XdmfGetIndexValueMacro( Dimension, XdmfInt64 );

//! Get the Start of HyperSlab Selection
  XdmfGetIndexValueMacro( Start, XdmfInt64 );
//! Get the Stride of HyperSlab Selection
  XdmfGetIndexValueMacro( Stride, XdmfInt64 );
//! Get the count of HyperSlab Selection
  XdmfGetIndexValueMacro( Count, XdmfInt64 );

//! Get the selection type ( HyperSlab / Coordinates )
  XdmfGetValueMacro( SelectionType, XdmfInt32 );
//! Get the selection type as a string
  XdmfString GetSelectionTypeAsString( void );

//! Internal Method to Get Low Level DataType
  XdmfGetValueMacro( DataType, hid_t );
//! Internal Method to Get Low Level DataSpace
  XdmfGetValueMacro( DataSpace, hid_t );

//! Get the size ( in bytes ) of the base Element
  XdmfInt64  GetElementSize( void );

//! Compound Data Type Access: SetDataType( XDMF_COMPOUND_TYPE ) must be called First
#ifndef SWIG
  XdmfInt32  AddCompoundMember( XdmfString Name,
        XdmfInt32 NumberType = XDMF_FLOAT32_TYPE,
        XdmfInt32 Rank = 1,
        XdmfInt64 *Dimensions = NULL,
        XdmfInt64 Offset = 0);
#endif

  XdmfInt32  AddCompoundMemberFromString( XdmfString Name,
        XdmfString NumberType,
        XdmfString Shape,
        XdmfInt64  Offset = 0);

//! Get the total number of members in the Compound Data Set
  XdmfInt64  GetNumberOfMembers( void );
//! Get the member name
  XdmfString  GetMemberName( XdmfInt64 Index );
//! Get the Total Number of Element in the member
  XdmfInt64  GetMemberLength( XdmfInt64 Index );
//! Get the total size ( in bytes ) of the member
  XdmfInt64  GetMemberSize( XdmfInt64 Index );
//! Get the Shape of the member
  XdmfInt32  GetMemberShape( XdmfInt64 Index, XdmfInt64 *Dimensions );
  XdmfString  GetMemberShapeAsString(  XdmfInt64 Index );
//! Get the member base number type
  XdmfInt32  GetMemberType( XdmfInt64 Index );
//! Get the member base number type as a string 
  XdmfString  GetMemberTypeAsString( XdmfInt64 Index );
//! Get the member offset
  XdmfInt64  GetMemberOffset( XdmfInt64 Index );


//! Internal Method to Copy From Exiting Type and Space
//  XdmfInt32  Copy( hid_t DataType, hid_t DataSpace );

protected:
  // HDF5 Stuff
  hid_t           DataType;
  hid_t    DataSpace;
/*
  H5T_class_t     Class;
  H5T_order_t     Order;
  size_t          Size;
*/
  XdmfInt32  SelectionType;
  XdmfInt32  NumberType;
  XdmfInt32  Rank;
  XdmfInt64  NextOffset;
  XdmfInt64  Dimension[XDMF_MAX_DIMENSION];
  hssize_t  Start[XDMF_MAX_DIMENSION];
  hsize_t    Stride[XDMF_MAX_DIMENSION];
  hsize_t    Count[XDMF_MAX_DIMENSION];
};

#endif // __XdmfDataDesc_h
