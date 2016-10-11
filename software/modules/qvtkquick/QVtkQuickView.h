///\file QVtkQuickView.h
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

#ifndef Q_VTK_QUICK_VIEW_H
#define Q_VTK_QUICK_VIEW_H

#include <QtQuick>

class vtkRenderWindow;
class vtkRenderWindowInteractor;
class vtkObject;

/// QVtkQuickView displays a VTK window in a Qt window.
class QVtkQuickView : public QQuickView
{
  Q_OBJECT

  class PrivateData;
  QScopedPointer<PrivateData> d_data;

  friend class QQuickRenderWindowItem;

public:
  QVtkQuickView(QWindow* parent = 0);
  QVtkQuickView(QQmlEngine* engine, QWindow * parent);
  QVtkQuickView(const QUrl& source, QWindow * parent = 0);
  virtual ~QVtkQuickView();

  virtual vtkRenderWindow* GetRenderWindow();
  virtual vtkRenderWindowInteractor* GetInteractor();

protected Q_SLOTS:
  virtual void onVtkRenderEvent();
  virtual void onSceneGraphInitialized();
  virtual void onSceneGraphInvalidated();
  virtual void onBeforeRendering();

protected:
  void initialize();

};

#endif
