/*=========================================================================
  
  Program:   Visualization Toolkit
  Module:    vtkXdmfMPIController.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  
Copyright (c) 1993-2001 Ken Martin, Will Schroeder, Bill Lorensen 
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * Neither name of Ken Martin, Will Schroeder, or Bill Lorensen nor the names
   of any contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

 * Modified source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
// .NAME vtkXdmfMPIController - Process communication using MPI
// .SECTION Description
// vtkXdmfMPIController is a concrete class which implements the
// abstract multi-process control methods defined in vtkMultiProcessController
// using MPI (Message Passing Interface) 
// cf. Using MPI / Portable Parallel Programming with the Message-Passing
// Interface, Gropp et al, MIT Press.
// It also provide functionality specific to MPI and not present in 
// vtkMultiProcessController.
// Before any MPI communication can occur Initialize() must be called
// by all processes. It is required to be called once, controllers
// created after this need not call Initialize().
// At the end of the program Finalize() must be called by all
// processes. 
// The use of user-defined communicators are supported with 
// vtkMPICommunicator and vtkMPIGroup. Note that a duplicate of
// the user defined communicator is used for internal communications (RMIs).
// This communicator has the same properties as the user one except that
// it has a new context which prevents the two communicators from
// interfering with each other.

// .SECTION see also
// vtkOutputPort vtkInputPort  vtkMultiProcessController
// vtkMPICommunicator vtkMPIGroup

#ifndef __vtkXdmfMPIController_h
#define __vtkXdmfMPIController_h

#include "vtkMPIController.h"

class VTK_PARALLEL_EXPORT vtkXdmfMPIController : public vtkMPIController
{

public:

  static vtkXdmfMPIController *New();
  vtkTypeMacro(vtkXdmfMPIController,vtkMultiProcessController);

  virtual void FakeInitialize( void );

protected:

};


#endif


