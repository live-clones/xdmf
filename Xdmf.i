/*
XdmfPython.cpp:
swig -v -c++ -python -o XdmfPython.cpp Xdmf.i
*/


#ifdef XDMF_BUILD_DSM

%module Xdmf
%{

    // MPI Includes
    #include <mpi.h>

    // XdmfCore Includes
    #include <Xdmf.hpp>
    #include <XdmfArray.hpp>
    #include <XdmfArrayType.hpp>
    #include <XdmfCoreItemFactory.hpp>
    #include <XdmfCoreReader.hpp>
    #include <XdmfDSMManager.hpp>
    #include <XdmfDSMBuffer.hpp>
    #include <XdmfDSMCommMPI.hpp>
    #include <XdmfError.hpp>
    #include <XdmfHDF5Controller.hpp>
    #include <XdmfHDF5Writer.hpp>
    #include <XdmfHDF5ControllerDSM.hpp>
    #include <XdmfHDF5WriterDSM.hpp>
    #include <XdmfHeavyDataController.hpp>
    #include <XdmfHeavyDataWriter.hpp>
    #include <XdmfInformation.hpp>
    #include <XdmfItem.hpp>
    #include <XdmfItemProperty.hpp>
    #include <XdmfSharedPtr.hpp>
    #include <XdmfSparseMatrix.hpp>
    #include <XdmfSystemUtils.hpp>
    #include <XdmfVisitor.hpp>
    #include <XdmfWriter.hpp>

    // Xdmf Includes
    #include <XdmfAttribute.hpp>
    #include <XdmfAttributeCenter.hpp>
    #include <XdmfAttributeType.hpp>
    #include <XdmfCurvilinearGrid.hpp>
    #include <XdmfDomain.hpp>
    #include <XdmfFunction.hpp>
    #include <XdmfGeometry.hpp>
    #include <XdmfGeometryType.hpp>
    #include <XdmfGraph.hpp>
    #include <XdmfGrid.hpp>
    #include <XdmfGridCollection.hpp>
    #include <XdmfGridCollectionType.hpp>
    #include <XdmfItemFactory.hpp>
    #include <XdmfMap.hpp>
    #include <XdmfReader.hpp>
    #include <XdmfRectilinearGrid.hpp>
    #include <XdmfRegularGrid.hpp>
    #include <XdmfSet.hpp>
    #include <XdmfSetType.hpp>
    #include <XdmfTime.hpp>
    #include <XdmfTopology.hpp>
    #include <XdmfTopologyType.hpp>
    #include <XdmfUnstructuredGrid.hpp>
%}

#else
%module Xdmf
%{

    // XdmfCore Includes
    #include <Xdmf.hpp>
    #include <XdmfArray.hpp>
    #include <XdmfArrayType.hpp>
    #include <XdmfCoreItemFactory.hpp>
    #include <XdmfCoreReader.hpp>
    #include <XdmfError.hpp>
    #include <XdmfHDF5Controller.hpp>
    #include <XdmfHDF5Writer.hpp>
    #include <XdmfHeavyDataController.hpp>
    #include <XdmfHeavyDataWriter.hpp>
    #include <XdmfInformation.hpp>
    #include <XdmfItem.hpp>
    #include <XdmfItemProperty.hpp>
    #include <XdmfSharedPtr.hpp>
    #include <XdmfSparseMatrix.hpp>
    #include <XdmfSystemUtils.hpp>
    #include <XdmfVisitor.hpp>
    #include <XdmfWriter.hpp>

    // Xdmf Includes
    #include <XdmfAttribute.hpp>
    #include <XdmfAttributeCenter.hpp>
    #include <XdmfAttributeType.hpp>
    #include <XdmfCurvilinearGrid.hpp>
    #include <XdmfDomain.hpp>
    #include <XdmfFunction.hpp>
    #include <XdmfGeometry.hpp>
    #include <XdmfGeometryType.hpp>
    #include <XdmfGraph.hpp>
    #include <XdmfGrid.hpp>
    #include <XdmfGridCollection.hpp>
    #include <XdmfGridCollectionType.hpp>
    #include <XdmfItemFactory.hpp>
    #include <XdmfMap.hpp>
    #include <XdmfReader.hpp>
    #include <XdmfRectilinearGrid.hpp>
    #include <XdmfRegularGrid.hpp>
    #include <XdmfSet.hpp>
    #include <XdmfSetType.hpp>
    #include <XdmfTime.hpp>
    #include <XdmfTopology.hpp>
    #include <XdmfTopologyType.hpp>
    #include <XdmfUnstructuredGrid.hpp>
%}

#endif

%import XdmfCore.i

#ifdef SWIGJAVA

// Typemaps that work for Java

