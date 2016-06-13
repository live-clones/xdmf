# Intel MPI requires a minimum of 2 cores per process

$MPIEXEC -n 2 ./XdmfAcceptTestPaged &

$MPIEXEC -n 2 ./XdmfConnectTestPaged &

$MPIEXEC -n 2 ./XdmfConnectTest2Paged
