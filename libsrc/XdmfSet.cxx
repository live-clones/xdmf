#include "XdmfSet.h"

#include "XdmfDataItem.h"
#include "XdmfDataDesc.h"
#include "XdmfArray.h"
#include "XdmfDOM.h"
#include "XdmfMap.h"

XdmfSet::XdmfSet() {
  this->SetElementName("Set");
  this->IdsAreMine = 1;
  this->CellIdsAreMine = 1;
  this->FaceIdsAreMine = 1;
  this->SetType = XDMF_SET_TYPE_UNSET;
  this->Ids = NULL;
  this->CellIds = NULL;
  this->FaceIds = NULL;
  this->ShapeDesc = new XdmfDataDesc();
  this->Active = 0;
  this->Size = 0;
  this->Maps == (XdmfMap **)calloc(1, sizeof(XdmfMap *));
  this->NumberOfMaps = 0;
  }

XdmfSet::~XdmfSet() {
    XdmfInt32 Index;

  if( this->IdsAreMine && this->Ids )  delete this->Ids;
  if( this->CellIdsAreMine && this->CellIds )  delete this->CellIds;
  if( this->FaceIdsAreMine && this->FaceIds )  delete this->FaceIds;
  for ( Index = 0; Index < this->NumberOfMaps; Index ++ ){
      if (this->Maps[Index]->GetDeleteOnGridDelete()){
          delete this->Maps[Index];
      }
  }
  free(this->Maps);
  delete this->ShapeDesc;
  }

XdmfInt32
XdmfSet::Release(){
  if( this->IdsAreMine && this->Ids )  delete this->Ids;
  this->Ids = NULL;
  if( this->CellIdsAreMine && this->Ids )  delete this->CellIds;
  this->CellIds = NULL;
  if( this->FaceIdsAreMine && this->Ids )  delete this->FaceIds;
  this->FaceIds = NULL;
  return(XDMF_SUCCESS);
}

XdmfInt32
XdmfSet::Insert( XdmfElement *Child){
    if(Child && (
        XDMF_WORD_CMP(Child->GetElementName(), "Map") ||
        XDMF_WORD_CMP(Child->GetElementName(), "Attribute") ||
        XDMF_WORD_CMP(Child->GetElementName(), "DataItem") ||
        XDMF_WORD_CMP(Child->GetElementName(), "Information")
        )){
        XdmfInt32   status = XdmfElement::Insert(Child);
        if((status = XDMF_SUCCESS) && XDMF_WORD_CMP(Child->GetElementName(), "Map")){
            XdmfMap *ChildMap = (XdmfMap *)Child;
            this->NumberOfMaps++;
            this->Maps = ( XdmfMap **)realloc( this->Maps,
                this->NumberOfMaps * sizeof( XdmfMap * ));
            if(!this->Maps) {
                XdmfErrorMessage("Realloc of Map List Failed");
                return(XDMF_FAIL);
            }
            this->Maps[this->NumberOfMaps - 1] = ChildMap;
            }
    }else{
        XdmfErrorMessage("Set can only Insert Attribute, DataItem or Information elements");
    }
    return(XDMF_FAIL);
}

XdmfInt32
XdmfSet::Build(){
    if(XdmfElement::Build() != XDMF_SUCCESS) return(XDMF_FAIL);
    this->Set("SetType", this->GetSetTypeAsString());
    if(this->Ids){
        XdmfDataItem    *di = NULL;
        XdmfXmlNode     node;
        //! Is there already a DataItem
        node = this->DOM->FindDataElement(0, this->GetElement());
        if(node) {
            di = (XdmfDataItem *)this->GetCurrentXdmfElement(node);
        }
        if(!di){
            di = new XdmfDataItem;
            node = this->DOM->InsertNew(this->GetElement(), "DataItem");
            di->SetDOM(this->DOM);
            di->SetElement(node);
        }
        di->SetArray(this->Ids);
        if(this->Ids->GetNumberOfElements() > 100) di->SetFormat(XDMF_FORMAT_HDF);
        di->Build();

    }
    return(XDMF_SUCCESS);
}

