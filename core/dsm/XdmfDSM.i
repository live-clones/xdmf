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

%extend XdmfHeavyDataController {
    static shared_ptr<XdmfHDF5ControllerDSM> XdmfHDF5ControllerDSMCast(PyObject * obj)
    {
      void * resultPointer = 0;
      swig_type_info * returnType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfHeavyDataController_t");
      SWIG_ConvertPtr(obj, &resultPointer, returnType, 0);
      shared_ptr<XdmfHeavyDataController> * returnControllerPointer =
        reinterpret_cast<shared_ptr<XdmfHeavyDataController> *>(resultPointer);
      shared_ptr<XdmfHeavyDataController> returnController = returnControllerPointer[0];
      if (shared_ptr<XdmfHDF5ControllerDSM> returnHDF5Controller = shared_dynamic_cast<XdmfHDF5ControllerDSM>(returnController)) {
        return returnHDF5Controller;
      }
      else {
        XdmfError::message(XdmfError::FATAL, "Error: Attempting to cast a non HDF5 Controller to HDF5");
        return shared_ptr<XdmfHDF5ControllerDSM>();
      }
    }
};

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
