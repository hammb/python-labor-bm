///\file QVtkQuickRenderWindowNode.cpp
///\brief QVtkQuickRenderWindowNode - A specialized geometry node that represents a vtk render window in a scene graph
///  
///A specialized texture node that represents a vtk render window in a qt5 scene graph
///
///\par Projekt : VTK/QML Interaction
///
///\par Company:   Mannheim University of Applied Sciences\n
///                emb-lab\n
///                Adress:     Paul-Wittsack-Strasse 10, D-68163 Mannheim, Germany\n
///                eMail:      \send zimmermann.emb@googlemail.com\n
///
///\par License:   This software is released under the GNU Public License v3.
///
///\author         Timo Zimmermann

#include "QVtkQuickRenderWindowNode.h"
#include "QSGFboTexture.h"

#include <QtQuick>

#include "vtkCommand.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkGenericRenderWindowInteractor.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkgl.h"
#include "vtkActor.h"
#include "vtkProp3D.h"


class QVtkQuickRenderWindowNode::PrivateData
{
public:
    PrivateData() 
        :frameBufferObject(nullptr)
        ,fboTexture(nullptr)
        ,renderWindow(nullptr)
        ,size()
    { }
    
    QScopedPointer<QOpenGLFramebufferObject> frameBufferObject;
    QScopedPointer<QSGFboTexture> fboTexture;
    QScopedPointer<QOpenGLContext> vtkRenderContext;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    QQuickWindow* quickWindow;
    QSize size;
};

QVtkQuickRenderWindowNode::QVtkQuickRenderWindowNode(QQuickWindow* quickWindow) : QSGSimpleTextureNode()
    ,d_data(new PrivateData())
{
    setFlag(UsePreprocess, true);
    d_data->quickWindow = quickWindow;

    //initialization

    //setup the render window
    d_data->renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    d_data->renderWindow->SetMapped(0);
    d_data->renderWindow->SetDoubleBuffer(0);
    d_data->renderWindow->SetSwapBuffers(0);
    d_data->renderWindow->SetFrontBuffer(vtkgl::COLOR_ATTACHMENT0_EXT);
    d_data->renderWindow->SetFrontLeftBuffer(vtkgl::COLOR_ATTACHMENT0_EXT);
    d_data->renderWindow->SetBackBuffer(vtkgl::COLOR_ATTACHMENT0_EXT);
    d_data->renderWindow->SetBackLeftBuffer(vtkgl::COLOR_ATTACHMENT0_EXT);

    //create the opengl context for vtk and the surface
    QOpenGLContext *current = d_data->quickWindow->openglContext();
    current->doneCurrent();

    d_data->vtkRenderContext.reset(new QOpenGLContext());
    d_data->vtkRenderContext->setShareContext(current);
    d_data->vtkRenderContext->setFormat(current->format());
    d_data->vtkRenderContext->create();

    //d_data->renderWindow->OpenGLInitContext(); //not sure what this does

    current->makeCurrent(d_data->quickWindow);
}

QVtkQuickRenderWindowNode::~QVtkQuickRenderWindowNode()
{

}

void QVtkQuickRenderWindowNode::setSize(const QSize &size)
{
    if(d_data->size == size)
        return;

    d_data->size = size;
    markDirty(DirtyGeometry);
}

QSize QVtkQuickRenderWindowNode::size() const
{
    return d_data->size;
}

vtkRenderWindow* QVtkQuickRenderWindowNode::renderWindow()
{
    return d_data->renderWindow.GetPointer();
}

void QVtkQuickRenderWindowNode::preprocess()
{
    //switch to the vtk context
    d_data->quickWindow->openglContext()->doneCurrent();
    d_data->vtkRenderContext->makeCurrent(d_data->quickWindow);

    //check if geometry has changed or fbo is empty
    if((d_data->frameBufferObject == nullptr) || (d_data->frameBufferObject->size() != d_data->size)) {
        //create new framebuffer and texture
        d_data->frameBufferObject.reset( new QOpenGLFramebufferObject(d_data->size, QOpenGLFramebufferObject::CombinedDepthStencil) );
        d_data->fboTexture.reset( new QSGFboTexture(d_data->frameBufferObject.data()) );
        setTexture(d_data->fboTexture.data());
    }

    //bind the frame buffer to the context
    d_data->frameBufferObject->bind();
    
    //Start() / restore VTK state
    d_data->renderWindow->PushState();
    d_data->renderWindow->OpenGLInitState();

    //Render
    d_data->renderWindow->SetSize(d_data->size.width(), d_data->size.height());
    d_data->renderWindow->Render();
    d_data->renderWindow->WaitForCompletion();

    //End() / save VTK state
    d_data->renderWindow->PopState();

    //release the framebuffer again
    d_data->frameBufferObject->release();

    //switch back to the scene graph context
    d_data->vtkRenderContext->doneCurrent();
    d_data->quickWindow->openglContext()->makeCurrent(d_data->quickWindow);
}
