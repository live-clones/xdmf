# Intel MPI requires a minimum of 2 cores per process

mpirun -n 2 $PYTHON_EXECUTABLE ./XdmfExampleAcceptTest.py &

mpirun -n 2 $PYTHON_EXECUTABLE ./XdmfExampleConnectTest.py &

mpirun -n 2 $PYTHON_EXECUTABLE ./XdmfExampleConnectTest2.py
