///\file QVtkQuickImageViewerItem.cpp
///\brief QVtkQuickImageViewerItem - Displays a vtkRenderWindow as QQuickItem in a QML scene
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
///\author         Timo Zimmermann

#include "QVtkQuickImageViewerItem.h"
#include "QVtkQuickRenderWindowNode.h"

#include <QtQuick>
#include <QtDebug>

#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkResliceImageViewer.h"
#include "vtkRenderer.h"
#include "vtkActor.h"
#include "vtkImageData.h"
#include "vtkImageActor.h"
#include "vtkImageReslice.h"
#include "vtkLookupTable.h"
#include "vtkImageMapToColors.h"
#include "vtkImageMapper3D.h"
#include "vtkMatrix4x4.h"
#include "vtkCamera.h"

#include "vtkLineSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkPropAssembly.h"

#include "QVtkQuickImageData.h"
#include "vtkDICOMImageReader.h"

static double orientationCosines[][9] = { 
    {       //axial
        1, 0, 0,
        0,-1, 0,
        0, 0, 1,
    }, {    //coronal
        1, 0, 0,
        0, 0, 1,
        0, 1, 0,
    }, {    //sagittal
        0, 1, 0,
        0, 0, 1,
        1, 0, 0,
    }
};

static double orientationPresets[][16] = { 
    {       //axial
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1 
    }, {    //coronal
        1, 0, 0, 0,
        0, 0, 1, 0,
        0, 1, 0, 0,
        0, 0, 0, 1 
    }, {    //sagittal
        0, 0, 1, 0,
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 1 
    }
};

static Qt::GlobalColor sliceColorPresets[3] = {
    Qt::red, 
    Qt::green, 
    Qt::blue
};

class QVtkQuickImageViewerItem::PrivateData 
{
public:
    PrivateData()
        :imageData(new QVtkQuickImageData())
        ,resliceAxes(vtkSmartPointer<vtkMatrix4x4>::New())
        ,imageReslice(vtkSmartPointer<vtkImageReslice>::New())
        ,lookupTable(vtkSmartPointer<vtkLookupTable>::New())
        ,levelWindowMapper(vtkSmartPointer<vtkImageMapToColors>::New())
        ,imageActor(vtkSmartPointer<vtkImageActor>::New())
        ,imagePosition(0, 0, 0)
        ,viewPosition(0, 0, 0)
        ,sliceOrientation(0)
        ,level(0)
        ,window(2048)
        ,zoom(1.0)
        ,panX(1.0)
        ,panY(1.0)
        ,rollAngle(0)
    { 

    }

    QVtkQuickImageData* imageData;
    vtkSmartPointer<vtkMatrix4x4> resliceAxes;
    vtkSmartPointer<vtkImageReslice> imageReslice;
    vtkSmartPointer<vtkLookupTable> lookupTable;
    vtkSmartPointer<vtkImageMapToColors> levelWindowMapper;
    vtkSmartPointer<vtkImageActor> imageActor;
    QVector3D imagePosition;
    QVector3D viewPosition;
    int sliceOrientation;
    int level;
    int window;
    QPoint pressedMousePos;
    QPoint oldMousePos;
    Qt::MouseButtons causingButtons;
    double zoom;
    double panX;
    double panY;
    double rollAngle;

};


QVtkQuickImageViewerItem::QVtkQuickImageViewerItem(QQuickItem* parent) : QVtkQuickRenderWindowItem(parent)
    ,d_data(new PrivateData())
{
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);

    QObject::connect(this, &QVtkQuickImageViewerItem::imageDataChanged, this, &QVtkQuickImageViewerItem::update);
    QObject::connect(this, &QVtkQuickImageViewerItem::viewPositionChanged, this, &QVtkQuickImageViewerItem::update);
    QObject::connect(this, &QVtkQuickImageViewerItem::imagePositionChanged, this, &QVtkQuickImageViewerItem::update);
    QObject::connect(this, &QVtkQuickImageViewerItem::sliceOrientationChanged, this, &QVtkQuickImageViewerItem::update);
    QObject::connect(this, &QVtkQuickImageViewerItem::levelChanged, this, &QVtkQuickImageViewerItem::update);
    QObject::connect(this, &QVtkQuickImageViewerItem::windowChanged, this, &QVtkQuickImageViewerItem::update);
}


