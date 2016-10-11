#include "vtkinternalopenglrenderwindow.h"
#include "QVtkQuickFramebufferObjectRenderer.h"

#include <vtkObjectFactory.h>
#include <vtkRendererCollection.h>
#include <vtkCamera.h>
#include <vtkSmartPointer.h>

void vtkInternalOpenGLRenderWindow::OpenGLInitState()
{
    Superclass::OpenGLInitState();

    //Before any of the gl* functions in QOpenGLFunctions are called for a
    //given OpenGL context, an initialization must be run within that context
    //this->MakeCurrent();
    initializeOpenGLFunctions();

    glUseProgram(0);            //Shouldn't Superclass::OpenGLInitState() handle this?
    //glDisable(GL_DEPTH_TEST); //depth buffer fighting between the cone and the backround without this
    glDisable(GL_BLEND);        //doesn't seem crucial (?) but it is one of the differnces that showed up in apitrace analysis
}

///Override to use deferred rendering - Tell the QSG that we need to
///be rendered which will then, at the appropriate time, call
///InternalRender to do the actual OpenGL rendering.
void vtkInternalOpenGLRenderWindow::Render()
{
    if(this->m_qtFboRenderer == nullptr) return;

    static_cast<QVtkQuickFramebufferObjectRenderer*>(this->m_qtFboRenderer)->update();
}


///Do the actual OpenGL rendering
void vtkInternalOpenGLRenderWindow::InternalRender()
{
    Superclass::Render();
    WaitForCompletion();
}

///Provides a convenient way to set the protected FBO ivars from an existing
///FBO that was created and owned by Qt's FBO abstraction class QOpenGLFramebufferObject
void vtkInternalOpenGLRenderWindow::SetFramebufferObject(QOpenGLFramebufferObject *fbo)
{
    //QOpenGLFramebufferObject documentation states that "The color render
    //buffer or texture will have the specified internal format, and will
    //be bound to the GL_COLOR_ATTACHMENT0 attachment in the framebuffer
    //object"
    this->SetStereoRender(0);
    this->AlphaBitPlanesOn();
    this->SetBackLeftBuffer(static_cast<unsigned int>(GL_COLOR_ATTACHMENT0));
    this->SetFrontLeftBuffer(static_cast<unsigned int>(GL_COLOR_ATTACHMENT0));
    this->SetBackBuffer(static_cast<unsigned int>(GL_COLOR_ATTACHMENT0));
    this->SetFrontBuffer(static_cast<unsigned int>(GL_COLOR_ATTACHMENT0));

    //Save GL objects by static casting to standard C types. GL* types
    //are not allowed in VTK header files.
    QSize fboSize = fbo->size();
    this->Size[0] = fboSize.width();
    this->Size[1] = fboSize.height();
    this->NumberOfFrameBuffers = 1;
    this->FrameBufferObject = static_cast<unsigned int>(fbo->handle());
    this->DepthRenderBufferObject = 0; // static_cast<unsigned int>(depthRenderBufferObject);
    this->TextureObjects[0] = static_cast<unsigned int>(fbo->texture());
    this->OffScreenRendering = 1;
    this->OffScreenUseFrameBuffer = 1;
    this->Modified();
}

void vtkInternalOpenGLRenderWindow::SetFramebufferRenderer(QQuickFramebufferObject::Renderer *qsgRenderer)
{
    this->m_qtFboRenderer = qsgRenderer;
}

vtkInternalOpenGLRenderWindow::vtkInternalOpenGLRenderWindow() : vtkGenericOpenGLRenderWindow()
  ,m_qtFboRenderer(nullptr)
{

}

vtkInternalOpenGLRenderWindow::~vtkInternalOpenGLRenderWindow()
{
    //Prevent superclass destructors from destroying the framebuffer object.
    //QOpenGLFramebufferObject owns the FBO and manages it's lifecyle.
    this->OffScreenRendering = 0;
}

vtkStandardNewMacro(vtkInternalOpenGLRenderWindow)
