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
#ifndef __XdmfGeometry_h
#define __XdmfGeometry_h

#include "XdmfTopology.h"

#define XDMF_GEOMETRY_NONE    0
#define XDMF_GEOMETRY_XYZ    1
#define XDMF_GEOMETRY_XY    2
#define XDMF_GEOMETRY_X_Y_Z    3
#define XDMF_GEOMETRY_VXVYVZ    4
#define XDMF_GEOMETRY_ORIGIN_DXDYDZ  5



class XDMF_EXPORT XdmfGeometry : public XdmfLightData {

public:
  XdmfGeometry();
  ~XdmfGeometry();

  const char * GetClassName() { return ( "XdmfGeometry" ) ; };

  XdmfSetValueMacro( NumberOfPoints, XdmfInt64 );
  XdmfGetValueMacro( NumberOfPoints, XdmfInt64 );

  XdmfSetValueMacro( GeometryType, XdmfInt32 );
  XdmfGetValueMacro( GeometryType, XdmfInt32 );

  XdmfString GetGeometryTypeAsString( void );
  XdmfInt32 SetGeometryTypeFromString( XdmfString GeometryType );

  XdmfInt32 InitGeometryFromElement( XdmfXNode *Element );
  XdmfInt32 SetGeometryFromElement( XdmfXNode *Element );
#ifndef DOXYGEN_SKIP
  // XdmfInt32 SetBaseGeometry( XdmfTopology *Topology, XdmfPointer Grid );
#endif

  XdmfInt32 SetPoints( XdmfArray *Points );
  XdmfGetValueMacro( Points, XdmfArray *);

#ifndef SWIG
  XdmfInt32 SetOrigin( XdmfFloat64 *Origin );
  XdmfInt32 SetDxDyDz( XdmfFloat64 *DxDyDz);
#endif
  XdmfFloat64 *GetOrigin( void ) { return( this->Origin ); };
  XdmfFloat64 GetOriginX( void ) { return( this->Origin[0] ); };
  XdmfFloat64 GetOriginY( void ) { return( this->Origin[1] ); };
  XdmfFloat64 GetOriginZ( void ) { return( this->Origin[2] ); };
  XdmfInt32 SetOrigin( XdmfFloat64 X, XdmfFloat64 Y, XdmfFloat64 Z );
  XdmfInt32 SetDxDyDz( XdmfFloat64 Dx, XdmfFloat64 Dy, XdmfFloat64 Dz );
  XdmfFloat64 GetDx( void ) { return( this->DxDyDz[0] ); };
  XdmfFloat64 GetDy( void ) { return( this->DxDyDz[1] ); };
  XdmfFloat64 GetDz( void ) { return( this->DxDyDz[2] ); };
  XdmfFloat64 *GetDxDyDz( void ) { return( this->DxDyDz ); };
  

  XdmfArray *GetVectorX( void ) { return( this->VectorX ); };
  XdmfArray *GetVectorY( void ) { return( this->VectorY ); };
  XdmfArray *GetVectorZ( void ) { return( this->VectorZ ); };
  void SetVectorX( XdmfArray *Array ) { this->VectorX = Array; }; 
  void SetVectorY( XdmfArray *Array ) { this->VectorY = Array; }; 
  void SetVectorZ( XdmfArray *Array ) { this->VectorZ = Array; }; 

  XdmfInt32 HasData( void ) {
    if ( this->Points || ( this->VectorX && this->VectorY && this->VectorZ )){
      return( XDMF_SUCCESS );
    }
    return( XDMF_FAIL );
    }

  XdmfInt32 Update( void );
protected:

  XdmfInt32  GeometryType;
  XdmfInt32  PointsAreMine;
  XdmfInt64  NumberOfPoints;
  XdmfArray  *Points;
  XdmfFloat64  Origin[3];
  XdmfFloat64  DxDyDz[3];
  XdmfArray  *VectorX;
  XdmfArray  *VectorY;
  XdmfArray  *VectorZ;
};

extern XDMF_EXPORT XdmfGeometry *GetXdmfGeometryHandle( void *Pointer );
#endif // __XdmfGeometry_h
