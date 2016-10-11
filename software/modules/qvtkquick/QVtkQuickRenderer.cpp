#include "QVtkQuickRenderer.h"

#include "vtkRenderer.h"
#include "vtkSmartPointer.h"

class QVtkQuickRenderer::PrivateData
{
public:
    vtkSmartPointer<vtkRenderer> renderer;
    QVtkQuickCamera* camera;

};

QVtkQuickRenderer::QVtkQuickRenderer(QObject *parent) : QObject(parent)
  ,d_data(new PrivateData())
{
    d_data->renderer = vtkSmartPointer<vtkRenderer>::New();
    d_data->camera = new QVtkQuickCamera(d_data->renderer->GetActiveCamera(), this);
}

QVtkQuickRenderer::QVtkQuickRenderer(vtkRenderer *renderer, QObject *parent) : QObject(parent)
  ,d_data(new PrivateData())
{
    d_data->renderer = renderer;
    d_data->camera = new QVtkQuickCamera(d_data->renderer->GetActiveCamera(), this);
}

QVtkQuickRenderer::~QVtkQuickRenderer()
{

}

QVtkQuickCamera *QVtkQuickRenderer::camera()
{
    return d_data->camera;
}

vtkRenderer *QVtkQuickRenderer::getVtkRenderer()
{
    return d_data->renderer.Get();
}

void QVtkQuickRenderer::resetCamera()
{
    d_data->renderer->ResetCamera();
}
