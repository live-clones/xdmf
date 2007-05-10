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
/*     Copyright @ 2007 US Army Research Laboratory                */
/*     All Rights Reserved                                         */
/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
/*                                                                 */
/*     This software is distributed WITHOUT ANY WARRANTY; without  */
/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
/*     for more information.                                       */
/*                                                                 */
/*******************************************************************/
#include "Xdmf.h"

#include "mpi.h"

//using namespace std;

int
main( int argc, char **argv ) {

int	            i, Data[256];
XdmfInt32       rank, status, who;
XdmfInt64       start, end;
XdmfDsmCommMpi  *MyComm = new XdmfDsmCommMpi;
XdmfDsmBuffer         *MyDsm = new XdmfDsmBuffer;

MPI_Init(&argc, &argv);

// New Communicator
MyComm->Init();
cout << "Hello from " << MyComm->GetId() << " of " << MyComm->GetTotalSize() << endl;
rank = MyComm->GetId();

// MyDsm->ConfigureUniform(MyComm, 1000000, 2, 50);
MyDsm->ConfigureUniform(MyComm, 1000000);

who = MyDsm->AddressToId(1500000);
MyDsm->GetAddressRangeForId(who, &start, &end);

// cout << "Address Range for " << who << " = " << start << " - " << end << endl;
// MyDsm->Put(0, 5000000, Data);
if(rank == 0){
    XdmfInt32   i, Opcode, Source;
    XdmfInt64   Address, Length;
    for(i=1;i<MyComm->GetTotalSize();i++){
    status = MyDsm->ReceiveCommandHeader(&Opcode, &Source, &Address, &Length);
    if(status = XDMF_SUCCESS){
        cout << "Receive From " << Source << " Address " << Address << " Length " << Length << endl;
    }else{
        cout << "Receive Failed" << endl;
    }
    }
}else{
    XdmfInt32   Opcode, Dest = 0;
    XdmfInt64   Address = 0, Length = 100;
    Address = rank * 100;
    status = MyDsm->SendCommandHeader(Opcode, Dest, Address, Length);
    if(status == XDMF_SUCCESS){
        cout << "Send Succeeded for " << Length << " bytes" << endl;
    }else{
        cout << "Send Failed" << endl;
    }
}

MPI_Finalize();

return 0;
}



