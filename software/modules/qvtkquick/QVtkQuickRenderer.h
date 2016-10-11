#ifndef QVTKQUICKRENDERER_H
#define QVTKQUICKRENDERER_H

#include <QtCore>
#include "QVtkQuickCamera.h"

class vtkRenderer;

class QVtkQuickRenderer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVtkQuickCamera* camera READ camera NOTIFY cameraChanged)

    class PrivateData;
    QScopedPointer<PrivateData> d_data;

public:
    explicit QVtkQuickRenderer(QObject *parent = 0);
    QVtkQuickRenderer(vtkRenderer* renderer, QObject *parent = 0);
    virtual ~QVtkQuickRenderer();

    QVtkQuickCamera* camera();
    vtkRenderer* getVtkRenderer();

    Q_INVOKABLE void resetCamera();

signals:
    void cameraChanged();

};

#endif // QVTKQUICKRENDERER_H
