///\file QVtkQuickimageData.h
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

#ifndef Q_VTK_QUICK_IMAGE_DATA_H
#define Q_VTK_QUICK_IMAGE_DATA_H

#include <QtQuick>

class vtkImageData;

class QVtkQuickImageData : public QQuickItem
{
public:
    enum ImageAxis {
        ImageAxisX = 0,
        ImageAxisY,
        ImageAxisZ,
        ImageAxisCount
    };

private:
    Q_OBJECT
    Q_ENUMS(ImageAxis)
    Q_PROPERTY(QVector3D imageSpacing READ imageSpacing NOTIFY imageChanged)
    Q_PROPERTY(QVector3D imageCenter READ imageCenter NOTIFY imageChanged)
    Q_PROPERTY(QVector3D imageOrigin READ imageOrigin NOTIFY imageChanged)
    Q_PROPERTY(QVector3D imageDimension READ imageDimension NOTIFY imageChanged)
    
    class PrivateData;
    QScopedPointer<PrivateData> d_data;

public:
    QVtkQuickImageData(vtkImageData* imageData = nullptr, QQuickItem* parent = nullptr);
    ~QVtkQuickImageData();

    void setImageData(vtkImageData* imageData);
    vtkImageData* imageData() const;

    Q_INVOKABLE qreal getImageSpacing(int imageAxis) const;
    Q_INVOKABLE qreal getImageCenter(int imageAxis) const;
    Q_INVOKABLE qreal getImageOrigin(int imageAxis) const;
    Q_INVOKABLE int getImageDimension(int imageAxis) const;

    QVector3D imageSpacing() const;
    QVector3D imageCenter() const;
    QVector3D imageOrigin() const;
    QVector3D imageDimension() const;

signals:
    void imageChanged();
};

QML_DECLARE_TYPE(QVtkQuickImageData)

#endif