%typemap(out) shared_ptr<XdmfItem> {
    if(shared_ptr<XdmfAttribute> value = shared_dynamic_cast<XdmfAttribute>($1)) {
        *(shared_ptr< XdmfAttribute > **)&($result) = value ? new shared_ptr< XdmfAttribute >(value) : 0;
    }
    else if(shared_ptr<XdmfCurvilinearGrid> value = shared_dynamic_cast<XdmfCurvilinearGrid>($1)) {
        *(shared_ptr< XdmfCurvilinearGrid > **)&($result) = value ? new shared_ptr< XdmfCurvilinearGrid >(value) : 0;
    }
    else if(shared_ptr<XdmfGridCollection> value = shared_dynamic_cast<XdmfGridCollection>($1)) {
        *(shared_ptr< XdmfGridCollection > **)&($result) = value ? new shared_ptr< XdmfGridCollection >(value) : 0;
    }
    else if(shared_ptr<XdmfDomain> value = shared_dynamic_cast<XdmfDomain>($1)) {
        *(shared_ptr< XdmfDomain > **)&($result) = value ? new shared_ptr< XdmfDomain >(value) : 0;
    }
    else if(shared_ptr<XdmfGeometry> value = shared_dynamic_cast<XdmfGeometry>($1)) {
        *(shared_ptr< XdmfGeometry > **)&($result) = value ? new shared_ptr< XdmfGeometry >(value) : 0;
    }
    else if(shared_ptr<XdmfInformation> value = shared_dynamic_cast<XdmfInformation>($1)) {
        *(shared_ptr< XdmfInformation > **)&($result) = value ? new shared_ptr< XdmfInformation >(value) : 0;
    }
    else if(shared_ptr<XdmfRectilinearGrid> value = shared_dynamic_cast<XdmfRectilinearGrid>($1)) {
        *(shared_ptr< XdmfRectilinearGrid > **)&($result) = value ? new shared_ptr< XdmfRectilinearGrid >(value) : 0;
    }
    else if(shared_ptr<XdmfRegularGrid> value = shared_dynamic_cast<XdmfRegularGrid>($1)) {
        *(shared_ptr< XdmfRegularGrid > **)&($result) = value ? new shared_ptr< XdmfRegularGrid >(value) : 0;
    }
    else if(shared_ptr<XdmfSet> value = shared_dynamic_cast<XdmfSet>($1)) {
        *(shared_ptr< XdmfSet > **)&($result) = value ? new shared_ptr< XdmfSet >(value) : 0;
    }
    else if(shared_ptr<XdmfTime> value = shared_dynamic_cast<XdmfTime>($1)) {
        *(shared_ptr< XdmfTime > **)&($result) = value ? new shared_ptr< XdmfTime >(value) : 0;
    }
    else if(shared_ptr<XdmfTopology> value = shared_dynamic_cast<XdmfTopology>($1)) {
        *(shared_ptr< XdmfTopology > **)&($result) = value ? new shared_ptr< XdmfTopology >(value) : 0;
    }
    else if(shared_ptr<XdmfUnstructuredGrid> value = shared_dynamic_cast<XdmfUnstructuredGrid>($1)) {
        *(shared_ptr< XdmfUnstructuredGrid > **)&($result) = value ? new shared_ptr< XdmfUnstructuredGrid >(value) : 0;
    }
    else {
        *(shared_ptr< XdmfItem > **)&($result) = &($1);
    }
}

// Ignore multiple inheritance warning
#pragma SWIG nowarn=813

// Ignore const overloaded methods
%ignore XdmfCurvilinearGrid::getDimensions() const;
%ignore XdmfDomain::getCurvilinearGrid(const unsigned int) const;
%ignore XdmfDomain::getCurvilinearGrid(const std::string &) const;
%ignore XdmfDomain::getGridCollection(const unsigned int) const;
%ignore XdmfDomain::getGridCollection(const std::string &) const;
%ignore XdmfDomain::getRectilinearGrid(const unsigned int) const;
%ignore XdmfDomain::getRectilinearGrid(const std::string &) const;
%ignore XdmfDomain::getRegularGrid(const unsigned int) const;
%ignore XdmfDomain::getRegularGrid(const std::string &) const;
%ignore XdmfDomain::getUnstructuredGrid(const unsigned int) const;
%ignore XdmfDomain::getUnstructuredGrid(const std::string &) const;
%ignore XdmfGrid::getAttribute(const unsigned int) const;
%ignore XdmfGrid::getAttribute(const std::string &) const;
%ignore XdmfGrid::getMap() const;
%ignore XdmfGrid::getMap(unsigned int const) const;
%ignore XdmfGrid::getMap(unsigned int const &) const;
%ignore XdmfGrid::getMap(std::string const &) const;
%ignore XdmfGrid::getSet(const unsigned int) const;
%ignore XdmfGrid::getSet(const std::string &) const;
%ignore XdmfGrid::getTime() const;
%ignore XdmfRectilinearGrid::getCoordinates(const unsigned int) const;
%ignore XdmfRectilinearGrid::getCoordinates() const;
%ignore XdmfRectilinearGrid::getDimensions() const;
%ignore XdmfRegularGrid::getBrickSize() const;
%ignore XdmfRegularGrid::getDimensions() const;
%ignore XdmfRegularGrid::getOrigin() const;
%ignore XdmfSet::getAttribute(const unsigned int) const;
%ignore XdmfSet::getAttribute(const std::string &) const;

// Ignore ItemTags
%ignore XdmfAttribute::ItemTag;
%ignore XdmfCurvilinearGrid::ItemTag;
%ignore XdmfDomain::ItemTag;
%ignore XdmfGeometry::ItemTag;
%ignore XdmfGraph::ItemTag;
%ignore XdmfGrid::ItemTag;
%ignore XdmfGridCollection::ItemTag;
%ignore XdmfMap::ItemTag;
%ignore XdmfRectilinearGrid::ItemTag;
%ignore XdmfRegularGrid::ItemTag;
%ignore XdmfSet::ItemTag;
%ignore XdmfTime::ItemTag;
%ignore XdmfTopology::ItemTag;
%ignore XdmfUnstructuredGrid::ItemTag;

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("XdmfJava");
        }
        catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load for" +
                                "XdmfJava\n" + e);
            System.exit(1);
        }
    }
