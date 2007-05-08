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

int	            i, size, rank, Data[256];
XdmfInt32       status, who;
XdmfDsmCommMpi  *MyComm = new XdmfDsmCommMpi;
XdmfDsmMsg      MyMsg;

MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

cout << "Hello from " << rank << " of " << size << endl;
MyComm->SetId(rank);
MyComm->SetTotalSize(size);

if(rank){
    Data[0] = rank * 10;
    MyMsg.Dest = 0;
    MyMsg.Size = 256 * sizeof(int);
    MyMsg.Data = Data;
    MyComm->Send(&MyMsg);
}else{
    i = size - 1;
    while(i){
        who = MyComm->Check(&MyMsg);
        if(who != XDMF_FAIL){
            MyMsg.Source = who;
            Data[0] = -1;
            MyMsg.Data = Data;
            cout << "Receive from " << who << endl;
            status = MyComm->Receive(&MyMsg);
            cout << "Data[0] = " << Data[0] << endl;
            i--;
        }
    }
}

MPI_Finalize();

return 0;
}



