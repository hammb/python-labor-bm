#include "QVtkQuickCamera.h"

#include "vtkSmartPointer.h"
#include "vtkCamera.h"

class QVtkQuickCamera::PrivateData
{
public:
    vtkSmartPointer<vtkCamera> camera;
};

QVtkQuickCamera::QVtkQuickCamera(QObject *parent) : QObject(parent)
  ,d_data(new PrivateData())
{
    d_data->camera = vtkSmartPointer<vtkCamera>::New();
}

QVtkQuickCamera::QVtkQuickCamera(vtkCamera* camera, QObject *parent) : QObject(parent)
  ,d_data(new PrivateData())
{
    d_data->camera = camera;
}

QVtkQuickCamera::~QVtkQuickCamera()
{

}

vtkCamera *QVtkQuickCamera::getVtkCamera()
{
    return d_data->camera.Get();
}

void QVtkQuickCamera::orthogonalizeViewUp()
{
    d_data->camera->OrthogonalizeViewUp();
}

void QVtkQuickCamera::dolly(double value)
{
    d_data->camera->Dolly(value);
}

void QVtkQuickCamera::roll(double angle)
{
    d_data->camera->Roll(angle);
}

void QVtkQuickCamera::azimuth(double angle)
{
    d_data->camera->Azimuth(angle);
}

void QVtkQuickCamera::yaw(double angle)
{
    d_data->camera->Yaw(angle);
}

void QVtkQuickCamera::elevation(double angle)
{
    d_data->camera->Elevation(angle);
}

void QVtkQuickCamera::pitch(double angle)
{
    d_data->camera->Pitch(angle);
}

void QVtkQuickCamera::zoom(double factor)
{
    d_data->camera->Zoom(factor);
}

void QVtkQuickCamera::setFocalPoint(double x, double y, double z)
{
    d_data->camera->SetFocalPoint(x,y,z);
}

void QVtkQuickCamera::setPosition(double x, double y, double z)
{
    d_data->camera->SetPosition(x,y,z);
}

QList<double> QVtkQuickCamera::getPosition()
{
    double arg[3];
    d_data->camera->GetPosition(arg);
    QList<double> list;
    list << arg[0] << arg[1] << arg[2];

    return list;
}

QList<double> QVtkQuickCamera::getFocalPoint()
{
    double arg[3];
    d_data->camera->GetFocalPoint(arg);
    QList<double> list;
    list << arg[0] << arg[1] << arg[2];

    return list;
}
