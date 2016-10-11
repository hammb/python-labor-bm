#ifndef QVTKFrameBufferObjectItem_h_
#define QVTKFrameBufferObjectItem_h_

#include <QtQuick/QQuickFramebufferObject>
#include "QVtkQuickRenderer.h"
#include "QVtkQuickInteractor.h"

class vtkGenericOpenGLRenderWindow;
class QVtkQuickFramebufferObjectRenderer;

class QVtkQuickFramebufferObjectItem : public QQuickFramebufferObject
{
    Q_OBJECT

    Q_PROPERTY(QVtkQuickRenderer* renderer READ renderer NOTIFY rendererChanged)
    Q_PROPERTY(QVtkQuickInteractor* interactor READ interactor NOTIFY interactorChanged)

    friend class QVtkQuickFramebufferObjectRenderer;

    class PrivateData;
    QScopedPointer<PrivateData> d_data;

public:
    QVtkQuickFramebufferObjectItem(QQuickItem* parent = nullptr);
    virtual ~QVtkQuickFramebufferObjectItem();

    Renderer *createRenderer() const;

    QVtkQuickRenderer* renderer() const;
    QVtkQuickInteractor* interactor() const;

signals:
    void rendererChanged();
    void interactorChanged();

protected:
    virtual void setupPipeline(vtkGenericOpenGLRenderWindow *renderWindow);
    virtual void updatePipeline();

};

#endif
