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
#ifndef SWIG

#ifndef __XdmfVtk_h
#define __XdmfVtk_h

#include "XdmfGrid.h"
#include "XdmfAttribute.h"

/*!
This is s conveniece object to handle 
common vtk tasks and getting data beteen the
XDMF Object Directory and vtk Objects. It uses
the MPI object to implement a parallel vtk ( use CollectPolygons() and GenerateXML() )
*/
class XdmfVtk : public XdmfHDF, public XdmfDom {

public:
  XdmfVtk();
  ~XdmfVtk();
  const char * GetClassName() { return ( "XdmfVtk" ) ; };

//! Get the number of Polygons after collection
  XdmfGetValueMacro( NumberOfPolygons, XdmfInt64 );
//! Get the number of Connections after collection
  XdmfGetValueMacro( NumberOfConnections, XdmfInt64 );
//! Get the number of Points after collection
  XdmfGetValueMacro( NumberOfPoints, XdmfInt64 );
//! Get the number of PointScalars after collection
  XdmfGetValueMacro( NumberOfPointScalars, XdmfInt64 );
//! Get the number of CellScalars after collection
  XdmfGetValueMacro( NumberOfCellScalars, XdmfInt64 );

//! Set the current MPI Object
  XdmfSetValueMacro( Mpi, XdmfMpi *);
//! Get the current MPI Object
  XdmfGetValueMacro( Mpi, XdmfMpi *);


/*! Write out the XML
What = "Head" | "Tail" | NULL ( the guts )
*/
  XdmfInt32 GenerateXML( XdmfString What = NULL );

//! Set the Current Polygons
  XdmfSetValueMacro( Polygons, vtkPolyData *);
//! Get the Current Polygons
  XdmfGetValueMacro( Polygons, vtkPolyData *);

//! Overloaded function for Scripting Languages
  XdmfInt32 SetPolygons( VtkScriptObject VtkObject ) {
    return( this->SetPolygons(( vtkPolyData *)VtkObject) );
    }

//! Gather Polygons
  XdmfInt32 CollectPolygons( void );

protected:

  XdmfMpi    *Mpi;
  vtkPolyData  *Polygons;
  XdmfInt64  NumberOfPolygons;
  XdmfInt64  NumberOfPoints;
  XdmfInt64  NumberOfConnections;
  XdmfInt64  NumberOfPointScalars;
  XdmfInt64  NumberOfCellScalars;
};

/*!
Get Data from a vtkPolyData Object and return it as a XdmfArray. "What" may be
  Points
  Connections
  PointScalars
  CellScalars
*/
extern XdmfArray *XdmfConvertVtkPolyData ( VtkScriptObject VtkObject , XdmfString What );
//! Get a void pointer from the Scripting Language vtk Object
extern void     *XdmfGetVtkAddress( VtkScriptObject VtkObject );
//! Get the Scripting Language interface to a vtk Pointer
extern VtkScriptObject XdmfVtkAddressToCommand( void *VtkObject);
//! Copy a XdmfArray to a vtkDataArray
extern void *XdmfCopyArrayToVtkData( XdmfArray *Array, VtkScriptObject VtkObject );
//! Copy a vtkDataArray to a XdmfArray
extern void *XdmfCopyVtkDataToArray( VtkScriptObject VtkObject, XdmfArray *Array );


#endif // __XdmfVtk_h

#else  // SWIG

class XdmfVtk : public virtual XdmfHDF, public virtual XdmfDom {

public:
  XdmfVtk();
  ~XdmfVtk();
  const char * GetClassName() { return ( "XdmfVtk" ) ; };

//! Get the number of Polygons after collection
  XdmfGetValueMacro( NumberOfPolygons, XdmfInt64 );
//! Get the number of Connections after collection
  XdmfGetValueMacro( NumberOfConnections, XdmfInt64 );
//! Get the number of Points after collection
  XdmfGetValueMacro( NumberOfPoints, XdmfInt64 );
//! Get the number of PointScalars after collection
  XdmfGetValueMacro( NumberOfPointScalars, XdmfInt64 );
//! Get the number of CellScalars after collection
  XdmfGetValueMacro( NumberOfCellScalars, XdmfInt64 );
//! Set the current MPI Object
  XdmfSetValueMacro( Mpi, XdmfMpi *);
//! Get the current MPI Object
  XdmfGetValueMacro( Mpi, XdmfMpi *);

/*! Write out the XML
What = "Head" | "Tail" | NULL ( the guts )
*/
  XdmfInt32 GenerateXML( XdmfString What = NULL );

//! Get the Current Polygons
  XdmfGetValueMacro( Polygons, vtkPolyData *);

//! Set the Current Polygons
  XdmfInt32 SetPolygons( VtkScriptObject VtkObject ) {
    return( this->SetPolygons(( vtkPolyData *)VtkObject) );
    }

//! Gather Polygons
  XdmfInt32 CollectPolygons( void );

} ;

extern XdmfArray *XdmfConvertVtkPolyData ( VtkScriptObject VtkObject , XdmfString What );
extern void     *XdmfGetVtkAddress( VtkScriptObject VtkObject );
extern VtkScriptObject XdmfVtkAddressToCommand( void *VtkObject);
extern void *XdmfCopyArrayToVtkData( XdmfArray *Array, VtkScriptObject VtkObject );
extern void *XdmfCopyVtkDataToArray( VtkScriptObject VtkObject, XdmfArray *Array );
#endif  // SWIG

