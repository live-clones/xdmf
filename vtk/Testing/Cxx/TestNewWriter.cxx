/*=========================================================================

  Program:   Visualization Toolkit
  Module:    TestNewWriter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//This tests vtkXdmfWriter2

#include "vtkXdmfWriter2.h"
#include "vtkDataObjectGenerator.h"
#include "vtkTimeSourceExample.h"
#include "vtkTemporalFractal.h"
#include "vtkCompositeDataPipeline.h"

#include "vtkImageData.h"

int main (int vtkNotUsed(ac), char **vtkNotUsed(av))
{
//  vtkCompositeDataPipeline *cdp = vtkCompositeDataPipeline::New();
//  vtkAlgorithm::SetDefaultExecutivePrototype(cdp);
//  cdp->Delete();

  vtkDataObjectGenerator *dog = vtkDataObjectGenerator::New();
  dog->SetProgram("ID1");
  dog->SetProgram("UF1");
  dog->SetProgram("RG1");
  dog->SetProgram("SG1");
  dog->SetProgram("PD1");
  dog->SetProgram("PD2");
  dog->SetProgram("UG1");
  
  dog->SetProgram("MB{ }");
  dog->SetProgram("MB{ID1}");
  dog->SetProgram("MB{UF1}");
  dog->SetProgram("MB{RG1}");
  dog->SetProgram("MB{SG1}");
  dog->SetProgram("MB{PD1}");
  dog->SetProgram("MB{PD2}");
  dog->SetProgram("MB{UG1}");
  dog->SetProgram("MB{ ID1 UF1 RG1 SG1 PD1 UG1 }");
  dog->SetProgram("HB[ (UF1)(UF1)(UF1) ]");

  vtkTimeSourceExample *tsrc = vtkTimeSourceExample::New();
//  tsrcl->Update();
//  tsrc->GetOutput()->PrintSelf(cerr, vtkIndent(0));

  vtkTemporalFractal *tsrc2 = vtkTemporalFractal::New();
  tsrc2->DiscreteTimeStepsOn();

  vtkXdmfWriter2 *xwriter = vtkXdmfWriter2::New();
  xwriter->SetFileName("WhadyaKnow.xmf");
//  xwriter->SetInputConnection(0, dog->GetOutputPort());
  xwriter->SetInputConnection(0, tsrc->GetOutputPort());
//  xwriter->SetInputConnection(0, tsrc2->GetOutputPort());
  xwriter->WriteAllTimeStepsOn();
  xwriter->Write();

  xwriter->Delete();
  tsrc2->Delete();
  tsrc->Delete();
  dog->Delete();

  //TODO:
  //Verify result file exists, verify it is sane, to make a pass/fail return code
  //Delete result file if test passed

  // vtkAlgorithm::SetDefaultExecutivePrototype(NULL); //needed or cdp is never freed

  return 0;
}


