# Intel MPI requires a minimum of 2 cores per process

mpirun -n 2 ./XdmfAcceptTest &

mpirun -n 2 ./XdmfConnectTest &

mpirun -n 2 ./XdmfConnectTest2
