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
#include <vtkXdmfUtil.h>

#include <vtkObjectFactory.h>
#include <vtkCommand.h>

#include <vtkUnsignedCharArray.h>
#include <vtkIntArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>


//----------------------------------------------------------------------------
vtkXdmfUtil* vtkXdmfUtil::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkXdmfUtil");
  if(ret)
    {
    return (vtkXdmfUtil*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkXdmfUtil;
}

vtkXdmfUtil::vtkXdmfUtil()
{
}


int
vtkXdmfUtil::IsInside( vtkCell *Cell, float X, float Y, float Z )
{
int  Inside = 0;
int  NPoints = Cell->GetNumberOfPoints();
int  SubId = 0;
int  bi = 0;
float  Position[3], ClosestPoint[3], Pcoord[3], Dist2, Weights[20];
// float  Bounds[6];

// Weights = new float[ NPoints ];

// cerr << "Testing IsInside" << endl;
// Cell->GetBounds( Bounds );

// cerr << "Cell Type " << Cell->GetCellType() << endl;
// cerr << "Xrange = " << Bounds[bi + 0] << " " << Bounds[bi + 1] << endl;
// cerr << "Yrange = " << Bounds[bi + 2] << " " << Bounds[bi + 3] << endl;
// cerr << "Zrange = " << Bounds[bi + 4] << " " << Bounds[bi + 5] << endl;
// cerr << "___________________" << endl << Cell->GetClassName() << endl << flush;
// Cell->PrintSelf(cerr, 2);
// cerr << flush;
Position[0] = X;
Position[1] = Y;
Position[2] = Z;
Inside = Cell->EvaluatePosition( Position,
    ClosestPoint,
    SubId,
    Pcoord,
    Dist2,
    Weights);

// cerr << flush << "Raw Inside = " << Inside << endl;
// cerr << "XYZ " << X << " " << Y << " " << Z << endl;
// cerr << "Pcoord = " << Pcoord[0] << ", " << Pcoord[1] << ", " << Pcoord[2] << endl;
// cerr << "Dist2 = " << Dist2 << endl << "_________________" << endl;
// cerr << flush;
return( Inside );
}

vtkIntArray
*vtkXdmfUtil::GetInside( vtkUnstructuredGrid *Cells, vtkPoints *Points ){

vtkIntArray *Inside = vtkIntArray::New();
long  cnt = 0 , pi, pt, ci;


cerr << "# Points = " <<  Points->GetNumberOfPoints() << endl;
cerr << "# Cells = " << Cells->GetNumberOfCells() << endl;
Inside->SetNumberOfValues( Points->GetNumberOfPoints() );
pt = Points->GetNumberOfPoints();
for( pi = 0 ; pi < Points->GetNumberOfPoints() ; pi++ ){
  float *Point = Points->GetPoint( pi );
  Inside->SetValue( pi, 0 );
  if ( cnt >= 10000 ){
    cnt = 0;
    cerr << 100.0 * pi / pt << "% done" << endl;
    }
  cnt++;
  for( ci = 0 ; ci < Cells->GetNumberOfCells() ; ci++ ){
    vtkCell *Cell = Cells->GetCell(ci);
    int  IsInside;
    
    // cerr << "On Cell " << ci << endl;
    IsInside = this->IsInside( Cell, Point[0], Point[1], Point[2] );
    if( IsInside > 0 ) {
      Inside->SetValue( pi, 1 );
      break;
      }
  }
}

return( Inside );
}

vtkIntArray
*vtkXdmfUtil::GetInsideRect( vtkUnstructuredGrid *Cells, int Kdim, float zstart, float dz, int Jdim, float ystart, float dy, int Idim, float xstart, float dx){

vtkIntArray *Inside;
int  IsInside, i, j, k;
int  *ip;
float  x, y, z;
long  ncells, npoints, OnePercent;
long  ni = 0, cnt = 0 , pi, ci, bi;
vtkCell  *Cell;
float  *Bounds;
float  CellBounds[6];


npoints = Kdim * Jdim * Idim;
cerr << "# Points = " <<  npoints << endl;
cerr << "Points Xmin Xmax " << xstart << " " << xstart + ( Idim * dx) << endl;
cerr << "Points Ymin Ymax " << ystart << " " << ystart + ( Jdim * dy) << endl;
cerr << "Points Zmin Zmax " << ystart << " " << ystart + ( Kdim * dz) << endl;
cerr << "# Cells = " << Cells->GetNumberOfCells() << endl;
Cells->GetBounds( CellBounds );
// cerr << "Cells Xmin " << CellBounds[0] << endl;
// cerr << "Cells Xmax " << CellBounds[1] << endl;
// cerr << "Cells Ymin " << CellBounds[2] << endl;
// cerr << "Cells Ymax " << CellBounds[3] << endl;
// cerr << "Cells Zmin " << CellBounds[4] << endl;
// cerr << "Cells Zmax " << CellBounds[5] << endl;
Inside = vtkIntArray::New();
Inside->SetNumberOfValues( npoints );
ip = Inside->GetPointer(0);
memset( ip, 0, npoints * sizeof( int ));
Bounds = new float[ Cells->GetNumberOfCells() * 6 ];
for( ci = 0 ; ci < Cells->GetNumberOfCells(); ci++ ){
  Cell = Cells->GetCell(ci);
  Cell->GetBounds( &Bounds[ ci * 6 ] );  
  }
ncells = Cells->GetNumberOfCells();
OnePercent = ncells / 100;
for( ci = 0 ; ci < ncells ; ci++ ){
 if( cnt >= OnePercent ){
  cnt = 0;
  cerr << 100.0 * ci / ncells << " % done" << endl;
  }
 cnt++;
 bi = ci * 6;
// cerr << flush;
// cerr << "Bounds[bi + 0] = " << Bounds[bi + 0] << endl;
// cerr << "Bounds[bi + 1] = " << Bounds[bi + 1] << endl;
// cerr << "Bounds[bi + 2] = " << Bounds[bi + 2] << endl;
// cerr << "Bounds[bi + 3] = " << Bounds[bi + 3] << endl;
// cerr << "Bounds[bi + 4] = " << Bounds[bi + 4] << endl;
// cerr << "Bounds[bi + 5] = " << Bounds[bi + 5] << endl;
// cerr << flush;
 for(i = 0; i < Idim ; i++){
  x = xstart + ( i * dx );
  if(( x > Bounds[bi + 0] ) && ( x < Bounds[bi + 1] ) ){
   for(j = 0; j < Jdim ; j++){
    y = ystart + ( j * dy );
    if(( y > Bounds[bi + 2] ) && ( y < Bounds[bi + 3] ) ){
     for(k = 0; k < Kdim ; k++){
      z = zstart + ( k * dz );
      if((z > Bounds[bi + 4] ) && ( z < Bounds[bi + 5] )){
  pi = ( k * ( Idim * Jdim )) + ( j * Idim ) + i;
  if(ip[pi] == 0){
    Cell = Cells->GetCell(ci);
    IsInside = this->IsInside( Cell, x, y, z);
    if( IsInside > 0 ) {
      ip[pi] = 1;
      ni++;
      }
  }
      }
     }
    }
   }
  }
 }
}
cerr << ni << " Voxels Inside" << endl;
delete [] Bounds;
// Inside->Delete();
return( Inside );
}

