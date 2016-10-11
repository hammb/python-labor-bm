///\file QVtkQuickImageViewerItem.h
///\brief QVtkQuickImageViewerItem - Displays a vtkRenderWindow as QQuickItem in a QML scene
///  
///Displays a vtkRenderWindow as QQuickItem in a QML scene
///
///\par Projekt : VTK/QML Interaction
///
///\par Company:   Mannheim University of Applied Sciences\n
///                EMB-Lab\n
///                Address:    Paul-Wittsack-Strasse 10, D-68163 Mannheim, Germany\n
///                eMail:      \send zimmermann.emb@googlemail.com\n
///
///\author         Timo Zimmermann

#ifndef Q_VTK_QUICK_RESLICE_IMAGE_VIEWER_H
#define Q_VTK_QUICK_RESLICE_IMAGE_VIEWER_H

#include <QtQuick>

#include "QVtkQuickRenderWindowItem.h"

class QVtkQuickImageData;

///WORK IN PROGRESS
class QVtkQuickImageViewerItem : public QVtkQuickRenderWindowItem
{
public:
    enum Orientation {
        OrientationAxial = 0,
        OrientationCoronal = 1,
        OrientationSagittal = 2,
        OrientationFree
    };

private:
    Q_OBJECT
    Q_ENUMS(Orientation)
    Q_PROPERTY(QVtkQuickImageData* imageData READ imageData WRITE setImageData NOTIFY imageDataChanged)
    Q_PROPERTY(QVector3D imagePosition READ imagePosition WRITE setImagePosition NOTIFY imagePositionChanged)
    Q_PROPERTY(QVector3D viewPosition READ viewPosition WRITE setViewPosition NOTIFY viewPositionChanged)
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY viewPositionChanged)
    Q_PROPERTY(int sliceNumber READ sliceNumber WRITE setSliceNumber NOTIFY viewPositionChanged)
    Q_PROPERTY(int sliceOrientation READ sliceOrientation WRITE setSliceOrientation NOTIFY sliceOrientationChanged)
    Q_PROPERTY(int level READ level WRITE setLevel NOTIFY levelChanged)
    Q_PROPERTY(int window READ window WRITE setWindow NOTIFY windowChanged)
    Q_PROPERTY(int maxSliceNumber READ maxSliceNumber NOTIFY imageDataChanged)
    Q_PROPERTY(double sliceWidth READ sliceWidth NOTIFY imageDataChanged)
    Q_PROPERTY(double sliceHeight READ sliceHeight NOTIFY imageDataChanged)
    Q_PROPERTY(double voxelWidth READ voxelWidth NOTIFY imageDataChanged)
    Q_PROPERTY(double voxelHeight READ voxelHeight NOTIFY imageDataChanged)
    Q_PROPERTY(double zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(double panX READ panX WRITE setPanX NOTIFY panXChanged)
    Q_PROPERTY(double panY READ panY WRITE setPanY NOTIFY panYChanged)
    Q_PROPERTY(double rollAngle READ rollAngle WRITE setRollAngle NOTIFY rollAngleChanged)

    class PrivateData;
    QScopedPointer<PrivateData> d_data;

public:

    double oldPanX;
    double oldPanY;

    QVtkQuickImageViewerItem(QQuickItem* parent = nullptr);
    virtual ~QVtkQuickImageViewerItem();

    void setImageData(QVtkQuickImageData* imageData);
    QVtkQuickImageData* imageData() const;

    void setImagePosition(QVector3D imagePosition);
    QVector3D imagePosition() const;

    void setViewPosition(QVector3D viewPosition);
    QVector3D viewPosition() const;

    void setPosition(QPointF position);
    QPointF position() const;

    void setSliceNumber(int sliceNumber);
    int sliceNumber() const;

    void setSliceOrientation(int sliceOrientation);
    int sliceOrientation() const;

    void setLevel(int level);
    int level() const;

    void setWindow(int window);
    int window() const;
    
    int maxSliceNumber() const;
    double sliceHeight();
    double sliceWidth();
    double voxelHeight();
    double voxelWidth();

    void setZoom(double arg);
    double zoom() const;

    void setPanX(double arg);
    double panX() const;

    void setPanY(double arg);
    double panY() const;

    void setRollAngle(double arg);
    double rollAngle() const;

protected:
    void setupPipeline();
    void updatePipeline();
    virtual QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*);

    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void wheelEvent(QWheelEvent * event);

signals:
    void sliceOrientationChanged();
    void imageDataChanged();
    void levelChanged();
    void windowChanged();
    void imagePositionChanged();
    void viewPositionChanged();
    void zoomChanged();
    void panXChanged();
    void panYChanged();
    void rollAngleChanged();
 
};

#endif