XdmfConstString
XdmfSet::GetSetTypeAsString( void ){
  switch ( this->SetType ){
    case XDMF_SET_TYPE_CELL :
      return( "Cell" );
    case XDMF_SET_TYPE_FACE :
      return( "Face" );
    case XDMF_SET_TYPE_EDGE :
      return( "Edge" );
    case XDMF_SET_TYPE_NODE :
      return( "Node" );
    case XDMF_SET_TYPE_UNSET :
      return( "Unset" );
    default :
      break;
    }
    XdmfErrorMessage("Unknown SetType = " << this->SetType);
    return(0);
}


XdmfInt32
XdmfSet::SetSetTypeFromString( XdmfConstString regionType ){
if( XDMF_WORD_CMP( regionType, "Cell" ) ) {
  this->SetType = XDMF_SET_TYPE_CELL;
} else if( XDMF_WORD_CMP( regionType, "Face" ) ) {
  this->SetType = XDMF_SET_TYPE_FACE;
} else if( XDMF_WORD_CMP( regionType, "Edge" ) ) {
  this->SetType = XDMF_SET_TYPE_EDGE;
} else if( XDMF_WORD_CMP( regionType, "Node" ) ) {
  this->SetType = XDMF_SET_TYPE_NODE;
} else {
  XdmfErrorMessage("Unknown Set Type " << regionType );
  return( XDMF_FAIL );
  }
return( XDMF_SUCCESS );
}

// Ids
XdmfInt32
XdmfSet::SetIds(XdmfArray *someIds){
    if(someIds == this->Ids) return(XDMF_SUCCESS);
    if(this->IdsAreMine && this->Ids) delete this->Ids;
    this->IdsAreMine = 0;
    this->Ids = someIds;
    return(XDMF_SUCCESS);
}

XdmfArray *
XdmfSet::GetIds(XdmfInt32 Create){
    if(!this->Ids && Create){
        this->Ids = new XdmfArray;
        this->IdsAreMine = 1;
    }
    return(this->Ids);
}

// CellIds
XdmfInt32
XdmfSet::SetCellIds(XdmfArray *someCellIds){
    if(someCellIds == this->CellIds) return(XDMF_SUCCESS);
    if(this->CellIdsAreMine && this->CellIds) delete this->CellIds;
    this->CellIdsAreMine = 0;
    this->CellIds = someCellIds;
    return(XDMF_SUCCESS);
}

XdmfArray *
XdmfSet::GetCellIds(XdmfInt32 Create){
    if(!this->CellIds && Create){
        this->CellIds = new XdmfArray;
        this->CellIdsAreMine = 1;
    }
    return(this->CellIds);
}

// FaceIds
XdmfInt32
XdmfSet::SetFaceIds(XdmfArray *someFaceIds){
    if(someFaceIds == this->FaceIds) return(XDMF_SUCCESS);
    if(this->FaceIdsAreMine && this->FaceIds) delete this->FaceIds;
    this->FaceIdsAreMine = 0;
    this->FaceIds = someFaceIds;
    return(XDMF_SUCCESS);
}

XdmfArray *
XdmfSet::GetFaceIds(XdmfInt32 Create){
    if(!this->FaceIds && Create){
        this->FaceIds = new XdmfArray;
        this->FaceIdsAreMine = 1;
    }
    return(this->FaceIds);
}

