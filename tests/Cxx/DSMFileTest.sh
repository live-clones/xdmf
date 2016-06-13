# Intel MPI requires a minimum of 2 cores per process

#$MPIEXEC -n 4 ./XdmfFileAcceptTest
$MPIEXEC -n 2 ./XdmfFileAcceptTest &
sleep 10
$MPIEXEC -n 2 ./XdmfFileConnectTest
