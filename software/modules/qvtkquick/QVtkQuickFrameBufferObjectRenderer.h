#ifndef QVTKQUICKFRAMEBUFFEROBJECTRENDERER_H
#define QVTKQUICKFRAMEBUFFEROBJECTRENDERER_H

#include <QOpenGLFunctions>
#include <QtCore>

#include <QQuickFramebufferObject>
#include <QOpenGLFramebufferObject>

class vtkGenericOpenGLRenderWindow;

class QVtkQuickFramebufferObjectRenderer : public QObject, public QQuickFramebufferObject::Renderer
{
    Q_OBJECT

    friend class vtkInternalOpenGLRenderWindow;

    class PrivateData;
    QScopedPointer<PrivateData> d_data;

public:
    QVtkQuickFramebufferObjectRenderer(QObject *parent = nullptr);
    virtual ~QVtkQuickFramebufferObjectRenderer();

    vtkGenericOpenGLRenderWindow* getVtkRenderWindow() const;

    virtual void synchronize(QQuickFramebufferObject * fboItem);
    virtual void render();

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);
};

#endif // QVTKQUICKFRAMEBUFFEROBJECTRENDERER_H