QVtkQuickImageViewerItem::~QVtkQuickImageViewerItem()
{
    
}

void QVtkQuickImageViewerItem::mouseMoveEvent(QMouseEvent * event)
{
    if(d_data->causingButtons == Qt::NoButton) return;

    QPoint offset =  d_data->oldMousePos - event->pos();
    if(d_data->causingButtons & Qt::LeftButton) {
        setSliceNumber(sliceNumber() + offset.y());
    }
    if(d_data->causingButtons & Qt::RightButton) {
        setLevel(level() + offset.x()*2);
        setWindow(window() + offset.y()*2);
    }
    d_data->oldMousePos = event->pos();
}

void QVtkQuickImageViewerItem::mousePressEvent(QMouseEvent * event)
{
    d_data->causingButtons |= event->buttons();
    d_data->pressedMousePos = d_data->oldMousePos = event->pos();
}

void QVtkQuickImageViewerItem::mouseReleaseEvent(QMouseEvent * event)
{
    if(d_data->causingButtons & Qt::LeftButton) {
        if(d_data->pressedMousePos == event->pos()) {
           setPosition(event->pos());
        }
    }

    d_data->pressedMousePos = d_data->oldMousePos = QPoint(0, 0);
    d_data->causingButtons = Qt::NoButton;
}

void QVtkQuickImageViewerItem::wheelEvent(QWheelEvent * event)
{
    if(event->delta() == 0) return;
    setSliceNumber( sliceNumber() + (event->delta()/qAbs(event->delta())) );
}

void QVtkQuickImageViewerItem::setImageData(QVtkQuickImageData* imageData)
{
    if(d_data->imageData == imageData) return;

    d_data->imageData = imageData;
    d_data->imageReslice->SetInputData(d_data->imageData->imageData());
    emit imageDataChanged();
}

QVtkQuickImageData* QVtkQuickImageViewerItem::imageData() const
{
    return d_data->imageData;
}

void QVtkQuickImageViewerItem::setImagePosition(QVector3D imagePosition)
{
    if(qFuzzyCompare(d_data->imagePosition, imagePosition)) return;

    QVector3D spacing = d_data->imageData->imageSpacing();
    d_data->imagePosition = imagePosition;

    double slice[4];
    double center[4];
    slice[0] = imagePosition.x()/spacing.x();
    slice[1] = imagePosition.y()/spacing.y();
    slice[2] = imagePosition.z()/spacing.z();
    slice[3] = 1;

    double orientationMatrix[16] = { 0 };
    vtkMatrix4x4::Invert(orientationPresets[d_data->sliceOrientation], orientationMatrix);
    vtkMatrix4x4::MultiplyPoint(orientationMatrix, slice, center);
    setViewPosition(QVector3D(center[0], center[1], center[2]));

    emit imagePositionChanged();
}

QVector3D QVtkQuickImageViewerItem::imagePosition() const
{
    return d_data->imagePosition;
}

void QVtkQuickImageViewerItem::setViewPosition(QVector3D viewPosition)
{
    if(qFuzzyCompare(d_data->viewPosition, viewPosition)) return;

    QVector3D spacing = d_data->imageData->imageSpacing();
    d_data->viewPosition = viewPosition;

    double slice[4];
    double center[4];
    slice[0] = viewPosition.x(); //*d_data->imageData->getImageSpacing((d_data->sliceOrientation+0) % 3);
    slice[1] = viewPosition.y(); //*d_data->imageData->getImageSpacing((d_data->sliceOrientation+1) % 3);
    slice[2] = viewPosition.z(); //*d_data->imageData->getImageSpacing((d_data->sliceOrientation+2) % 3);
    slice[3] = 1;

    vtkMatrix4x4::MultiplyPoint(orientationPresets[d_data->sliceOrientation], slice, center);
    setImagePosition(QVector3D(center[0]*spacing.x(), center[1]*spacing.y(), center[2]*spacing.z()));

    emit viewPositionChanged();
}

QVector3D QVtkQuickImageViewerItem::viewPosition() const
{
    return d_data->viewPosition;
}

void QVtkQuickImageViewerItem::setPosition(QPointF position)
{
    if(d_data->viewPosition.toPointF() == position) return;

    setViewPosition(QVector3D( position.x(), position.y(), viewPosition().z()));
}

