import vtk
import math
#vtkCylinderSource
#vtkPolyDataMapper
#vtkActor
#vtkRenderer

cylinderBlue = vtk.vtkCylinderSource()
cylinderBlue.SetResolution(100)

cylinderRed = vtk.vtkCylinderSource()
cylinderRed.SetResolution(100)

cylinderGreen = vtk.vtkCylinderSource()
cylinderGreen.SetResolution(100)

#######################

cylinderMapperBlue = vtk.vtkPolyDataMapper()
cylinderMapperBlue.SetInputConnection(cylinderBlue.GetOutputPort())

cylinderMapperRed = vtk.vtkPolyDataMapper()
cylinderMapperRed.SetInputConnection(cylinderRed.GetOutputPort())

cylinderMapperGreen = vtk.vtkPolyDataMapper()
cylinderMapperGreen.SetInputConnection(cylinderRed.GetOutputPort())

######################

cylinderActorBlue = vtk.vtkActor()
cylinderActorBlue.SetMapper(cylinderMapperBlue)

cylinderActorRed = vtk.vtkActor()
cylinderActorRed.SetMapper(cylinderMapperRed)

cylinderActorGreen = vtk.vtkActor()
cylinderActorGreen.SetMapper(cylinderMapperRed)

#######################

matrix = vtk.vtkMatrix4x4()

matrix.SetElement(0,0,math.cos(45))
matrix.SetElement(0,1,0)
matrix.SetElement(0,2,math.sin(45))

matrix.SetElement(1,0,0)
matrix.SetElement(1,1,1)
matrix.SetElement(1,2,0)

matrix.SetElement(2,0,-math.sin(45))
matrix.SetElement(2,1,0)
matrix.SetElement(2,2,math.cos(45))

cylinderActorBlue.GetProperty().SetColor(0,0,1)
cylinderActorBlue.RotateX(45)

cylinderActorRed.GetProperty().SetColor(1,0,0)
#cylinderActorRed.RotateY(45)
cylinderActorRed.SetUserMatrix(matrix)

cylinderActorGreen.GetProperty().SetColor(0,1,0)
cylinderActorGreen.RotateZ(45)

#######################

cylinderActorRed.SetOrigin(0,0,2)
cylinderActorGreen.SetOrigin(2,0,2)

#########################

ren = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()

renWin.AddRenderer(ren)

iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

ren.AddActor(cylinderActorRed)
ren.AddActor(cylinderActorBlue)
ren.AddActor(cylinderActorGreen)

ren.SetBackground(0.53,0.81,0.98)
renWin.SetSize(400, 400)

iren.Initialize()

renWin.Render()

iren.Start()
iren.Disable()
