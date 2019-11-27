import vtk
import os

dirpath = os.getcwd()+ os.sep + 'Marching Man'

colors = vtk.vtkNamedColors()

ren = vtk.vtkRenderer()

renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(ren)

iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

DICOMimageReader = vtk.vtkDICOMImageReader()
DICOMimageReader.SetDirectoryName(dirpath)

vtkImageShiftScale = vtk.vtkImageShiftScale()
vtkImageShiftScale.SetInputConnection(DICOMimageReader.GetOutputPort())
vtkImageShiftScale.SetOutputScalarTypeToUnsignedShort()

vtkGPUVolumeRayCastMapper = vtk.vtkGPUVolumeRayCastMapper()
vtkGPUVolumeRayCastMapper.SetInputConnection(vtkImageShiftScale.GetOutputPort())

volumeColor = vtk.vtkColorTransferFunction()
volumeColor.AddRGBPoint(100,0.0, 0.0, 0.0)
volumeColor.AddRGBPoint(950, 1.0, 0.5, 0.3)
volumeColor.AddRGBPoint(1200, 1.0, 0.5, 0.3)
volumeColor.AddRGBPoint(1550, 1.0, 1.0, 0.9)

volumeScalarOpacity = vtk.vtkPiecewiseFunction()
volumeScalarOpacity.AddPoint(100, 0.00)
volumeScalarOpacity.AddPoint(950, 0.05)
volumeScalarOpacity.AddPoint(1200, 0.01)
volumeScalarOpacity.AddPoint(1550, 0.80)

volumeGradientOpacity = vtk.vtkPiecewiseFunction()
volumeGradientOpacity.AddPoint(0, 0.0)
volumeGradientOpacity.AddPoint(20, 0.5)
volumeGradientOpacity.AddPoint(30, 1.0)

volumeProperty = vtk.vtkVolumeProperty()
volumeProperty.SetColor(volumeColor)
volumeProperty.SetScalarOpacity(volumeScalarOpacity)
volumeProperty.SetGradientOpacity(volumeGradientOpacity)

vtkVolume = vtk.vtkVolume()
vtkVolume.SetMapper(vtkGPUVolumeRayCastMapper)
vtkVolume.SetProperty(volumeProperty)


ren.AddVolume(vtkVolume)
ren.SetBackground(colors.GetColor3d("SlateGray"))
ren.GetActiveCamera().SetFocalPoint(0, 0, 0)
ren.GetActiveCamera().SetPosition(0, -1, 0)
ren.GetActiveCamera().SetViewUp(0, 0, -1)
ren.ResetCamera()
ren.GetActiveCamera().Dolly(1.5)
ren.ResetCameraClippingRange()

renWin.SetSize(640, 480)

renWin.Render()
iren.Start()