#ifndef QVTKQUICKCAMERA_H
#define QVTKQUICKCAMERA_H

#include <QtCore>

class vtkCamera;

class QVtkQuickCamera : public QObject
{
    Q_OBJECT

    class PrivateData;
    QScopedPointer<PrivateData> d_data;

public:
    explicit QVtkQuickCamera(QObject *parent = 0);
    QVtkQuickCamera(vtkCamera* camera, QObject *parent = 0);
    virtual ~QVtkQuickCamera();

    vtkCamera* getVtkCamera();

    Q_INVOKABLE void orthogonalizeViewUp();
    Q_INVOKABLE void dolly(double value);
    Q_INVOKABLE void roll(double angle);
    Q_INVOKABLE void azimuth(double angle);
    Q_INVOKABLE void yaw(double angle);
    Q_INVOKABLE void elevation(double angle);
    Q_INVOKABLE void pitch(double angle);
    Q_INVOKABLE void zoom(double factor);
    Q_INVOKABLE void setFocalPoint(double x,double y,double z);
    Q_INVOKABLE void setPosition(double x, double y, double z);
    Q_INVOKABLE QList<double> getPosition();
    Q_INVOKABLE QList<double> getFocalPoint();

signals:

};

#endif // QVTKQUICKCAMERA_H
