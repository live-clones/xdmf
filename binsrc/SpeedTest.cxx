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
#include <Xdmf.h>


int
main( int argc, char **argv ){

XdmfDOM    *DOM = new XdmfDOM();
// XdmfFormatMulti  *Formatter = new XdmfFormatMulti();
XdmfTransform *Formatter = new XdmfTransform();
XdmfFormatHDF  *HFormatter = new XdmfFormatHDF();
XdmfDataDesc  *Desc;
XdmfXNode  *Node;
XdmfArray  *Array;
XdmfHDF    *LowLevelH5 = new XdmfHDF();
XdmfString  Attribute;
XdmfInt32  Rank, NData;
XdmfInt64  Bytes, Dimensions[ XDMF_MAX_DIMENSION ];

int    n, i;
double    Total = 0, Start, Now;


if( argc < 2 ) {
  XdmfErrorMessage("Usage : " << argv[0] << " Filename.xml");
  exit(1);
  }

// Open the XML file
// DOM->SetGlobalDebugOn();
DOM->SetInputFileName( argv[ argc - 1 ] );
DOM->Parse();
NData = DOM->FindNumberOfElements("DataTransform");
cout << "Found " << NData << " DataTransform(s)" << endl;
// Find the DataTransform Desciptor
Node = DOM->FindElement( "DataTransform", 0 );
if( !Node ){
  XdmfErrorMessage("Can't get node");
  exit(1);
  }

Formatter->SetDOM( DOM );
Desc = Formatter->ElementToDataDesc( Node );
cout << "Rank = " << Desc->GetRank() << " Shape " << Desc->GetShapeAsString() << endl;
cout << Desc->GetSelectionSize() << " Element Selected" << endl;
cout << "HyperSlab = " << Desc->GetHyperSlabAsString() << endl;
// Create the Data if it doesn't exist
Rank = Desc->GetShape( Dimensions );
Array = new XdmfArray( Desc->GetNumberType() );
Array->SetShape( Rank, Dimensions );
Array->Generate( 1, Desc->GetNumberOfElements() );

// Get the actual dataset
Node = DOM->FindElement( "DataStructure", 1 );
Attribute = DOM->Get( Node, "CData");
XDMF_WORD_TRIM(Attribute);
cout << "Dataset refers to \"" << Attribute << "\"" << endl;
HFormatter->SetDOM( DOM );
HFormatter->ArrayToElement( Array, Attribute, Node );

//Change the Shape of the Array
Array->SetNumberOfElements( Desc->GetSelectionSize() );

// Timing Loop
LowLevelH5->Open( Attribute, "rw" );
LowLevelH5->CopySelection( Desc );
cout << "Accessing " << Desc->GetSelectionSize() << " of " <<
  Desc->GetNumberOfElements() << " elements" << endl;
n = 10;
for ( i = 0 ; i < n ; i++ ){
  Start =  ndgm_time_now();
  LowLevelH5->Write( Array );
//  LowLevelH5->Read( Array );
  Now =  ndgm_time_now();
  Total += Now - Start;
  Array->Generate( 0, 1 );
  Start =  ndgm_time_now();
  LowLevelH5->Read( Array );
//  LowLevelH5->Write( Array );
  Now =  ndgm_time_now();
  Total += Now - Start;
  Array->Generate( 1, 0 );
  }
LowLevelH5->Close();
// Total = Now - Start;
Bytes = Array->GetSelectionSize() * Array->GetElementSize();
cout << Bytes << " Bytes x " << n << " x 2 (Read/Write) in "<< Total << " Seconds" << endl;
cout << ( Bytes * n * 2 ) / Total << endl;
exit( 0 );
}
