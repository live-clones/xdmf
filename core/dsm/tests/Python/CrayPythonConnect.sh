# Intel MPI requires a minimum of 2 cores per process

echo "aprun -n 2 $PYTHON_EXECUTABLE ./XdmfExampleAcceptTest.py : -n 2 $PYTHON_EXECUTABLE ./XdmfExampleConnectTest.py : -n 2 $PYTHON_EXECUTABLE ./XdmfExampleConnectTest2.py"

aprun -n 2 $PYTHON_EXECUTABLE ./XdmfExampleAcceptTest.py : -n 2 $PYTHON_EXECUTABLE ./XdmfExampleConnectTest.py : -n 2 $PYTHON_EXECUTABLE ./XdmfExampleConnectTest2.py
