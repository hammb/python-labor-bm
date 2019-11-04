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
DICOMimageReader.Update()

locator = vtk.vtkMergePoints()
locator.SetDivisions(64, 64, 92)
locator.SetNumberOfPointsPerBucket(2)
locator.AutomaticOff()

iso = vtk.vtkMarchingCubes()
iso.SetInputConnection(DICOMimageReader.GetOutputPort())
iso.ComputeGradientsOn()
iso.ComputeScalarsOff()
iso.SetValue(0, 1150)
iso.SetLocator(locator)

isoMapper = vtk.vtkPolyDataMapper()
isoMapper.SetInputConnection(iso.GetOutputPort())
isoMapper.ScalarVisibilityOff()

isoActor = vtk.vtkActor()
isoActor.SetMapper(isoMapper)
isoActor.GetProperty().SetColor(colors.GetColor3d("Wheat"))

outline = vtk.vtkOutlineFilter()
outline.SetInputConnection(DICOMimageReader.GetOutputPort())

outlineMapper = vtk.vtkPolyDataMapper()
outlineMapper.SetInputConnection(outline.GetOutputPort())

outlineActor = vtk.vtkActor()
outlineActor.SetMapper(outlineMapper)

    # Add the actors to the renderer, set the background and size.
    #
ren.AddActor(outlineActor)
ren.AddActor(isoActor)
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