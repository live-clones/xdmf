from vtk import *
vtkxdmf = 0
try:
  import libvtkXdmfPython
  vtkxdmf = libvtkXdmfPython
except:
  import vtkXdmfPython
  vtkxdmf = vtkXdmfPython
import sys

r = vtkxdmf.vtkXdmfReader()
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

