#include "QVtkQuickFramebufferObjectRenderer.h"
#include "QVtkQuickFrameBufferObjectItem.h"
#include "vtkinternalopenglrenderwindow.h"

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkObjectFactory.h>
#include <vtkRendererCollection.h>
#include <vtkCamera.h>
#include <vtkSmartPointer.h>

class QVtkQuickFramebufferObjectRenderer;

class QVtkQuickFramebufferObjectRenderer::PrivateData
{
public:
    vtkSmartPointer<vtkInternalOpenGLRenderWindow> vtkRenderWindow;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> vtkGenericRenderWindow;
    QOpenGLFramebufferObject* framebufferObject;
};

QVtkQuickFramebufferObjectRenderer::QVtkQuickFramebufferObjectRenderer(QObject* parent) : QObject(parent)
  ,d_data(new PrivateData)
{
    d_data->vtkRenderWindow = vtkSmartPointer<vtkInternalOpenGLRenderWindow>::New();
    d_data->vtkGenericRenderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    d_data->framebufferObject = nullptr;

    d_data->vtkRenderWindow->Register(nullptr);             //Increase reference count
    d_data->vtkRenderWindow->SetFramebufferRenderer(this);
}

QVtkQuickFramebufferObjectRenderer::~QVtkQuickFramebufferObjectRenderer()
{
    d_data->vtkRenderWindow->SetFramebufferRenderer(nullptr);
    d_data->vtkRenderWindow->Delete();                      //Decrease reference count
}

vtkGenericOpenGLRenderWindow *QVtkQuickFramebufferObjectRenderer::getVtkRenderWindow() const
{
    return d_data->vtkRenderWindow.Get();
}

///The render AND gui thread are BLOCKED. The only safe place to exchange data between the item and the renderer
void QVtkQuickFramebufferObjectRenderer::synchronize(QQuickFramebufferObject *fboItem)
{
    QVtkQuickFramebufferObjectItem* item = static_cast<QVtkQuickFramebufferObjectItem*>(fboItem);

    if(item == nullptr) {
        qWarning() << "Wrong FBO item type!";
        return;
    }

    //The first synchronize call - right before the the framebufferObject is created for the first time
    //initialize the FBO item
    if (d_data->framebufferObject == nullptr) {
        qDebug("QVtkQuickFramebufferObjectRenderer::setupPipeline");
        item->setupPipeline(getVtkRenderWindow());
        getVtkRenderWindow()->Start();
    }

    item->updatePipeline();
}

///\remarks Called from the render thread when the GUI thread is NOT blocked
void QVtkQuickFramebufferObjectRenderer::render()
{
    d_data->vtkRenderWindow->PushState();
    d_data->vtkRenderWindow->OpenGLInitState();
    d_data->vtkRenderWindow->InternalRender();
    d_data->vtkRenderWindow->PopState();
}

QOpenGLFramebufferObject *QVtkQuickFramebufferObjectRenderer::createFramebufferObject(const QSize &size)
{
    qDebug("QVtkQuickFramebufferObjectRenderer::createFramebufferObject");

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::Depth);

    d_data->framebufferObject = new QOpenGLFramebufferObject(size, format);
    d_data->vtkRenderWindow->SetFramebufferObject(d_data->framebufferObject);

    return d_data->framebufferObject;
}