QPointF QVtkQuickImageViewerItem::position() const
{
    return d_data->viewPosition.toPointF();
}

void QVtkQuickImageViewerItem::setSliceNumber(int sliceNumber)
{
    if(d_data->viewPosition.z() == sliceNumber) return;

    setViewPosition(QVector3D( viewPosition().x(), viewPosition().y(), qBound(0, sliceNumber, maxSliceNumber())));
}

int QVtkQuickImageViewerItem::sliceNumber() const
{
    return viewPosition().z();
}

void QVtkQuickImageViewerItem::setSliceOrientation(int sliceOrientation)
{
    if(d_data->sliceOrientation == sliceOrientation) return;

    d_data->sliceOrientation = sliceOrientation;
    emit sliceOrientationChanged();
}

int QVtkQuickImageViewerItem::sliceOrientation() const
{
    return d_data->sliceOrientation;
}

void QVtkQuickImageViewerItem::setLevel(int level)
{
    level = qBound(-(4096/2), level, 4096/2);
    if(d_data->level == level) return;

    d_data->level = level;
    emit levelChanged();
}

int QVtkQuickImageViewerItem::level() const
{
    return d_data->level;
}

void QVtkQuickImageViewerItem::setWindow(int window)
{
    window = qBound(1, window, 4096);
    if(d_data->window == window) return;

    d_data->window = window;
    emit windowChanged();
}

int QVtkQuickImageViewerItem::window() const
{
    return d_data->window;
}

int QVtkQuickImageViewerItem::maxSliceNumber() const
{
    return d_data->imageData->getImageDimension((d_data->sliceOrientation+2) % 3) - 1;
}

double QVtkQuickImageViewerItem::sliceHeight()
{
    return d_data->imageData->getImageDimension((d_data->sliceOrientation+1) % 3) * voxelHeight();
}

double QVtkQuickImageViewerItem::sliceWidth()
{
    return d_data->imageData->getImageDimension((d_data->sliceOrientation+0) % 3) * voxelWidth();
}

double QVtkQuickImageViewerItem::voxelHeight()
{
    return d_data->imageData->getImageSpacing((d_data->sliceOrientation+1) % 3);
}

double QVtkQuickImageViewerItem::voxelWidth()
{
    return d_data->imageData->getImageSpacing((d_data->sliceOrientation+0) % 3);
}

void QVtkQuickImageViewerItem::setZoom(double arg)
{
    if(d_data->zoom == arg) return;

    d_data->zoom = arg;
    emit zoomChanged();
}

double QVtkQuickImageViewerItem::zoom() const
{
    return d_data->zoom;
}

void QVtkQuickImageViewerItem::setPanX(double arg)
{
    if(d_data->panX == arg) return;

    d_data->panX = arg;
    emit panXChanged();
}

double QVtkQuickImageViewerItem::panX() const
{
    return d_data->panX;
}

void QVtkQuickImageViewerItem::setPanY(double arg)
{
    if(d_data->panY == arg) return;

    d_data->panY = arg;
    emit panYChanged();
}

double QVtkQuickImageViewerItem::panY() const
{
    return d_data->panY;
}

void QVtkQuickImageViewerItem::setRollAngle(double arg)
{
    if(d_data->rollAngle == arg) return;

    d_data->rollAngle = arg;
    emit rollAngleChanged();
}

double QVtkQuickImageViewerItem::rollAngle() const
{
    return d_data->rollAngle;
}

void QVtkQuickImageViewerItem::setupPipeline()
{
    d_data->resliceAxes->Identity();

    d_data->imageReslice->SetInputData(d_data->imageData->imageData());
    d_data->imageReslice->SetResliceAxes(d_data->resliceAxes);
    d_data->imageReslice->SetOutputDimensionality(2);
    d_data->imageReslice->SetInterpolationModeToLinear();

    d_data->lookupTable->SetRange(level()-window()/2., level()+window()/2.);
    d_data->lookupTable->SetValueRange(0.0, 1.0);       // from black to white
    d_data->lookupTable->SetSaturationRange(0.0, 0.0);  // no color saturation
    d_data->lookupTable->SetRampToLinear();
    d_data->lookupTable->Build();

    d_data->levelWindowMapper->SetLookupTable(d_data->lookupTable);
    d_data->levelWindowMapper->SetInputConnection(d_data->imageReslice->GetOutputPort());

    d_data->imageActor->GetMapper()->SetInputConnection(d_data->levelWindowMapper->GetOutputPort());

    renderer()->getVtkRenderer()->AddActor(d_data->imageActor);
    renderer()->getVtkRenderer()->ResetCamera();
    renderer()->getVtkRenderer()->GetActiveCamera()->OrthogonalizeViewUp();
    renderer()->getVtkRenderer()->GetActiveCamera()->ParallelProjectionOn();
}