%}

#endif /* SWIGJAVA */

#ifdef SWIGPYTHON

%include std_set.i
%include std_map.i
%include std_vector.i

#ifdef XDMF_BUILD_DSM

%include mpi4py/mpi4py.i

%mpi4py_typemap(Comm, MPI_Comm);

#endif /* XDMF_BUILD_DSM */

%template(XdmfMapNodeIdSet) std::set<int>;
%template(XdmfMapNodeIdMap) std::map<int, std::set<int> >;
%template(XdmfMapMap) std::map<int, std::map<int, std::set<int> > >;
%template(AttributeVector) std::vector<shared_ptr<XdmfAttribute> >;
%template(MapVector) std::vector<shared_ptr<XdmfMap> >;
%template(ArrayVector) std::vector<shared_ptr<XdmfArray> >;

%pythoncode {
    from XdmfCore import *
}

%fragment("XdmfItemCast", "header") {
    #include <XdmfSharedPtr.hpp>
    PyObject * XdmfItemCast(shared_ptr<XdmfItem> obj) {
        PyObject * result;
        if(shared_ptr<XdmfAttribute> attribute = shared_dynamic_cast<XdmfAttribute>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfAttribute>(attribute)), SWIGTYPE_p_boost__shared_ptrT_XdmfAttribute_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfCurvilinearGrid> gridCurvilinear = shared_dynamic_cast<XdmfCurvilinearGrid>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfCurvilinearGrid>(gridCurvilinear)), SWIGTYPE_p_boost__shared_ptrT_XdmfCurvilinearGrid_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfGridCollection> gridCollection = shared_dynamic_cast<XdmfGridCollection>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfGridCollection>(gridCollection)), SWIGTYPE_p_boost__shared_ptrT_XdmfGridCollection_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfDomain> domain = shared_dynamic_cast<XdmfDomain>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfDomain>(domain)), SWIGTYPE_p_boost__shared_ptrT_XdmfDomain_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfGeometry> geometry = shared_dynamic_cast<XdmfGeometry>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfGeometry>(geometry)), SWIGTYPE_p_boost__shared_ptrT_XdmfGeometry_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfGraph> graph = shared_dynamic_cast<XdmfGraph>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfGraph>(graph)), SWIGTYPE_p_boost__shared_ptrT_XdmfGraph_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfInformation> information = shared_dynamic_cast<XdmfInformation>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfInformation>(information)), SWIGTYPE_p_boost__shared_ptrT_XdmfInformation_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfMap> map = shared_dynamic_cast<XdmfMap>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfMap>(map)), SWIGTYPE_p_boost__shared_ptrT_XdmfMap_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfRectilinearGrid> gridRectilinear = shared_dynamic_cast<XdmfRectilinearGrid>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfRectilinearGrid>(gridRectilinear)), SWIGTYPE_p_boost__shared_ptrT_XdmfRectilinearGrid_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfRegularGrid> gridRegular = shared_dynamic_cast<XdmfRegularGrid>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfRegularGrid>(gridRegular)), SWIGTYPE_p_boost__shared_ptrT_XdmfRegularGrid_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfSet> set = shared_dynamic_cast<XdmfSet>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfSet>(set)), SWIGTYPE_p_boost__shared_ptrT_XdmfSet_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfTime> time = shared_dynamic_cast<XdmfTime>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfTime>(time)), SWIGTYPE_p_boost__shared_ptrT_XdmfTime_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfTopology> topology = shared_dynamic_cast<XdmfTopology>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfTopology>(topology)), SWIGTYPE_p_boost__shared_ptrT_XdmfTopology_t, SWIG_POINTER_OWN);
        }
        else if(shared_ptr<XdmfUnstructuredGrid> gridUnstructured = shared_dynamic_cast<XdmfUnstructuredGrid>(obj)) {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfUnstructuredGrid>(gridUnstructured)), SWIGTYPE_p_boost__shared_ptrT_XdmfUnstructuredGrid_t, SWIG_POINTER_OWN);
        }
        else {
            result = SWIG_NewPointerObj(SWIG_as_voidptr(new shared_ptr<XdmfItem>(obj)), SWIGTYPE_p_boost__shared_ptrT_XdmfItem_t, SWIG_POINTER_OWN);
        }
        return result;
    }
}

%typemap(out, fragment="XdmfItemCast") shared_ptr<XdmfItem> {
    $result = XdmfItemCast($1);
}

%typemap(out, fragment="XdmfItemCast") std::vector<shared_ptr<XdmfItem> > {
    $result = PyList_New($1.size());
    for(std::vector<shared_ptr<XdmfItem> >::size_type i = 0; i < $1.size(); ++i) {
        PyList_SetItem($result, i, XdmfItemCast($1[i]));
    } 
}

%extend XdmfAttributeCenter {
    bool __eq__(const XdmfAttributeCenter * attributeCenter) {
        return $self == attributeCenter;
    }
}

%extend XdmfAttributeType {
    bool __eq__(const XdmfAttributeType * attributeType) {
   	return $self == attributeType;
    }
}

%extend XdmfGeometryType {
    bool __eq__(const XdmfGeometryType * geometryType) {
   	return $self == geometryType;
    }
}

%extend XdmfGridCollectionType {
    bool __eq__(const XdmfGridCollectionType * gridCollectionType) {
   	return $self == gridCollectionType;
    }
}

