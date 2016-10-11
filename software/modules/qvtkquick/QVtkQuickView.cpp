///\file QVtkQuickView.cpp
///\brief QVtkQuickView - Display a vtkRenderWindow under a QML scene graph
///  
///Displays a vtkRenderWindow under a QML scene
///
///\par Projekt : VTK/QML Interaction
///
///\par Company:   Mannheim University of Applied Sciences\n
///                emb-lab\n
///                Adress:     Paul-Wittsack-Strasse 10, D-68163 Mannheim, Germany\n
///                eMail:      \send zimmermann.emb@googlemail.com\n
///
///\author         Timo Zimmermann

#include "QVtkQuickView.h"
#include <memory>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFunctions>

#include <QMouseEvent>
#include <QResizeEvent>

#include <QtDebug>

#include "vtkCommand.h"
#include "vtkSmartPointer.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkRenderWindow.h"


class QVtkQuickView::PrivateData
{
public:
      PrivateData() { }

      vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
};


QVtkQuickView::QVtkQuickView(QWindow* parent)
    : QQuickView(parent), d_data(new PrivateData())
{
    initialize();
}

QVtkQuickView::QVtkQuickView(QQmlEngine* engine, QWindow * parent)
  : QQuickView(engine, parent), d_data(new PrivateData())
{
    initialize();
}

QVtkQuickView::QVtkQuickView(const QUrl& source, QWindow * parent)
  : QQuickView(source, parent), d_data(new PrivateData())
{
    initialize();
}


QVtkQuickView::~QVtkQuickView()
{

}

void  QVtkQuickView::initialize()
{
    QObject::connect(this, SIGNAL(sceneGraphInitialized()), this, SLOT(onSceneGraphInitialized()), Qt::DirectConnection);
    QObject::connect(this, SIGNAL(sceneGraphInvalidated()), this, SLOT(onSceneGraphInvalidated()), Qt::DirectConnection);
    QObject::connect(this, SIGNAL(beforeRendering()), this, SLOT(onBeforeRendering()), Qt::DirectConnection);

    setClearBeforeRendering(false);

    d_data->renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    d_data->renderWindow->SwapBuffersOff();

    d_data->renderWindow->SetSize(width(), height());
    d_data->renderWindow->SetPosition(x(), y());


    //re-render of the vtk scene must be triggered by the interactor not the window itself
    d_data->renderWindow->AddObserver(vtkCommand::RenderEvent, this, &QVtkQuickView::onVtkRenderEvent);
}

vtkRenderWindow* QVtkQuickView::GetRenderWindow()
{
    return d_data->renderWindow;
}

vtkRenderWindowInteractor* QVtkQuickView::GetInteractor()
{
    return d_data->renderWindow->GetInteractor();
}

void QVtkQuickView::onVtkRenderEvent()
{
    //trigger the rendering of the whole scene
    QQuickView::update();
}

void QVtkQuickView::onSceneGraphInitialized()
{
    d_data->renderWindow->OpenGLInitContext();
}

void QVtkQuickView::onSceneGraphInvalidated()
{
    d_data->renderWindow->Finalize();
}

void QVtkQuickView::onBeforeRendering()
{
    //Incompatibility between GLSL and the OpenGL 1.x api used by vtk
    //The qml scene graph seems to use a gl-programm and does not unload it - maybe a qt bug?
    openglContext()->functions()->glUseProgram(0);

    d_data->renderWindow->PushState();
    d_data->renderWindow->OpenGLInitState();
    d_data->renderWindow->Render();
    d_data->renderWindow->WaitForCompletion();
    d_data->renderWindow->PopState();
}



