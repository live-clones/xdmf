/*
  To create C++ files:
XdmfTcl.cxx:
swig -v -c++ -make_default -includeall -tcl -prefix Xdmf  -namespace $(ICE_INCLUDES) -o XdmfTcl.cxx ${srcdir}/Xdmf.i

XdmfPython.cxx:
swig -v -c++ -make_default -includeall -shadow -python $(ICE_INCLUDES) -o XdmfPython.cxx ${srcdir}/Xdmf.i

XdmfJava.cxx:
swig -v -c++ -make_default -includeall -shadow -java $(ICE_INCLUDES) -o XdmfJava.cxx ${srcdir}/Xdmf.i;  
*/

%module Xdmf
%{
#include <XdmfArray.h>
#include <XdmfAttribute.h>
#include <XdmfCharArray.h>
#include <XdmfDOM.h>
#include <XdmfDataDesc.h>
#include <XdmfExpression.h>
#include <XdmfFormat.h>
#include <XdmfFormatHDF.h>
#include <XdmfFormatMulti.h>
#include <XdmfFormatXML.h>
#include <XdmfGeometry.h>
#include <XdmfGrid.h>
#include <XdmfHDF.h>
#include <XdmfHDFSupport.h>
#include <XdmfHeavyData.h>
#include <XdmfLightData.h>
#include <XdmfObject.h>
#include <XdmfParameter.h>
#include <XdmfTopology.h>
#include <XdmfTransform.h>
#include <XdmfXNode.h>
#include <XdmfNDGM.h>
%}
#include <XdmfArray.h>
#include <XdmfAttribute.h>
#include <XdmfCharArray.h>
#include <XdmfDOM.h>
#include <XdmfDataDesc.h>
#include <XdmfExpression.h>
#include <XdmfFormat.h>
#include <XdmfFormatHDF.h>
#include <XdmfFormatMulti.h>
#include <XdmfFormatXML.h>
#include <XdmfGeometry.h>
#include <XdmfGrid.h>
#include <XdmfHDF.h>
#include <XdmfHeavyData.h>
#include <XdmfLightData.h>
#include <XdmfObject.h>
#include <XdmfParameter.h>
#include <XdmfTopology.h>
#include <XdmfTransform.h>
#include <XdmfXNode.h>
#include <XdmfNDGM.h>
