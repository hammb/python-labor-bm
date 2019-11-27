
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
#DICOMimageReader.SetFileName(dirpath)
DICOMimageReader.Update()

#imageViewer = vtk.vtkImageViewer2()
#imageViewer.SetInputConnection(DICOMimageReader.GetOutputPort());

ism = vtk.vtkImageSliceMapper()
ism.SetInputConnection(DICOMimageReader.GetOutputPort())

mapActor = vtk.vtkImageActor()
mapActor.SetMapper(ism)
mapActor.SetZSlice(80)
#sliceTextProp = vtk.vtkTextProperty()
#sliceTextProp.SetFontFamilyToCourier()
#sliceTextProp.SetFontSize(20)
#sliceTextProp.SetVerticalJustificationToBottom()
#sliceTextProp.SetJustificationToLeft()

#sliceTextMapper  = vtk.vtkTextMapper()
#msg = StatusMessage.Format(imageViewer.GetSliceMin(),imageViewer.GetSliceMax())
#sliceTextMapper.SetTextProperty(sliceTextProp)

#sliceTextActor = vtk.vtkActor2D()
#sliceTextActor.SetMapper(sliceTextMapper)
#sliceTextActor.SetPosition(15, 10)

#usageTextProp = vtk.vtkTextProperty()
#usageTextProp.SetFontFamilyToCourier()
#usageTextProp.SetFontSize(14)
#usageTextProp.SetVerticalJustificationToTop()
#usageTextProp.SetJustificationToLeft()

#usageTextMapper = vtk.vtkTextMapper()
#usageTextMapper.SetTextProperty(usageTextProp)

#usageTextActor = vtk.vtkActor2D()
#usageTextActor.SetMapper(usageTextMapper)
#usageTextActor.GetPositionCoordinate().SetCoordinateSystemToNormalizedDisplay()
#usageTextActor.GetPositionCoordinate().SetValue( 0.05, 0.95)

#imageViewer.SetupInteractor(iren)
#imageViewer.SetSlice(40)
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
#isoMapper.SetInputConnection(iso.GetOutputPort())
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
ren.AddActor(mapActor)
#ren.AddActor2D(usageTextActor)
ren.SetBackground(colors.GetColor3d("SlateGray"))
#ren.GetActiveCamera().SetFocalPoint(0, 0, 0)
#ren.GetActiveCamera().SetPosition(0, -1, 0)
#ren.GetActiveCamera().SetViewUp(0, 0, -1)
#ren.ResetCamera()
#ren.GetActiveCamera().Dolly(1.5)
#ren.ResetCameraClippingRange()

renWin.SetSize(600, 600)

renWin.Render()
iren.Start()
#imageViewer.Render()
#imageViewer.GetRenderer().ResetCamera()
#imageViewer.Render()
iren.Start();