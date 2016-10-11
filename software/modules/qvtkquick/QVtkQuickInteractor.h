#ifndef QVTKQUICKINTERACTORFILTER_H
#define QVTKQUICKINTERACTORFILTER_H

#include <QtCore>

class vtkGenericRenderWindowInteractor;
class vtkObject;
class QEvent;

class QVtkQuickInteractor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)

    class PrivateData;
    QScopedPointer<PrivateData> d_data;

public:
    QVtkQuickInteractor(QObject* parent);
    ~QVtkQuickInteractor();

    vtkGenericRenderWindowInteractor *getVtkInteractor() const;

    bool isEnabled() const;
    void setEnabled(bool arg);

protected:
    void vtkTimerEvent(vtkObject *caller, unsigned long eventId, void *callData);
    bool mapEvent(QEvent *);
    bool eventFilter(QObject *, QEvent *);

signals:
    void enabledChanged(bool);
};

#endif
