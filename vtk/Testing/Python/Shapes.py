from vtk import *
from libvtkXdmfPython import *
import sys

r = vtkXdmfReader()
r.SetFileName(sys.argv[1])
r.UpdateInformation()
r.DisableAllGrids()
#r.EnableGrid("Brick of Hex")
r.EnableAllGrids()
r.EnableAllArrays()
r.Update()

cnt = r.GetNumberOfGrids()
cnt = 2

for a in range(cnt):
  r.GetOutput(a).SetUpdateExtentToWholeExtent()
  r.GetOutput(a).Update()
  print r.GetOutput(a)

