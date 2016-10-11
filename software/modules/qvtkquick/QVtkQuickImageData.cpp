///\file QVtkQuickimageData.cpp
///\brief QVtkQuickimageData - Thin wrapper of vtkImageData.
///  
///Thin wrapper of vtkImageData.
///
///\par Projekt : VTK/QML Interaction
///
///\par Company:   Mannheim University of Applied Sciences\n
///                emb-lab\n
///                Adress:     Paul-Wittsack-Strasse 10, D-68163 Mannheim, Germany\n
///                eMail:      \send zimmermann.emb@googlemail.com\n
///
///\author         Timo Zimmermann

#include "QVtkQuickImageData.h"

#include "vtkImageData.h"
#include "vtkSmartPointer.h"

class QVtkQuickImageData::PrivateData 
{
public:
    PrivateData() 
        :imageData(vtkSmartPointer<vtkImageData>::New())
        ,blankImageData(vtkSmartPointer<vtkImageData>::New())
    { 
        imageSpacing[0] = imageSpacing[1] = imageSpacing[2] = 1.0;
        imageCenter[0] = imageCenter[1] = imageCenter[2] = 0.0;
        imageOrigin[0] = imageOrigin[1] = imageOrigin[2] = 0.0;
        imageExtent[0] = imageExtent[1] = imageExtent[2] = imageExtent[3] = imageExtent[4] = imageExtent[5] = 0;
        imageDimensions[0] = imageDimensions[1] = imageDimensions[2] = 0;
    }

    vtkSmartPointer<vtkImageData> imageData;
    vtkSmartPointer<vtkImageData> blankImageData;
    double imageSpacing[3];
    double imageCenter[3];
    double imageOrigin[3];
    int imageDimensions[3];
    int imageExtent[6];
};

QVtkQuickImageData::QVtkQuickImageData(vtkImageData* imageData, QQuickItem* parent) : QQuickItem(parent)
    ,d_data(new PrivateData())
{
    d_data->blankImageData->SetExtent(0, 1, 0, 1, 0, 1);
    d_data->blankImageData->AllocateScalars(VTK_DOUBLE, 3);
    
    setImageData(imageData);
}

QVtkQuickImageData::~QVtkQuickImageData()
{

}

void QVtkQuickImageData::setImageData(vtkImageData* imageData)
{
    if(d_data->imageData == imageData) return;

    if(imageData == nullptr) imageData = d_data->blankImageData;

    d_data->imageData->DeepCopy(imageData);
    d_data->imageData->GetSpacing(d_data->imageSpacing);
    d_data->imageData->GetCenter(d_data->imageCenter);
    d_data->imageData->GetOrigin(d_data->imageOrigin);
    d_data->imageData->GetExtent(d_data->imageExtent);
    d_data->imageData->GetDimensions(d_data->imageDimensions);

    emit imageChanged();
}

vtkImageData* QVtkQuickImageData::imageData() const
{
    return d_data->imageData;
}

qreal QVtkQuickImageData::getImageSpacing(int imageAxis) const
{
    return imageAxis >= ImageAxisCount ? 0.0 : d_data->imageSpacing[imageAxis];
}

qreal QVtkQuickImageData::getImageCenter(int imageAxis) const
{
    return imageAxis >= ImageAxisCount ? 0.0 : d_data->imageCenter[imageAxis];
}

qreal QVtkQuickImageData::getImageOrigin(int imageAxis) const
{
    return imageAxis >= ImageAxisCount ? 0.0 : d_data->imageOrigin[imageAxis];
}

int QVtkQuickImageData::getImageDimension(int imageAxis) const
{
    return imageAxis >= ImageAxisCount ? 0 : d_data->imageDimensions[imageAxis];
}

QVector3D QVtkQuickImageData::imageSpacing() const
{
    return QVector3D(d_data->imageSpacing[0], d_data->imageSpacing[1], d_data->imageSpacing[2]);
}

QVector3D QVtkQuickImageData::imageCenter() const
{
    return QVector3D(d_data->imageCenter[0], d_data->imageCenter[1], d_data->imageCenter[2]);
}

QVector3D QVtkQuickImageData::imageOrigin() const
{
    return QVector3D(d_data->imageOrigin[0], d_data->imageOrigin[1], d_data->imageOrigin[2]);
}

QVector3D QVtkQuickImageData::imageDimension() const
{
    return QVector3D(d_data->imageDimensions[0], d_data->imageDimensions[1], d_data->imageDimensions[2]);
}
