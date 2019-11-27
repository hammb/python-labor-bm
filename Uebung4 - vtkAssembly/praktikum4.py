import vtk
import math

def cylinderFactory(height, radius, resolution):
    
    cylinder = vtk.vtkCylinderSource()
    cylinder.SetResolution(resolution)
    cylinder.SetRadius(radius)
    cylinder.SetHeight(height)
    
    return cylinder

def cubeFactory(xLength, yLength, zLength):
    
    cube = vtk.vtkCubeSource()
    cube.SetXLength(xLength)
    cube.SetYLength(yLength)
    cube.SetZLength(zLength)
    
    return cube

cylinder = cylinderFactory(5,1,100)

cylinderMapper = vtk.vtkPolyDataMapper()
cylinderMapper.SetInputConnection(cylinder.GetOutputPort())

cylinderActor1 = vtk.vtkActor()
cylinderActor2 = vtk.vtkActor()
cylinderActor3 = vtk.vtkActor()

cylinderActor1.SetMapper(cylinderMapper)
cylinderActor2.SetMapper(cylinderMapper)
cylinderActor3.SetMapper(cylinderMapper)

cylinderActor1.GetProperty().SetColor(1,0,0)
cylinderActor2.GetProperty().SetColor(0,1,0)
cylinderActor3.GetProperty().SetColor(0,0,1)

cylinderActor1.SetPosition(1,0,0)
cylinderActor2.SetPosition(4,0,0)
cylinderActor3.SetPosition(7,0,0)

cube = cubeFactory(10,1,2)

cubeMapper = vtk.vtkPolyDataMapper()
cubeMapper.SetInputConnection(cube.GetOutputPort())

cubeActor1 = vtk.vtkActor()
cubeActor2 = vtk.vtkActor()

cubeActor1.SetMapper(cubeMapper)
cubeActor2.SetMapper(cubeMapper)

cubeActor1.SetPosition(4,3,0)
cubeActor2.SetPosition(4,-3,0)

gruppe = vtk.vtkAssembly()

gruppe.AddPart(cylinderActor1)
gruppe.AddPart(cylinderActor2)
gruppe.AddPart(cylinderActor3)
gruppe.AddPart(cubeActor1)
gruppe.AddPart(cubeActor2)

gruppe2 = vtk.vtkAssembly()
gruppe2.ShallowCopy(gruppe)
gruppe2.SetPosition(0,0,-2)
gruppe2.RotateY(90)

###

ren = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()

renWin.AddRenderer(ren)

iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

ren.AddActor(gruppe)
ren.AddActor(gruppe2)

ren.SetBackground(0.53,0.81,0.98)
renWin.SetSize(400, 400)

iren.Initialize()

renWin.Render()

iren.Start()
iren.Disable()