%extend XdmfSetType {
    bool __eq__(const XdmfSetType * setType) {
   	return $self == setType;
    }
}

%extend XdmfTopologyType {
    bool __eq__(const XdmfTopologyType * topologyType) {
   	return $self == topologyType;
    }
}

/*This causes it to avoid throwing a warning for redefining fuctions that are defined for XdmfArray.
  I do this because doing so was intentional.*/
#pragma SWIG nowarn=302

%extend XdmfFunction {

%{
  #include <stack>
  static std::map<std::string, PyObject *> pythonFunctions;
  static std::map<char, PyObject *> pythonOperations;
  static int pythonOperationPriority [4];
  static std::string pythonSupportedOperations = "";
%}
/*These can't be accessed from python so don't worry about security issues.*/

/*Note, the things in this block are defined twice for a reason.
  If they are not defined here they can not be referenced by other functions in swig.*/
%{
    /*trying to transfer python funcitons*/
    /*note, accessing private members is impossible from swig.*/

    /*to generate new static functions mark them as static here.*/
    static shared_ptr<XdmfArray> evaluateFunction(std::vector<shared_ptr<XdmfArray> > functValues, std::string functName)
    {
      if (pythonFunctions.find(functName)!= pythonFunctions.end()) {
        swig_type_info * paramType = SWIG_TypeQuery("_p_std__vectorT_boost__shared_ptrT_XdmfArray_t_std__allocatorT_boost__shared_ptrT_XdmfArray_t_t_t");
        PyObject * pyVector = SWIG_NewPointerObj(static_cast<void*>(& functValues), paramType, SWIG_POINTER_NEW);
        PyObject * args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, pyVector);/*In this case you could also cast a pointer to the vector into the PyObject * type, but that doesn't work for all types*/
        PyObject * resultObject = PyObject_CallObject(pythonFunctions[functName], args);
        void * resultPointer = 0;
        swig_type_info * returnType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
        SWIG_ConvertPtr(resultObject, &resultPointer, returnType, 0);
        shared_ptr<XdmfArray> * returnArrayPointer = reinterpret_cast<shared_ptr<XdmfArray> *>(resultPointer);
        shared_ptr<XdmfArray> returnArray = returnArrayPointer[0];
        return returnArray;
      }
      else {
         /*this does not actually cause an infinte recursive loop, it sends the values to the version of the function defined in XdmfArray.cpp*/
         return XdmfFunction::evaluateFunction(functValues, functName);
      }
    }

    static shared_ptr<XdmfArray> evaluateOperation(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2, char functName)
    {
      if (pythonOperations.find(functName)!= pythonOperations.end()) {
        swig_type_info * paramType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
        PyObject * pyVal1 = SWIG_NewPointerObj(static_cast<void*>(& val1), paramType, SWIG_POINTER_NEW);
        PyObject * pyVal2 = SWIG_NewPointerObj(static_cast<void*>(& val2), paramType, SWIG_POINTER_NEW);
        PyObject * args = PyTuple_New(2);
        PyTuple_SetItem(args, 0, pyVal1);
        PyTuple_SetItem(args, 1, pyVal2);
        PyObject * resultObject = PyObject_CallObject(pythonOperations[functName], args);
        void * resultPointer = 0;
        swig_type_info * returnType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
        SWIG_ConvertPtr(resultObject, &resultPointer, returnType, 0);
        shared_ptr<XdmfArray> * returnArrayPointer = reinterpret_cast<shared_ptr<XdmfArray> *>(resultPointer);
        shared_ptr<XdmfArray> returnArray = returnArrayPointer[0];
        return returnArray;
      }
      else {
         /*this does not actually cause an infinte recursive loop, it sends the values to the version of the function defined in XdmfArray.cpp*/
         return XdmfFunction::evaluateOperation(val1, val2, functName);
      }
    }

    static shared_ptr<XdmfArray> evaluateExpression(std::string expression, std::map<std::string, shared_ptr<XdmfArray> > variables)
    {
      std::stack<shared_ptr<XdmfArray> > valueStack;
      std::stack<char> operationStack;

      /*string is parsed left to right
        elements of the same priority are evaluated right to left*/
      for (unsigned int i = 0; i < expression.size(); i++) {
        if (XdmfFunction::getValidDigitChars().find(expression[i]) != std::string::npos) { /*found to be a digit*/
          /*progress until a non-digit is found*/
          int valueStart = i;
          while (XdmfFunction::getValidDigitChars().find(expression[i + 1]) != std::string::npos) {
            i++;
          }
          /*push back to the value stack*/
          shared_ptr<XdmfArray> valueArray = XdmfArray::New();
          valueArray->insert(0, atof(expression.substr(valueStart, i + 1 - valueStart).c_str()));/*use this to convert to double*/
          valueStack.push(valueArray);
        }
        else if (XdmfFunction::getValidVariableChars().find(expression[i]) != std::string::npos) {/*found to be a variable*/
          int valueStart = i;
          /*progress until a nonvariable value is found*/
          while (XdmfFunction::getValidVariableChars().find(expression[i + 1]) != std::string::npos) {
            i++;
          }
          /*convert to equivalent*/
          if (variables.find(expression.substr(valueStart, i + 1 - valueStart)) == variables.end()) {
            std::vector<std::string> functionList = XdmfFunction::getSupportedFunctions();
            bool functionExists = false;
            for (unsigned int j = 0; j < functionList.size() && !functionExists; j++) {
              if (functionList[j] == expression.substr(valueStart, i + 1 - valueStart)) {
                functionExists = true;
              }
            }
            if (functionExists) {
              XdmfError::message(XdmfError::FATAL,
                                 "Error: Invalid Variable in evaluateExpression " + expression.substr(valueStart, i + 1 - valueStart));
            }
            else {
              std::string currentFunction = expression.substr(valueStart, i + 1 - valueStart);
              /*check if next character is an open parenthesis*/
              if (expression[i+1] != '(') {
                XdmfError::message(XdmfError::FATAL,
                                   "Error: No values supplied to function " + expression.substr(valueStart, i + 1 - valueStart));
              }
              /*if it is grab the string between paranthesis*/
              i = i + 2;
              valueStart = i;
              int numOpenParenthesis = 0;
              while ((expression[i] != ')' || numOpenParenthesis) && i < expression.size()) {
                if (expression[i] == '(') {
                  numOpenParenthesis++;
                }
                else if (expression[i] == ')') {
                  numOpenParenthesis--;
                }
                i++;
              }
              std::string functionParameters = expression.substr(valueStart, i - valueStart);
              std::vector<shared_ptr<XdmfArray> > parameterVector;
              /*split that string at commas*/
              size_t parameterSplit = 0;
              while (parameterSplit != std::string::npos) {
                parameterSplit = 0;
                parameterSplit = functionParameters.find_first_of(",", parameterSplit);
                /*feed the substrings to the parse function*/
                if (parameterSplit == std::string::npos) {
                  parameterVector.push_back(evaluateExpression(functionParameters, variables));
                }
                else {
                  parameterVector.push_back(evaluateExpression(functionParameters.substr(0, parameterSplit), variables));
                  functionParameters = functionParameters.substr(parameterSplit+1);
                }
              }
              valueStack.push(evaluateFunction(parameterVector, currentFunction));
            }
          }
          else {
            /*push equivalent to value stack*/
            valueStack.push(variables.find(expression.substr(valueStart, i + 1 - valueStart))->second);
          }
        }
        else if (XdmfFunction::getSupportedOperations().find(expression[i]) != std::string::npos) {/*found to be an operation*/
          /*pop operations off the stack until one of a lower or equal importance is found*/
          if (operationStack.size() > 0) {
            if (expression[i] == ')') {
              /*to close a parenthesis pop off all operations until another parentheis is found*/
              while (operationStack.size() > 0 && operationStack.top() != '(') {
                if (valueStack.size() < 2) {/*must be at least two values for this loop to work properly*/
                  XdmfError::message(XdmfError::FATAL,
                                     "Error: Not Enough Values in evaluateExpression");
                }
                else {
                  shared_ptr<XdmfArray> val2 = valueStack.top();
                  valueStack.pop();
                  shared_ptr<XdmfArray> val1 = valueStack.top();
                  valueStack.pop();
                  valueStack.push(evaluateOperation(val1, val2, operationStack.top()));
                  operationStack.pop();
                }
              }
              operationStack.pop();
            }
            else if (expression[i] == '(') {
              /*just add it if it's a start parenthesis
                nothing happens here in that case
                addition happens after the if statement block*/
            }
            else {
              int operationLocation = XdmfFunction::getOperationPriority(expression[i]);
              int topOperationLocation = XdmfFunction::getOperationPriority(operationStack.top());
              /*see order of operations to determine importance*/
              while (operationStack.size() > 0 && operationLocation < topOperationLocation) {
                if (valueStack.size() < 2) {/*must be at least two values for this loop to work properly*/
                  XdmfError::message(XdmfError::FATAL,
                                     "Error: Not Enough Values in evaluateExpression");
                }
                else {
                  shared_ptr<XdmfArray> val2 = valueStack.top();
                  valueStack.pop();
                  shared_ptr<XdmfArray> val1 = valueStack.top();
                  valueStack.pop();
                  valueStack.push(evaluateOperation(val1, val2, operationStack.top()));
                  operationStack.pop();
                  if (operationStack.size() == 0) {
                    break;
                  }
                  topOperationLocation = XdmfFunction::getOperationPriority(operationStack.top());
                }
              }
            }
          }
          if (expression[i] != ')') {
            /*add the operation to the operation stack*/
            operationStack.push(expression[i]);
          }
        }
        /*if not a value or operation the character is ignored*/
      }


      /*empty what's left in the stacks before finishing*/
      while (valueStack.size() > 1 && operationStack.size() > 0) {
        if (valueStack.size() < 2) {/*must be at least two values for this loop to work properly*/
          XdmfError::message(XdmfError::FATAL,
                             "Error: Not Enough Values in evaluateExpression");
        }
        else {
          if(operationStack.top() == '(') {
            XdmfError::message(XdmfError::WARNING,
                               "Warning: Unpaired Parenthesis");
          }
          else {
            shared_ptr<XdmfArray> val2 = valueStack.top();
            valueStack.pop();
            shared_ptr<XdmfArray> val1 = valueStack.top();
            valueStack.pop();
            if (operationStack.size() == 0) {
              XdmfError::message(XdmfError::FATAL,
                                 "Error: Not Enough Operators in evaluateExpression");
            }
            else {
              valueStack.push(evaluateOperation(val1, val2, operationStack.top()));
              operationStack.pop();
            }
          }
        }
      }

      /*throw error if there's extra operations*/
      if (operationStack.size() > 0) {
        XdmfError::message(XdmfError::WARNING,
                           "Warning: Left Over Operators in evaluateExpression");
      }

      if (valueStack.size() > 1) {
        XdmfError::message(XdmfError::WARNING,
                           "Warning: Left Over Values in evaluateExpression");
      }

      return valueStack.top();
    }

%}

    /*trying to transfer python funcitons*/
    /*note, accessing private members is impossible from swig.*/
    static int addFunction(std::string newName, PyObject * functionref)
    {
      /*check if the object is callable*/
      if (PyCallable_Check(functionref) == 1) {
        pythonFunctions[newName] = functionref;
        return pythonFunctions.size();
      }
      else {
        XdmfError::message(XdmfError::FATAL,
          "Error: Function is not callable");
        return -1;
      }
    }

    /*to generate new static functions mark them as static here.*/
    static std::vector<std::string> getSupportedFunctions()
    {
      std::vector<std::string> returnVector = XdmfFunction::getSupportedFunctions();
      for (std::map<std::string, PyObject *>::iterator functionWalker = pythonFunctions.begin(); functionWalker != pythonFunctions.end(); functionWalker++) {
        returnVector.push_back(functionWalker->first);
      }
      return returnVector;
    }

    static shared_ptr<XdmfArray> evaluateFunction(std::vector<shared_ptr<XdmfArray> > functValues, std::string functName)
    {
      if (pythonFunctions.find(functName)!= pythonFunctions.end()) {
        swig_type_info * paramType = SWIG_TypeQuery("_p_std__vectorT_boost__shared_ptrT_XdmfArray_t_std__allocatorT_boost__shared_ptrT_XdmfArray_t_t_t");
        PyObject * pyVector = SWIG_NewPointerObj(static_cast<void*>(& functValues), paramType, SWIG_POINTER_NEW);
        PyObject * args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, pyVector);/*In this case you could also cast a pointer to the vector into the PyObject * type, but that doesn't work for all types*/
        PyObject * resultObject = PyObject_CallObject(pythonFunctions[functName], args);
        void * resultPointer = 0;
        swig_type_info * returnType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
        SWIG_ConvertPtr(resultObject, &resultPointer, returnType, 0);
        shared_ptr<XdmfArray> * returnArrayPointer = reinterpret_cast<shared_ptr<XdmfArray> *>(resultPointer);
        shared_ptr<XdmfArray> returnArray = returnArrayPointer[0];
        return returnArray;
      }
      else {
         /*this does not actually cause an infinte recursive loop, it sends the values to the version of the function defined in XdmfArray.cpp*/
         return XdmfFunction::evaluateFunction(functValues, functName);
      }
    }

    static int addOperation(char newName, PyObject * calcref, int priority)
    {
      if (newName == '(' || newName == ')') {
        XdmfError::message(XdmfError::FATAL,
                           "Error: Parenthesis can not be redefined");
      }
      /*check if the object is callable*/
      if (PyCallable_Check(calcref) == 1) {
        if (pythonOperations.find(newName) != pythonOperations.end() || XdmfFunction::getSupportedOperations().find(newName) != std::string::npos) {
          XdmfError::message(XdmfError::WARNING,
                             "Warning: Operation Redefined");/*It's a good idea to warn users when they're doing this*/
          size_t operationLocation = pythonSupportedOperations.find(newName);
          /*if not an already defined python function*/
          if (pythonOperations.find(newName) == pythonOperations.end()) {
            pythonSupportedOperations.push_back(newName);
            unsigned int priorityArraySize = sizeof(pythonOperationPriority)/sizeof(int);
            if (pythonSupportedOperations.size()-1 > priorityArraySize) {
              int newArray [priorityArraySize*2];
              std::copy(pythonOperationPriority, pythonOperationPriority+(priorityArraySize-1), newArray);
              *pythonOperationPriority = *newArray;
            }
          }
          pythonOperationPriority[operationLocation] = priority;
          /*if defined on the c++ side same as adding a new one*/
        }
        else {
          pythonSupportedOperations += newName;
          if (XdmfFunction::getValidVariableChars().find(newName) != std::string::npos || XdmfFunction::getValidDigitChars().find(newName) != std::string::npos) {
            XdmfError::message(XdmfError::FATAL,
              "Error: Operation Overlaps with Variables");
          }
          else {
            pythonSupportedOperations.push_back(newName);
            unsigned int priorityArraySize = sizeof(pythonOperationPriority)/sizeof(int);
            if (pythonSupportedOperations.size()-1 > priorityArraySize)
            {
              int newArray [priorityArraySize*2];
              std::copy(pythonOperationPriority, pythonOperationPriority+(priorityArraySize-1), newArray);
              *pythonOperationPriority = *newArray;
            }
          }
        }
        pythonOperations[newName] = calcref;
        return pythonOperations.size();
      }
      else {
        XdmfError::message(XdmfError::FATAL,
          "Error: Operation is not callable");
        return -1;
      }
    }

    static std::string getSupportedOperations()
    {
      std::string returnVector = XdmfFunction::getSupportedOperations();
      for (std::map<char, PyObject *>::iterator functionWalker = pythonOperations.begin(); functionWalker != pythonOperations.end(); functionWalker++) {
        returnVector += functionWalker->first;
      }
      return returnVector;
    }

    static shared_ptr<XdmfArray> evaluateOperation(shared_ptr<XdmfArray> val1, shared_ptr<XdmfArray> val2, char functName)
    {
      if (pythonOperations.find(functName)!= pythonOperations.end()) {
        swig_type_info * paramType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
        PyObject * pyVal1 = SWIG_NewPointerObj(static_cast<void*>(& val1), paramType, SWIG_POINTER_NEW);
        PyObject * pyVal2 = SWIG_NewPointerObj(static_cast<void*>(& val2), paramType, SWIG_POINTER_NEW);
        PyObject * args = PyTuple_New(2);
        PyTuple_SetItem(args, 0, pyVal1);
        PyTuple_SetItem(args, 1, pyVal2);
        PyObject * resultObject = PyObject_CallObject(pythonOperations[functName], args);
        void * resultPointer = 0;
        swig_type_info * returnType = SWIG_TypeQuery("_p_boost__shared_ptrT_XdmfArray_t");
        SWIG_ConvertPtr(resultObject, &resultPointer, returnType, 0);
        shared_ptr<XdmfArray> * returnArrayPointer = reinterpret_cast<shared_ptr<XdmfArray> *>(resultPointer);
        shared_ptr<XdmfArray> returnArray = returnArrayPointer[0];
        return returnArray;
      }
      else {
         /*this does not actually cause an infinte recursive loop, it sends the values to the version of the function defined in XdmfArray.cpp*/
         return XdmfFunction::evaluateOperation(val1, val2, functName);
      }
    }

    static shared_ptr<XdmfArray> evaluateExpression(std::string expression, std::map<std::string, shared_ptr<XdmfArray> > variables)
    {
      std::stack<shared_ptr<XdmfArray> > valueStack;
      std::stack<char> operationStack;

      /*string is parsed left to right
        elements of the same priority are evaluated right to left*/
      for (unsigned int i = 0; i < expression.size(); i++) {
        if (XdmfFunction::getValidDigitChars().find(expression[i]) != std::string::npos) { /*found to be a digit*/
          /*progress until a non-digit is found*/
          int valueStart = i;
          while (XdmfFunction::getValidDigitChars().find(expression[i + 1]) != std::string::npos) {
            i++;
          }
          /*push back to the value stack*/
          shared_ptr<XdmfArray> valueArray = XdmfArray::New();
          valueArray->insert(0, atof(expression.substr(valueStart, i + 1 - valueStart).c_str()));/*use this to convert to double*/
          valueStack.push(valueArray);
        }
        else if (XdmfFunction::getValidVariableChars().find(expression[i]) != std::string::npos) {/*found to be a variable*/
          int valueStart = i;
          /*progress until a nonvariable value is found*/
          while (XdmfFunction::getValidVariableChars().find(expression[i + 1]) != std::string::npos) {
            i++;
          }
          /*convert to equivalent*/
          if (variables.find(expression.substr(valueStart, i + 1 - valueStart)) == variables.end()) {
            std::vector<std::string> functionList = XdmfFunction::getSupportedFunctions();
            bool functionExists = false;
            for (unsigned int j = 0; j < functionList.size() && !functionExists; j++) {
              if (functionList[j] == expression.substr(valueStart, i + 1 - valueStart)) {
                functionExists = true;
              }
            }
            if (functionExists) {
              XdmfError::message(XdmfError::FATAL,
                                 "Error: Invalid Variable in evaluateExpression " + expression.substr(valueStart, i + 1 - valueStart));
            }
            else {
              std::string currentFunction = expression.substr(valueStart, i + 1 - valueStart);
              /*check if next character is an open parenthesis*/
              if (expression[i+1] != '(') {
                XdmfError::message(XdmfError::FATAL,
                                   "Error: No values supplied to function " + expression.substr(valueStart, i + 1 - valueStart));
              }
              /*if it is grab the string between paranthesis*/
              i = i + 2;
              valueStart = i;
              int numOpenParenthesis = 0;
              while ((expression[i] != ')' || numOpenParenthesis) && i < expression.size()) {
                if (expression[i] == '(') {
                  numOpenParenthesis++;
                }
                else if (expression[i] == ')') {
                  numOpenParenthesis--;
                }
                i++;
              }
              std::string functionParameters = expression.substr(valueStart, i - valueStart);
              std::vector<shared_ptr<XdmfArray> > parameterVector;
              /*split that string at commas*/
              size_t parameterSplit = 0;
              while (parameterSplit != std::string::npos) {
                parameterSplit = 0;
                parameterSplit = functionParameters.find_first_of(",", parameterSplit);
                /*feed the substrings to the parse function*/
                if (parameterSplit == std::string::npos) {
                  parameterVector.push_back(evaluateExpression(functionParameters, variables));
                }
                else {
                  parameterVector.push_back(evaluateExpression(functionParameters.substr(0, parameterSplit), variables));
                  functionParameters = functionParameters.substr(parameterSplit+1);
                }
              }
              valueStack.push(evaluateFunction(parameterVector, currentFunction));
            }
          }
          else {
            /*push equivalent to value stack*/
            valueStack.push(variables.find(expression.substr(valueStart, i + 1 - valueStart))->second);
          }
        }
        else if (XdmfFunction::getSupportedOperations().find(expression[i]) != std::string::npos) {/*found to be an operation*/
          /*pop operations off the stack until one of a lower or equal importance is found*/
          if (operationStack.size() > 0) {
            if (expression[i] == ')') {
              /*to close a parenthesis pop off all operations until another parentheis is found*/
              while (operationStack.size() > 0 && operationStack.top() != '(') {
                if (valueStack.size() < 2) {/*must be at least two values for this loop to work properly*/
                  XdmfError::message(XdmfError::FATAL,
                                     "Error: Not Enough Values in evaluateExpression");
                }
                else {
                  shared_ptr<XdmfArray> val2 = valueStack.top();
                  valueStack.pop();
                  shared_ptr<XdmfArray> val1 = valueStack.top();
                  valueStack.pop();
                  valueStack.push(evaluateOperation(val1, val2, operationStack.top()));
                  operationStack.pop();
                }
              }
              operationStack.pop();
            }
            else if (expression[i] == '(') {
              /*just add it if it's a start parenthesis
                nothing happens here in that case
                addition happens after the if statement*/
            }
            else {
              int operationLocation = XdmfFunction::getOperationPriority(expression[i]);
              int topOperationLocation = XdmfFunction::getOperationPriority(operationStack.top());
              /*see order of operations to determine importance*/
              while (operationStack.size() > 0 && operationLocation < topOperationLocation) {
                if (valueStack.size() < 2) {/*must be at least two values for this loop to work properly*/
                  XdmfError::message(XdmfError::FATAL,
                                     "Error: Not Enough Values in evaluateExpression");
                }
                else {
                  shared_ptr<XdmfArray> val2 = valueStack.top();
                  valueStack.pop();
                  shared_ptr<XdmfArray> val1 = valueStack.top();
                  valueStack.pop();
                  valueStack.push(evaluateOperation(val1, val2, operationStack.top()));
                  operationStack.pop();
                  if (operationStack.size() == 0) {
                    break;
                  }
                  topOperationLocation = XdmfFunction::getOperationPriority(operationStack.top());
                }
              }
            }
          }
          if (expression[i] != ')') {
            /*add the operation to the operation stack*/
            operationStack.push(expression[i]);
          }
        }
        /*if not a value or operation the character is ignored*/
      }


      /*empty what's left in the stacks before finishing*/
      while (valueStack.size() > 1 && operationStack.size() > 0) {
        if (valueStack.size() < 2) {/*must be at least two values for this loop to work properly*/
          XdmfError::message(XdmfError::FATAL,
                             "Error: Not Enough Values in evaluateExpression");
        }
        else {
          if(operationStack.top() == '(') {
            XdmfError::message(XdmfError::WARNING,
                               "Warning: Unpaired Parenthesis");
          }
          else {
            shared_ptr<XdmfArray> val2 = valueStack.top();
            valueStack.pop();
            shared_ptr<XdmfArray> val1 = valueStack.top();
            valueStack.pop();
            if (operationStack.size() == 0) {
              XdmfError::message(XdmfError::FATAL,
                                 "Error: Not Enough Operators in evaluateExpression");
            }
            else {
              valueStack.push(evaluateOperation(val1, val2, operationStack.top()));
              operationStack.pop();
            }
          }
        }
      }

      /*throw error if there's extra operations*/
      if (operationStack.size() > 0) {
        XdmfError::message(XdmfError::WARNING,
                           "Warning: Left Over Operators in evaluateExpression");
      }

      if (valueStack.size() > 1) {
        XdmfError::message(XdmfError::WARNING,
                           "Warning: Left Over Values in evaluateExpression");
      }

      return valueStack.top();
    }
};

