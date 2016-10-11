#ifndef IMAGERESLICEPIPELINE_H
#define IMAGERESLICEPIPELINE_H

#include <QObject>

#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
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
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkDICOMImageReader.h"


class ImageReslicePipeline : public QObject
{
    Q_OBJECT
	
    Q_PROPERTY(int sliceNumber READ sliceNumber WRITE setSliceNumber NOTIFY sliceNumberChanged)
	
	// ...

    int m_sliceNumber;
	// ...	

public:
    explicit ImageReslicePipeline(QObject *parent = 0);
    virtual ~ImageReslicePipeline();

	// ...
	
signals:
    void sliceNumberChanged();
	// ...

public slots:
    void setupPipeline(vtkRenderer* renderer);
    void updatePipeline();

};

#endif // IMAGERESLICEPIPELINE_H
