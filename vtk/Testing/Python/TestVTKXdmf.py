import vtk

vtkxdmf = 0
try:
  import libvtkXdmfPython
  vtkxdmf = libvtkXdmfPython
except:
  import vtkXdmfPython
  vtkxdmf = vtkXdmfPython

o = vtk.vtkObject()
del o

reader = vtkxdmf.vtkXdmfReader()
print reader
del reader
