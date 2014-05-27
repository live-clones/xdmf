# Intel MPI requires a minimum of 2 cores per process

$MPIEXEC -n 2 $PYTHON_EXECUTABLE ./XdmfExampleAcceptTest.py &

$MPIEXEC -n 2 $PYTHON_EXECUTABLE ./XdmfExampleConnectTest.py &

$MPIEXEC -n 2 $PYTHON_EXECUTABLE ./XdmfExampleConnectTest2.py
