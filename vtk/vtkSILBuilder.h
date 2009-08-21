/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkSILBuilder.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkSILBuilder - helper class to build a SIL.
// .SECTION Description

#ifndef __vtkSILBuilder_h
#define __vtkSILBuilder_h

#include "vtkObject.h"

class vtkUnsignedCharArray;
class vtkStringArray;
class vtkMutableDirectedGraph;

class VTK_EXPORT vtkSILBuilder : public vtkObject
{
public:
  static vtkSILBuilder* New();
  vtkTypeRevisionMacro(vtkSILBuilder, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  void SetSIL(vtkMutableDirectedGraph*);
  vtkGetObjectMacro(SIL, vtkMutableDirectedGraph);

  void Initialize();

  vtkIdType AddVertex(const char* name);
  vtkIdType AddChildEdge(vtkIdType parent, vtkIdType child);
  vtkIdType AddCrossEdge(vtkIdType src, vtkIdType dst);


  vtkGetMacro(RootVertex, vtkIdType);
//BTX
protected:
  vtkSILBuilder();
  ~vtkSILBuilder();

  vtkStringArray* NamesArray;
  vtkUnsignedCharArray* CrossEdgesArray;
  vtkMutableDirectedGraph* SIL;

  vtkIdType RootVertex;

private:
  vtkSILBuilder(const vtkSILBuilder&); // Not implemented.
  void operator=(const vtkSILBuilder&); // Not implemented.
//ETX
};

#endif