#endif /* SWIGPYTHON */

// Shared Pointer Templates
%shared_ptr(XdmfAttribute)
%shared_ptr(XdmfAttributeCenter)
%shared_ptr(XdmfAttributeType)
%shared_ptr(XdmfCurvilinearGrid)
%shared_ptr(XdmfDomain)
%shared_ptr(XdmfFunction)
%shared_ptr(XdmfGeometry)
%shared_ptr(XdmfGeometryType)
%shared_ptr(XdmfGraph)
%shared_ptr(XdmfGrid)
%shared_ptr(XdmfGridCollection)
%shared_ptr(XdmfGridCollectionType)
%shared_ptr(XdmfItemFactory)
%shared_ptr(XdmfMap)
%shared_ptr(XdmfReader)
%shared_ptr(XdmfRectilinearGrid)
%shared_ptr(XdmfRegularGrid)
%shared_ptr(XdmfSet)
%shared_ptr(XdmfSetType)
%shared_ptr(XdmfTime)
%shared_ptr(XdmfTopology)
%shared_ptr(XdmfTopologyType)
%shared_ptr(XdmfUnstructuredGrid)

%include Xdmf.hpp
%include XdmfGrid.hpp

%include XdmfAttribute.hpp
%include XdmfAttributeCenter.hpp
%include XdmfAttributeType.hpp
%include XdmfCurvilinearGrid.hpp
%include XdmfDomain.hpp
%include XdmfFunction.hpp
%include XdmfGeometry.hpp
%include XdmfGeometryType.hpp
%include XdmfGraph.hpp
%include XdmfGridCollection.hpp
%include XdmfGridCollectionType.hpp
%include XdmfItemFactory.hpp
%include XdmfMap.hpp
%include XdmfReader.hpp
%include XdmfRectilinearGrid.hpp
%include XdmfRegularGrid.hpp
%include XdmfSet.hpp
%include XdmfSetType.hpp
%include XdmfTime.hpp
%include XdmfTopology.hpp
%include XdmfTopologyType.hpp
%include XdmfUnstructuredGrid.hpp
