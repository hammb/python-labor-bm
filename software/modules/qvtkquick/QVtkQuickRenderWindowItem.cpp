///\file QVtkQuickRenderWindowItem.cpp
///\brief QVtkQuickRenderWindowItem - Displays a vtkRenderWindow as QQuickItem in a QML scene
///  
///Displays a vtkRenderWindow as QQuickItem in a QML scene
///
///\par Projekt:   VTK/QML Interaction
///
///\par Company:   Mannheim University of Applied Sciences\n
///                emb-lab\n
///                Adress:     Paul-Wittsack-Strasse 10, D-68163 Mannheim, Germany\n
///                eMail:      \send zimmermann.emb@googlemail.com\n
///
///\par License:   This software is released under the GNU Public License v3.
///
///\author         Timo Zimmermann

#include "QVtkQuickRenderWindowItem.h"
#include "QVtkQuickRenderWindowNode.h"
#include "QVtkQuickInteractor.h"

#include <QtQuick>
#include <QtDebug>

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkGenericRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkCommand.h"

Q_DECLARE_METATYPE(vtkRenderer*)

class QVtkQuickRenderWindowItem::PrivateData 
{
public:
    QScopedPointer<QVtkQuickRenderer> renderer;
    QScopedPointer<QVtkQuickInteractor> interactor;
    QObject* pipeline;
};

QVtkQuickRenderWindowItem::QVtkQuickRenderWindowItem(QQuickItem* parent) : QQuickItem(parent)
  ,d_data(new PrivateData())
{
    qRegisterMetaType<vtkRenderer*>("vtkRenderer*");

    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setFlag(QQuickItem::ItemHasContents, true);
    d_data->pipeline = nullptr;
    d_data->renderer.reset(new QVtkQuickRenderer(this));
    d_data->interactor.reset(new QVtkQuickInteractor(this));
    this->installEventFilter(d_data->interactor.data());

    QObject::connect(this, &QVtkQuickRenderWindowItem::pipeChanged, [this]() {
        if(d_data->pipeline == nullptr) return;
        QMetaObject::invokeMethod(d_data->pipeline, "setupPipeline", Q_ARG(vtkRenderer*, renderer()->getVtkRenderer()));
        QQuickItem::update();
    });

}

QVtkQuickRenderWindowItem::~QVtkQuickRenderWindowItem()
{
    
}

QVtkQuickRenderer *QVtkQuickRenderWindowItem::renderer() const
{
    return d_data->renderer.data();
}

QVtkQuickInteractor *QVtkQuickRenderWindowItem::interactor() const
{
    return d_data->interactor.data();
}

QSGNode* QVtkQuickRenderWindowItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData)
{
    Q_UNUSED(updatePaintNodeData)

    if (width() <= 0 || height() <= 0) {
        delete oldNode;
        return nullptr;
    }

    QVtkQuickRenderWindowNode* renderWindowNode = static_cast<QVtkQuickRenderWindowNode*>(oldNode);
    if (renderWindowNode == nullptr) {
        renderWindowNode = new QVtkQuickRenderWindowNode(window());
        renderWindowNode->renderWindow()->SetInteractor(d_data->interactor->getVtkInteractor());
    }

    renderWindowNode->setSize(boundingRect().size().toSize());
    renderWindowNode->setRect(boundingRect());
    renderWindowNode->markDirty(QSGNode::DirtyGeometry);

    //update the pipeline - this should be safe here, maybe this could also be moved to a separate thread
    updatePipeline();

    if(d_data->pipeline != nullptr)
        QMetaObject::invokeMethod(d_data->pipeline, "updatePipeline");

    //add renderer to the render window
    if(!renderWindowNode->renderWindow()->HasRenderer(d_data->renderer->getVtkRenderer())) {
        renderWindowNode->renderWindow()->AddRenderer(d_data->renderer->getVtkRenderer());
    }

    return renderWindowNode;
}

QObject *QVtkQuickRenderWindowItem::pipe() const
{
    return d_data->pipeline;
}

void QVtkQuickRenderWindowItem::setPipe( QObject* arg )
{
    if(d_data->pipeline == arg) return;

    d_data->pipeline = arg;
    emit pipeChanged();
}

void QVtkQuickRenderWindowItem::componentComplete()
{
    QQuickItem::componentComplete();
    setupPipeline();
}

void QVtkQuickRenderWindowItem::setupPipeline()
{

}

void QVtkQuickRenderWindowItem::updatePipeline()
{

}
