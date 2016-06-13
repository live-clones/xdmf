# Intel MPI requires a minimum of 2 cores per process

$MPIEXEC -n 2 ./CXdmfAcceptTest &

$MPIEXEC -n 2 ./CXdmfConnectTest &

$MPIEXEC -n 2 ./CXdmfConnectTest2
