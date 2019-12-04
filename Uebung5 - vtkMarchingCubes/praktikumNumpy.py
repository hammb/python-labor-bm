import vtk
from vtk.util import numpy_support
import os
import numpy
from matplotlib import pyplot, cm


dirpath = os.getcwd()+ os.sep + 'Marching Man'

colors = vtk.vtkNamedColors()

#ren = vtk.vtkRenderer()

#renWin = vtk.vtkRenderWindow()
#renWin.AddRenderer(ren)

#iren = vtk.vtkRenderWindowInteractor()
#iren.SetRenderWindow(renWin)

DICOMimageReader = vtk.vtkDICOMImageReader()
DICOMimageReader.SetDirectoryName(dirpath)
#DICOMimageReader.SetFileName(dirpath)
DICOMimageReader.Update()

_extent = DICOMimageReader.GetDataExtent()
ConstPixelDims = [_extent[1]-_extent[0]+1, _extent[3]-_extent[2]+1, _extent[5]-_extent[4]+1]

# Load spacing values
ConstPixelSpacing = DICOMimageReader.GetPixelSpacing()

x = numpy.arange(0.0, (ConstPixelDims[0]+1)*ConstPixelSpacing[0], ConstPixelSpacing[0])
y = numpy.arange(0.0, (ConstPixelDims[1]+1)*ConstPixelSpacing[1], ConstPixelSpacing[1])
z = numpy.arange(0.0, (ConstPixelDims[2]+1)*ConstPixelSpacing[2], ConstPixelSpacing[2])

# Get the 'vtkImageData' object from the reader
imageData = DICOMimageReader.GetOutput()
# Get the 'vtkPointData' object from the 'vtkImageData' object
pointData = imageData.GetPointData()
# Ensure that only one array exists within the 'vtkPointData' object
assert (pointData.GetNumberOfArrays()==1)
# Get the `vtkArray` (or whatever derived type) which is needed for the `numpy_support.vtk_to_numpy` function
arrayData = pointData.GetArray(0)

# Convert the `vtkArray` to a NumPy array
ArrayDicom = numpy_support.vtk_to_numpy(arrayData)
# Reshape the NumPy array to 3D using 'ConstPixelDims' as a 'shape'
ArrayDicom = ArrayDicom.reshape(ConstPixelDims, order='F')


pyplot.axes().set_aspect('equal', 'datalim')
pyplot.set_cmap(pyplot.gray())
pyplot.pcolormesh(x, y, numpy.flipud(numpy.rot90(ArrayDicom[:, :, 50])))
#locator = vtk.vtkMergePoints()
#locator.SetDivisions(64, 64, 92)
#locator.SetNumberOfPointsPerBucket(2)
#locator.AutomaticOff()


#iso = vtk.vtkMarchingCubes()
#iso.SetInputConnection(DICOMimageReader.GetOutputPort())
#iso.ComputeGradientsOn()
#iso.ComputeScalarsOff()
#iso.SetValue(0, 1150)
#iso.SetLocator(locator)

#isoMapper = vtk.vtkPolyDataMapper()
#isoMapper.SetInputConnection(view.GetOutputPort())
#isoMapper.ScalarVisibilityOff()

#isoActor = vtk.vtkActor()
#isoActor.SetMapper(isoMapper)
#isoActor.GetProperty().SetColor(colors.GetColor3d("Wheat"))

#outline = vtk.vtkOutlineFilter()
#outline.SetInputConnection(DICOMimageReader.GetOutputPort())

#outlineMapper = vtk.vtkPolyDataMapper()
#outlineMapper.SetInputConnection(outline.GetOutputPort())

#outlineActor = vtk.vtkActor()
#outlineActor.SetMapper(outlineMapper)

    # Add the actors to the renderer, set the background and size.
    #
#ren.AddActor(outlineActor)
#ren.AddActor(isoActor)
#ren.SetBackground(colors.GetColor3d("SlateGray"))
#ren.GetActiveCamera().SetFocalPoint(0, 0, 0)
#ren.GetActiveCamera().SetPosition(0, -1, 0)
#ren.GetActiveCamera().SetViewUp(0, 0, -1)
#ren.ResetCamera()
#ren.GetActiveCamera().Dolly(1.5)
#ren.ResetCameraClippingRange()

#renWin.SetSize(640, 480)

#renWin.Render()
#iren.Start()