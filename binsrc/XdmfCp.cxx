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

//using namespace std;

XdmfInt32
Decend(XdmfHDF *H5, XdmfHDF *Target){

XdmfInt32	i, j, NumberOfChildren;
char		*Cwd, *ToCwd;
XdmfString	CwdName, ToCwdName;
XdmfInt32	Rank;
XdmfInt64	Dimensions[XDMF_MAX_DIMENSION];

NumberOfChildren = H5->GetNumberOfChildren();

CwdName = H5->GetCwdName();
ToCwdName = Target->GetCwdName();
Cwd = new char[strlen(CwdName) + 2];
strcpy(Cwd, CwdName);
ToCwd = new char[strlen(ToCwdName) + 2];
strcpy(ToCwd, ToCwdName);

cout << "HDF5 Group " << Cwd << " has " << NumberOfChildren << " Children" << endl;
for(i=0 ; i < NumberOfChildren ; i++){
	XdmfInt32	Type;
	char		*Directory, *ToDirectory, *Child;
	XdmfString	DirName, ToDirName, ChildName;
	XdmfArray	*Data;

	Type = H5->GetChildType(i);
	ChildName = H5->GetChild(i);
	Child = new char[strlen(ChildName) + 2];
	strcpy(Child, ChildName);
	switch (Type) {
		case(XDMF_H5_DIRECTORY) :
			cout << "\tChild #" << i << " Directory = " << Child << endl;
			Directory = new char[strlen(Cwd) + strlen(Child) + 10];
			strcpy(Directory, Cwd);
			if(strcmp(Cwd, "/")){
				strcat(Directory, "/");
				}
			strcat(Directory, Child);
			DirName = (XdmfString)Directory;
			H5->Cd(DirName);
			ToDirectory = new char[strlen(ToCwd) + strlen(Child) + 10];
			strcpy(ToDirectory, ToCwd);
			if(strcmp(ToCwd, "/")){
				strcat(ToDirectory, "/");
				}
			strcat(ToDirectory, Child);
			ToDirName = (XdmfString)ToDirectory;
			Target->Mkdir(ToDirName);
			// Target->Cd(ToDirName);
			Target->Close();
			Target->Open(ToDirName);
			Decend(H5, Target);
			H5->Cd(Cwd);	
			// Target->Cd(ToCwd);	
			delete Directory;
			delete ToDirectory;
			break;
		case(XDMF_H5_DATASET) :
			cout << "\tChild #" << i << " DataSet = " << H5->GetChild(i) << endl;
			H5->SetPath(Child);
			if(H5->OpenDataset() != XDMF_SUCCESS){
				XdmfErrorMessage("Can't Open DataSet = " << Child);
				return(XDMF_FAIL);
				}
			Rank = H5->GetShape(Dimensions);
			for(j=0;j<Rank;j++){
				cout << "\t\tDimension[" << j << "] = " << Dimensions[j] << endl;
				}
			ToDirectory = new char[strlen(ToCwd) + strlen(Child) + 10];
			strcpy(ToDirectory, ToCwd);
			if(strcmp(ToCwd, "/")){
				strcat(ToDirectory, "/");
				}
			strcat(ToDirectory, Child);
			ToDirName = (XdmfString)ToDirectory;
			Target->CopyType(H5);
			Target->CopyShape(H5);
			if(Target->CreateDataset(ToDirName) != XDMF_SUCCESS){
				XdmfErrorMessage("Can't Create Target Datset " << Child);
				return(XDMF_FAIL);
				}
			Target->Close();
			Target->Open(ToDirName);
			Data = new XdmfArray();
			Data->CopyType(H5);
			Data->CopyShape(H5);
			if(!H5->Read(Data)){
				XdmfErrorMessage("Can't Read Dataset " << Child);
				return(XDMF_FAIL);
				}
			if(!Target->Write(Data)){
				XdmfErrorMessage("Can't Write Dataset " << Child);
				return(XDMF_FAIL);
				}
			delete Data;
			break;
		default :
			break;
		}
	}
return(XDMF_SUCCESS);
}
// Usage : XdmfCp FromDataSetName  ToDataSetName
int
main( int argc, char **argv ) {

XdmfHDF    *FromDataSet, *ToDataSet;

const char    *DataSetNameConst, *ToDataSetNameConst;
char    *DataSetName, *ToDataSetName;
int    i, k;
int	Compression = 0;
double    *DataFromSomewhereElse;

if(argc < 3){
	cerr << "Usage : " << argv[0] << " [-c 0-9] FromPath ToPath" << endl;
	exit(1);
}
if(!strcmp(argv[1], "-c")){
  Compression = atoi(argv[2]);
  DataSetNameConst = argv[3];
  ToDataSetNameConst = argv[4];
}else{
  DataSetNameConst = argv[1];
  ToDataSetNameConst = argv[2];
}

if(strrchr(DataSetNameConst, ':')){
	DataSetName = new char[ strlen(DataSetNameConst) + 1 ];
	strcpy(DataSetName, DataSetNameConst);
}else{
	DataSetName = new char[ strlen(DataSetNameConst) + 10 ];
	strcpy(DataSetName, DataSetNameConst);
	strcat(DataSetName, ":/");
}
if(strrchr(ToDataSetNameConst, ':')){
	ToDataSetName = new char[ strlen(ToDataSetNameConst) + 1 ];
	strcpy(ToDataSetName, ToDataSetNameConst);
}else{
	ToDataSetName = new char[ strlen(ToDataSetNameConst) + 10 ];
	strcpy(ToDataSetName, ToDataSetNameConst);
	strcat(ToDataSetName, ":/");
}


FromDataSet = new XdmfHDF();
ToDataSet = new XdmfHDF();
if(Compression){
	ToDataSet->SetCompression(Compression);
	}
if(FromDataSet->Open( DataSetName, "r" ) != XDMF_SUCCESS){
	XdmfErrorMessage("Can't Open Source XDMF DataSet = " << DataSetName);
	exit(1);
	}
if(ToDataSet->Open( ToDataSetName, "rw" ) != XDMF_SUCCESS){
	XdmfErrorMessage("Can't Target XDMF DataSet = " << ToDataSetName);
	exit(1);
	}
// ToDataSet->SetDebug(1);
Decend(FromDataSet, ToDataSet);
FromDataSet->Close();
ToDataSet->Close();
return 0;
}



