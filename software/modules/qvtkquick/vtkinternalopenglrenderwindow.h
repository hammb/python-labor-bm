#ifndef VTKINTERNALOPENGLRENDERWINDOW_H
#define VTKINTERNALOPENGLRENDERWINDOW_H

#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>
#include <QQuickFramebufferObject>

#include <vtkGenericOpenGLRenderWindow.h>

class vtkInternalOpenGLRenderWindow : public vtkGenericOpenGLRenderWindow, protected QOpenGLFunctions
{
    QQuickFramebufferObject::Renderer* m_qtFboRenderer;

public:
    static vtkInternalOpenGLRenderWindow* New();
    vtkTypeMacro(vtkInternalOpenGLRenderWindow, vtkGenericOpenGLRenderWindow)

    virtual void OpenGLInitState();
    virtual void Render();

    void InternalRender();
    void SetFramebufferObject(QOpenGLFramebufferObject *fbo);
    void SetFramebufferRenderer(QQuickFramebufferObject::Renderer* qsgRenderer);

protected:
    vtkInternalOpenGLRenderWindow();
    ~vtkInternalOpenGLRenderWindow();
};


#endif // VTKINTERNALOPENGLRENDERWINDOW_H
