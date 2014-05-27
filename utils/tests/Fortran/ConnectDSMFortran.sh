# Intel MPI requires a minimum of 2 cores per process

$MPIEXEC -n 2 ./AcceptDSMFortran &

sleep 5

$MPIEXEC -n 2 ./ConnectDSMFortran &

sleep 5

$MPIEXEC -n 2 ./ConnectDSMFortran2