void QVtkQuickImageViewerItem::updatePipeline()
{
    d_data->resliceAxes->DeepCopy(orientationPresets[d_data->sliceOrientation]);
    d_data->resliceAxes->SetElement(0, 3, imagePosition().x());
    d_data->resliceAxes->SetElement(1, 3, imagePosition().y());
    d_data->resliceAxes->SetElement(2, 3, imagePosition().z());
    d_data->resliceAxes->Modified();

    d_data->lookupTable->SetRange(level()-window()/2, level()+window()/2);

    renderer()->getVtkRenderer()->ResetCamera();
    double scale = qMin(sliceWidth(),sliceHeight())/2;
    double rollAngle = 1;
    scale *= d_data->zoom;
    rollAngle *= d_data->rollAngle;
    renderer()->getVtkRenderer()->GetActiveCamera()->SetParallelScale(scale);
    renderer() ->getVtkRenderer()->GetActiveCamera()->SetRoll(d_data->rollAngle);

    double camPos[3];
    double focalPoint[3];
    renderer()->getVtkRenderer()->GetActiveCamera()->GetPosition(camPos);
    renderer()->getVtkRenderer()->GetActiveCamera()->GetFocalPoint(focalPoint);
    double xPos;
    double yPos;

    if((oldPanX != d_data->panX) || (oldPanY != d_data->panY))
    {
            xPos = 210;//210*d_data->panX;//abs(camPos[0]);//210;//abs(camPos[0]);//*d_data->panX;//10*d_data->panX;//abs(camPos[0])*d_data->panX;
            yPos = 210;//210*d_data->panY;//abs(camPos[1]);//210;//abs(camPos[1]);//*d_data->panY;//10*d_data->panY;//abs(camPos[1])*d_data->panY;

            xPos *= d_data->panX;
            yPos *= d_data->panY;

            renderer()->getVtkRenderer()->GetActiveCamera()->SetFocalPoint(xPos,yPos,focalPoint[2]);
            renderer()->getVtkRenderer()->GetActiveCamera()->SetPosition(xPos,yPos,camPos[2]);
    }

    oldPanX = d_data->panX;
    oldPanY = d_data->panY;
}

QSGNode* QVtkQuickImageViewerItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* nodeData)
{
    //call base class to setup the render window, size, etc.
    QVtkQuickRenderWindowNode* renderWindowNode = static_cast<QVtkQuickRenderWindowNode*>( QVtkQuickRenderWindowItem::updatePaintNode(oldNode, nodeData) );
    if(renderWindowNode == nullptr) return oldNode;

    //add the crosshair in the scene graph
    if(renderWindowNode->childCount() < 2) {
        renderWindowNode->removeAllChildNodes();
        renderWindowNode->appendChildNode(new QSGSimpleRectNode());
        renderWindowNode->appendChildNode(new QSGSimpleRectNode());
    }

    QSGSimpleRectNode* horizontalLine = static_cast<QSGSimpleRectNode*>(renderWindowNode->childAtIndex(0));
    QSGSimpleRectNode* verticalLine = static_cast<QSGSimpleRectNode*>(renderWindowNode->childAtIndex(1));

    QRectF itemRect = boundingRect();
    itemRect.setHeight(1);
    itemRect.moveTop(d_data->viewPosition.y());
    horizontalLine->setColor(sliceColorPresets[(d_data->sliceOrientation+1) % 3]);
    horizontalLine->setRect(itemRect);

    itemRect = boundingRect();
    itemRect.setWidth(1);
    itemRect.moveLeft(d_data->viewPosition.x());
    verticalLine->setColor(sliceColorPresets[(d_data->sliceOrientation+2) % 3]);
    verticalLine->setRect(itemRect);

    return renderWindowNode;
}




