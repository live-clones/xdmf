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
#ifndef __XdmfGrid_h
#define __XdmfGrid_h

#include "XdmfTopology.h"

class XdmfGeometry;
class XdmfAttribute;

//! In memory representation of an XDMF Grid
/*!
	XdmfGrid is the in memory representation of the Xdmf Grid
	structure defined in the XML. XdmfGrids have \b Topolgy (i.e. 
	what type of grid and the connectivity if it's unstructured )
	\b Geometry ( the XYZ values for the grid nodes ) and zero
	or more \b Attributes (the computed values such ad scalars, 
	vectors, tensors, etc.)

	The XML for a Grid might look like :
\verbatim
<Grid Name="Sphere of Tets"
>

    <Topology Type="Tetrahedron"
     NumberOfElements="1838"
     BaseOffset="1" >

        <DataStructure Format="HDF"
         Dimensions="1838 4"
         DataType="Int" >
                        Shapes.h5:/Block 1/Connections
        </DataStructure>
    </Topology>
    <Geometry Type="XYZ" >

        <DataStructure Format="HDF"
         Dimensions="1309 3"
         DataType="Float" >
                Shapes.h5:/Geometry
        </DataStructure>
    </Geometry>
 

    <!-- ReUse the Geometry as a Scalar Value of X Position -->

    <Attribute Type="Scalar" Center="Node" Name="X Position">
        <DataTransform Dimensions="1309 1" Type="HyperSlab" >
            <DataStructure Format="XML" Dimensions="2 3">
                0 0 1 3 1309 1
            </DataStructure>
            <DataStructure Format="HDF"
                 Dimensions="1309 3"
                 DataType="Float" >
                Shapes.h5:/Geometry
            </DataStructure>
        </DataTransform>
    </Attribute>
</Grid>

\endverbatim

	Typical API usage might look like :
	\code

	XdmfDOM	*DOM = new XdmfDOM()
	XdmfGrid *Grid = new XdmfGrid()
	XdmfAttribute *XPos;
	XdmfXNode *GridNode;

	DOM->SetInputFileName("MyData.xmf");
	DOM->Parse()
	GridNode = DOM->FindElement("Grid");
	Grid->SetGridFromElement(GridNode);
	cout << "First Grid has " << Grid->GetNumberOfAttributes() << " Attributes" << endl;
	Grid->AssignAttributeByName("X Position");
	XPos = Grid->GetAssignedAttribute();
	
	\endcode
*/
class XDMF_EXPORT XdmfGrid : public XdmfTopology {

public:
  XdmfGrid();
  ~XdmfGrid();

  XdmfConstString GetClassName() { return ( "XdmfGrid" ) ; };

//! Explicitly set the XdmfGeometry for an XdmfGrid
  XdmfSetValueMacro( Geometry, XdmfGeometry *);
//! Get the XdmfGeometry for an XdmfGrid
  XdmfGetValueMacro( Geometry, XdmfGeometry *);
//! Get the number of Attributes defined for this grid.
/*!
	Attributes can be Scalars(1 value), Vectors(3 values),
	Tensors(9 values), or Matrix(NxM array). Attributes can be centered
	on the Node, Cell, Edge, Face, or Grid.
*/
  XdmfGetValueMacro( NumberOfAttributes, XdmfInt32 );
//! Retreive a particilar XdmfAttribute
/*!
	Returns the Xdmf Attribute from the grid.
	\param Index	0 based index of the Attribute to retreive
*/
  XdmfGetIndexValueMacro( Attribute, XdmfAttribute * );

  XdmfGetValueMacro( BaseGrid, XdmfPointer);

  // XdmfPointer GetBaseGridCopy( void );

//! Update an Attribute and Mark it as Primary
/*!
	When an XdmfGrid is read using SetGridFromElement() the Attribute
	values are not read in since there could potentially be an enourmous 
	amout of data associated with the computational grid. Instead, for
	each Attribute of interest, AssignAttribute is called. This updates
	the Heavy Data and marks it as the primary attribute. So the last
	Attribute read will be one marked : visualization readers might
	use this information in their filters. (i.e. An isosurface generator
	might use the primary scalar to determine the scalar value on which 
	to generate the surface.

	\param Index	0 based index of the Attribute to retreive
*/
  XdmfInt32 AssignAttribute( XdmfInt64 Index );
#ifndef SWIG
  XdmfInt32 AssignAttribute( XdmfAttribute *Attribute );
#endif
//! Same as AssignAttribute (more verbose for scripting languages)
  XdmfInt32 AssignAttributeByIndex( XdmfInt64 Index );
//! Assign the Attribute with the specified name
/*!
	In the XML of the grid, if an Attribute has a 
	\b Name value, this Attribute will be assigned.
	Example:
	\verbatim
	<Attribute Name="Pressure">
		<DataStructure
			Format="HDF"
			DataType="Float"
			Precision="4"
			Dimensions="10 20 30">
				Pressure.h5:/Time01/Pressure
		</DataStructure>
	</Attribute>
	\endverbatim
*/
  XdmfInt32 AssignAttributeByName( XdmfString Name );

//! Return the currently marked as Primary
  XdmfAttribute *GetAssignedAttribute( void ) { return( this->AssignedAttribute ); };
//! Returns the index of the Attribute currently marked as Primary
  XdmfInt64 GetAssignedAttributeIndex( void );

  //! Initialize Grid from XML but don't access Heavy Data
  /*!
	Initializes the basic grid structure based on the information found
	in the specified XML Node but does not read any of the underlying
	Heavy data. This can be used to determine the type of grid (structured
	or unstructured, Hex or Tet) and to determine the rank and dimensions
	of the grid.

	\param Element The root element for the grid from the XML 
  */
  XdmfInt32  InitGridFromElement( XdmfXNode *Element );
//! Initialize the grid and read the Heavy Data
/*!
	Initializes the basic grid structure based on the information found
        in the specified XML Node and Read the associated Heavy Data for the
	Topology and Geometry. Heavy Data for the Attreibute(s) is not read.
	Use AssignAttribute to update Attribute Heavy Data.
*/
  XdmfInt32  SetGridFromElement( XdmfXNode *Element );
  // XdmfInt32  SetBaseGrid( XdmfPointer Grid = NULL );

//! Overloads Topology->Update();
//! Update / Re-Read Heavy Data
/*!
	If the Heavy Data has changed (it's stored in NDGM and
	and a new iteration exists) this method will re-read the
	data. If InitGridFromElement() has been called, Update()
	will force a read of the Heavy Data.

*/
  XdmfInt32  Update( void );

protected:

  XdmfPointer  BaseGrid;
  XdmfGeometry  *Geometry;
  XdmfInt32  GeometryIsMine;
  XdmfInt32  BaseGridIsMine;
  XdmfInt32  NumberOfAttributes;
  XdmfAttribute  **Attribute;
  XdmfAttribute  *AssignedAttribute;
};

//! Using a SWIG style Pointer return an XdmfGrid Pointer
extern XDMF_EXPORT XdmfGrid *HandleToXdmfGrid( XdmfString Source);
#endif // __XdmfGrid_h
