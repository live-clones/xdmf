import vtk
import libvtkXdmfPython

o = vtk.vtkObject()
del o

reader = libvtkXdmfPython.vtkXdmfReader()
print reader
del reader
