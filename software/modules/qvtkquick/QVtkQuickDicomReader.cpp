///\file QVtkQuickDicomReader.h
///\brief QVtkQuickDicomReader - Simple vtk dicom reader 
///  
///QML wrapper for the standard vtk dicom reader
///
///\par Projekt : VTK/QML Interaction
///
///\par Company:   Mannheim University of Applied Sciences\n
///                emb-lab\n
///                Adress:     Paul-Wittsack-Strasse 10, D-68163 Mannheim, Germany\n
///                eMail:      \send zimmermann.emb@googlemail.com\n
///
///\author         Timo Zimmermann

#include "QVtkQuickDicomReader.h"

#include <QtConcurrent>

#include "vtkSmartPointer.h"
#include "vtkDICOMImageReader.h"
#include "vtkAlgorithm.h"
#include "vtkCommand.h"

#include "QVtkQuickImageData.h"
#include <memory>

class QVtkQuickDicomReader::PrivateData
{
public:
    QString directoryName;
    QString fileName;
    QVtkQuickDicomReader::Status status;
    double progress;
    vtkSmartPointer<vtkDICOMImageReader> dicomReader;
    QFutureWatcher<void> dicomReaderWatcher;
    std::unique_ptr<QVtkQuickImageData> dicomImageData;

    PrivateData()
        :directoryName()
        ,fileName()
        ,status(StatusReady)
        ,progress(0.0)
        ,dicomReader(vtkSmartPointer<vtkDICOMImageReader>::New())
        ,dicomImageData(new QVtkQuickImageData(nullptr))
    {}
};



QVtkQuickDicomReader::QVtkQuickDicomReader(QQuickItem* parent) : QQuickItem(parent)
    ,d_data(new PrivateData())
{
    QObject::connect(&d_data->dicomReaderWatcher, &QFutureWatcher<void>::started, [this](){ 
        setStatus(StatusLoading); 
    });
    QObject::connect(&d_data->dicomReaderWatcher, &QFutureWatcher<void>::finished, [this](){
        setImageData(new QVtkQuickImageData(d_data->dicomReader->GetOutput()));
        setStatus(StatusFinished);
    });

    d_data->dicomReader->AddObserver(vtkCommand::ProgressEvent, this, &QVtkQuickDicomReader::updateProgressEvent);
    d_data->dicomReader->AddObserver(vtkCommand::ErrorEvent, this, &QVtkQuickDicomReader::updateErrorEvent);
}

QVtkQuickDicomReader::~QVtkQuickDicomReader()
{

}

void QVtkQuickDicomReader::updateProgressEvent()
{
    setProgress(d_data->dicomReader->GetProgress());
}

void QVtkQuickDicomReader::updateErrorEvent(vtkObject *caller, unsigned long eventId, void* callData)
{
    Q_UNUSED(caller)
    Q_UNUSED(eventId)
    qWarning() << "QVtkQuickDicomReader | Error event |" << reinterpret_cast<const char*>(callData);
}

void QVtkQuickDicomReader::update()
{
    if(status() == StatusLoading) return;

    if(!d_data->fileName.isEmpty()) d_data->dicomReader->SetFileName(qPrintable(d_data->fileName));
    if(!d_data->directoryName.isEmpty()) d_data->dicomReader->SetDirectoryName(qPrintable(d_data->directoryName));

    setImageData(new QVtkQuickImageData(nullptr));
    setProgress(0.);
    d_data->dicomReaderWatcher.setFuture(QtConcurrent::run(static_cast<vtkAlgorithm*>(d_data->dicomReader.GetPointer()), &vtkAlgorithm::Update));
}

void QVtkQuickDicomReader::setDirectoryName(const QString& directoryName)
{
    if(d_data->directoryName == directoryName) return;

    d_data->directoryName = directoryName;
    emit directoryNameChanged();
}

QString QVtkQuickDicomReader::directoryName() const
{
    return d_data->directoryName;
}

void QVtkQuickDicomReader::setFileName(const QString& fileName)
{
    if(d_data->fileName == fileName) return;

    d_data->fileName = fileName;
    emit fileNameChanged();
}

QString QVtkQuickDicomReader::fileName() const
{
    return d_data->fileName;
}

void QVtkQuickDicomReader::setStatus(QVtkQuickDicomReader::Status status)
{
    if(d_data->status == status) return;

    d_data->status = status;
    emit statusChanged();
}

QVtkQuickDicomReader::Status QVtkQuickDicomReader::status() const
{
    return d_data->status;
}

void QVtkQuickDicomReader::setProgress(double progress)
{
    if(qFuzzyCompare(d_data->progress,progress)) return;

    d_data->progress = progress;
    emit progressChanged();
}

double QVtkQuickDicomReader::progress() const
{
    return d_data->progress;
}

void QVtkQuickDicomReader::setImageData(QVtkQuickImageData* imageData)
{
    if(d_data->dicomImageData.get() == imageData) return;

    d_data->dicomImageData.reset(imageData);
    emit imageDataChanged();
}

QVtkQuickImageData* QVtkQuickDicomReader::imageData() const
{
    return d_data->dicomImageData.get();
}
