# Intel MPI requires a minimum of 2 cores per process

mpirun -n 2 ./AcceptDSMFortran &

sleep 5

mpirun -n 2 ./ConnectDSMFortran &

sleep 5

mpirun -n 2 ./ConnectDSMFortran2
