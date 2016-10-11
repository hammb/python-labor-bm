///\file QVtkQuickRenderWindowItem.h
///\brief QVtkQuickRenderWindowItem - Displays a vtkRenderWindow as QQuickItem in a QML scene
///  
///Displays a vtkRenderWindow as QQuickItem in a QML scene
///
///\par Projekt:   VTK/QML Interaction
///
///\par Company:   Mannheim University of Applied Sciences\n
///                EMB-Lab\n
///                Address:    Paul-Wittsack-Strasse 10, D-68163 Mannheim, Germany\n
///                eMail:      \send zimmermann.emb@googlemail.com\n
///
///\author         Timo Zimmermann

#ifndef Q_VTK_QUICK_RENDER_WINDOW_ITEM_H
#define Q_VTK_QUICK_RENDER_WINDOW_ITEM_H

#include <QtQuick>
#include "QVtkQuickRenderer.h"
#include "QVtkQuickInteractor.h"

///WORK IN PROGRESS
class QVtkQuickRenderWindowItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QVtkQuickRenderer* renderer READ renderer NOTIFY rendererChanged)
    Q_PROPERTY(QVtkQuickInteractor* interactor READ interactor NOTIFY interactorChanged)
    Q_PROPERTY(QObject* pipe READ pipe WRITE setPipe NOTIFY pipeChanged)

    class PrivateData;
    QScopedPointer<PrivateData> d_data;

public:
    QVtkQuickRenderWindowItem(QQuickItem* parent = nullptr);
    virtual ~QVtkQuickRenderWindowItem();

    QVtkQuickRenderer* renderer() const;
    void setRenderer(QVtkQuickRenderer* renderer);

    QVtkQuickInteractor* interactor() const;

    QObject* pipe() const;
    void setPipe(QObject* arg );

protected:
    virtual void componentComplete();
    virtual void setupPipeline();
    virtual void updatePipeline();
    virtual QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*);

signals:
    void rendererChanged();
    void interactorChanged();
    void pipeChanged();
};

#endif
