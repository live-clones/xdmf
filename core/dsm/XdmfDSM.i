/*
XdmfDSMPython.cpp:
swig -v -c++ -python -o XdmfDSMPython.cpp XdmfDSM.i
*/


%module XdmfDSM
%{

    #include <mpi.h>

    #include <XdmfArray.hpp>
    #include <XdmfArrayReference.hpp>
    #include <XdmfArrayType.hpp>
    #include <XdmfCore.hpp>
    #include <XdmfCoreItemFactory.hpp>
    #include <XdmfCoreReader.hpp>
    #include <XdmfDSM.hpp>
    #include <XdmfDSMManager.hpp>
    #include <XdmfDSMBuffer.hpp>
    #include <XdmfDSMCommMPI.hpp>
    #include <XdmfError.hpp>
    #include <XdmfFunction.hpp>
    #include <XdmfHeavyDataController.hpp>
    #include <XdmfHeavyDataWriter.hpp>
    #include <XdmfHDF5Controller.hpp>
    #include <XdmfHDF5Writer.hpp>
    #include <XdmfHDF5ControllerDSM.hpp>
    #include <XdmfHDF5WriterDSM.hpp>
    #include <XdmfInformation.hpp>
    #include <XdmfItem.hpp>
    #include <XdmfItemProperty.hpp>
    #include <XdmfSharedPtr.hpp>
    #include <XdmfSparseMatrix.hpp>
    #include <XdmfSubset.hpp>
    #include <XdmfSystemUtils.hpp>
    #include <XdmfVersion.hpp>
    #include <XdmfVisitor.hpp>
    #include <XdmfWriter.hpp>

    #include <ProjectVersion.hpp>
%}

%import XdmfCore.i

#ifdef SWIGJAVA

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("XdmfDSMJava");
        }
        catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load for" +
                               "XdmfDSMJava\n" + e);
            System.exit(1);
        }
    }
%}


#endif /* SWIGJAVA */

#ifdef SWIGPYTHON

%pythoncode {
    from XdmfCore import *
}

%include mpi4py/mpi4py.i

%mpi4py_typemap(Comm, MPI_Comm);

%include <typemaps.i>
%apply int & INOUT {int & data };

#endif /* SWIGPYTHON */



// Shared Pointer Templates
%shared_ptr(XdmfHDF5ControllerDSM)
%shared_ptr(XdmfHDF5WriterDSM)


%include XdmfDSM.hpp
%include XdmfHDF5ControllerDSM.hpp
%include XdmfHDF5WriterDSM.hpp
%include XdmfDSMManager.hpp
%include XdmfDSMBuffer.hpp
%include XdmfDSMCommMPI.hpp
