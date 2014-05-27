# Intel MPI requires a minimum of 2 cores per process

$MPIEXEC -n 2 ./XdmfAcceptTest &

$MPIEXEC -n 2 ./XdmfConnectTest &

$MPIEXEC -n 2 ./XdmfConnectTest2
