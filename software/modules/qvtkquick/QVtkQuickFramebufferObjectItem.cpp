#include "QVtkQuickFramebufferObjectItem.h"

#include <QOpenGLFunctions>
#include <QQuickFramebufferObject>
#include <QOpenGLFramebufferObject>

#include "vtkRenderWindowInteractor.h"
#include "vtkGenericRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkinternalopenglrenderwindow.h"

#include "QVtkQuickFramebufferObjectRenderer.h"

class QVtkQuickFramebufferObjectItem::PrivateData
{
public:
    QScopedPointer<QVtkQuickRenderer> renderer;
    QScopedPointer<QVtkQuickInteractor> interactor;
    QVtkQuickFramebufferObjectRenderer* fboRenderer;
};

QVtkQuickFramebufferObjectItem::QVtkQuickFramebufferObjectItem(QQuickItem* parent) : QQuickFramebufferObject(parent)
    ,d_data(new PrivateData)
{
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    d_data->renderer.reset(new QVtkQuickRenderer(this));
    d_data->interactor.reset(new QVtkQuickInteractor(this));
    d_data->fboRenderer = nullptr;
}

QVtkQuickFramebufferObjectItem::~QVtkQuickFramebufferObjectItem()
{

}

QQuickFramebufferObject::Renderer *QVtkQuickFramebufferObjectItem::createRenderer() const
{
    qDebug("QVtkQuickFramebufferObjectItem::createRenderer()");
    return d_data->fboRenderer = new QVtkQuickFramebufferObjectRenderer();
}

QVtkQuickRenderer *QVtkQuickFramebufferObjectItem::renderer() const
{
    return d_data->renderer.data();
}

QVtkQuickInteractor *QVtkQuickFramebufferObjectItem::interactor() const
{
    return d_data->interactor.data();
}

///\brief Called once before the FBO is created for the first time.
/// This method is called from render thread while the GUI thread is blocked.
void QVtkQuickFramebufferObjectItem::setupPipeline(vtkGenericOpenGLRenderWindow *renderWindow)
{
    this->installEventFilter(d_data->interactor.data());
    renderWindow->SetInteractor(d_data->interactor->getVtkInteractor());
    renderWindow->AddRenderer(d_data->renderer->getVtkRenderer());
}

///\brief Called from the renderer synchronize() function
/// This method is called from render thread while the GUI thread is blocked.
void QVtkQuickFramebufferObjectItem::updatePipeline()
{

}
