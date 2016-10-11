///\file QVtkQuickRenderWindowNode.h
///\brief QVtkQuickRenderWindowNode - A specialized geometry node that represents a vtk render window in a scene graph
///  
///A specialized geometry node that represents a vtk render window in a scene graph
///
///\par Projekt : VTK/QML Interaction
///
///\par Company:   Mannheim University of Applied Sciences\n
///                emb-lab\n
///                Adress:     Paul-Wittsack-Strasse 10, D-68163 Mannheim, Germany\n
///                eMail:      \send zimmermann.emb@googlemail.com\n
///
///\author         Timo Zimmermann

#ifndef Q_VTK_QUICK_RENDER_WINDOW_NODE_H
#define Q_VTK_QUICK_RENDER_WINDOW_NODE_H

#include <QtQuick>

class vtkRenderWindow;
class vtkRenderWindowIneractor;

class QVtkQuickRenderWindowNode : public QSGSimpleTextureNode
{
    class PrivateData;
    QScopedPointer<PrivateData> d_data;

public:
    QVtkQuickRenderWindowNode(QQuickWindow* quickWindow);
    virtual ~QVtkQuickRenderWindowNode();

    void setSize(const QSize &size);
    QSize size() const;

    //vtk interface functions
    vtkRenderWindow* renderWindow();

protected:
    void preprocess();
};


#endif