XdmfInt32
XdmfSet::UpdateInformation() {

XdmfConstString  Value;

if(XdmfElement::UpdateInformation() != XDMF_SUCCESS) return(XDMF_FAIL);
if( XDMF_WORD_CMP(this->GetElementType(), "Set") == 0){
    XdmfErrorMessage("Element type" << this->GetElementType() << " is not of type 'Set'");
    return(XDMF_FAIL);
}

Value = this->Get( "Active" );
this->Active = 0;
if ( Value ){
  if( XDMF_WORD_CMP( Value, "1" ) ) {
    this->Active = 1;
  }
}

Value = this->Get( "SetType" );
if( Value ){
  this->SetSetTypeFromString( Value );
} else {
  this->SetType = XDMF_SET_TYPE_NODE;
}

// Allow Size | Length | Dimensions
Value = this->Get( "Size" );
if(!Value) Value = this->Get("Length");
if(!Value) Value = this->Get("Dimensions");
if( Value ){
  this->ShapeDesc->SetShapeFromString( Value );
  this->SetSize( this->ShapeDesc->GetNumberOfElements());
}else{
    XdmfXmlNode  IdsNode;
    IdsNode = this->DOM->FindDataElement( 0, Element );
    if(!IdsNode){
        XdmfErrorMessage("Dimensions of Set not set in XML and no DataItem found");
    }
    Value = this->DOM->Get( IdsNode, "Dimensions" );
    if(!Value){
        XdmfErrorMessage("Dimensions of Set not set in XML or DataItem");
        return(XDMF_FAIL);
    }else{
        this->ShapeDesc->SetShapeFromString( Value );
    }
}
// Get Maps
XdmfInt32 OldNumberOfMaps = this->NumberOfMaps;
this->NumberOfMaps = this->DOM->FindNumberOfElements("Map", this->Element );
if( this->NumberOfMaps > 0 ){
  XdmfInt32  Index;
  XdmfMap  *iMap;
  XdmfXmlNode    MapElement;

  for ( Index = 0; Index < OldNumberOfMaps; Index ++ )
    {
    delete this->Maps[Index];
    }
  this->Maps = ( XdmfMap **)realloc( this->Maps,
      this->NumberOfMaps * sizeof( XdmfMap * ));
  for( Index = 0 ; Index < this->NumberOfMaps ; Index++ ){
    iMap = new XdmfMap;

    this->Maps[Index] = iMap;
    MapElement = this->DOM->FindElement( "Map", Index, this->Element );
    iMap->SetDOM( this->DOM );    
    iMap->SetElement( MapElement );
    iMap->UpdateInformation();
    }
}
if(!this->Name) this->SetName(GetUnique("Set_"));
return( XDMF_SUCCESS );
}

XdmfInt32
XdmfSet::Update() {

XdmfInt32   Status;
XdmfInt32   NumberOfDataItems = 1;
XdmfInt32   i;

// check this out
if(XdmfElement::Update() != XDMF_SUCCESS) return(XDMF_FAIL);

if( this->SetType == XDMF_SET_TYPE_UNSET ){
  Status = this->UpdateInformation();
  if( Status == XDMF_FAIL ) {
    XdmfErrorMessage("Can't Initialize");
    return( XDMF_FAIL );
    }
  }

switch (this->SetType){
    case XDMF_SET_TYPE_FACE :
        NumberOfDataItems = 2;
        break;
    case XDMF_SET_TYPE_EDGE :
        NumberOfDataItems = 3;
        break;
    default :
        NumberOfDataItems = 1;
        break;
}

for(i=0 ; i < NumberOfDataItems ; i++){
    XdmfXmlNode IdsNode;
    XdmfInt32   *Mine;
    XdmfArray   **Array;

    switch (this->SetType){
        case XDMF_SET_TYPE_FACE :
            if(i == 0){
                Mine = &this->CellIdsAreMine;
                Array = &this->CellIds;
            }else{
                Mine = &this->IdsAreMine;
                Array = &this->Ids;
            }
            break;
        case XDMF_SET_TYPE_EDGE :
            if(i == 0){
                Mine = &this->CellIdsAreMine;
                Array = &this->CellIds;
            }else if(i == 1) {
                Mine = &this->FaceIdsAreMine;
                Array = &this->FaceIds;
            }
            break;
        default :
            Mine = &this->IdsAreMine;
            Array = &this->Ids;
            break;
    }
    IdsNode = this->DOM->FindDataElement(i, Element );
    if( IdsNode ){
        XdmfDataItem ValueReader;
        ValueReader.SetDOM( this->DOM );
        ValueReader.SetDsmBuffer(this->DsmBuffer);
        if(ValueReader.SetElement(IdsNode) == XDMF_FAIL) return(XDMF_FAIL);
        if(ValueReader.UpdateInformation() == XDMF_FAIL) return(XDMF_FAIL);
        if(ValueReader.Update() == XDMF_FAIL) return(XDMF_FAIL);
        if( *Mine && *Array){
            delete *Array;
            *Mine = 0;
        }
        // Steal the array
        *Array = ValueReader.GetArray();
        if( *Array == 0 ) {
            XdmfErrorMessage("Error Retrieving Data Ids");
            return( XDMF_FAIL );
        }
        ValueReader.SetArrayIsMine(0);
        *Mine = 1;
    } else {
        XdmfErrorMessage("Set does not have enough DataItems. Error reading DataItem #" << i);
        return( XDMF_FAIL );
    }
}

return( XDMF_SUCCESS );
}
