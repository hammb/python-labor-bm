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

#ifndef Q_VTK_QUICK_DICOM_READER_H
#define Q_VTK_QUICK_DICOM_READER_H

#include <QtQuick>

class QVtkQuickImageData;
class vtkObject;

class QVtkQuickDicomReader : public QQuickItem
{
public:
    enum Status {
        StatusReady,
        StatusLoading,
        StatusFinished
    };

private:
    Q_OBJECT
    Q_ENUMS(Status)
    Q_PROPERTY(QString directoryName READ directoryName WRITE setDirectoryName NOTIFY directoryNameChanged)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QVtkQuickImageData* imageData READ imageData NOTIFY imageDataChanged)

    class PrivateData;
    QScopedPointer<PrivateData> d_data;

    void updateProgressEvent();
    void updateErrorEvent(vtkObject *caller, unsigned long eventId, void *callData);

public:
    QVtkQuickDicomReader(QQuickItem* parent = nullptr);
    ~QVtkQuickDicomReader();

    void setDirectoryName(const QString& directoryName);
    QString directoryName() const;

    void setFileName(const QString& fileName);
    QString fileName() const;

    void setStatus(Status status);
    Status status() const;

    void setProgress(double progress);
    double progress() const;

    void setImageData(QVtkQuickImageData* imageData);
    QVtkQuickImageData* imageData() const;

public slots:
    void update();

signals:
    void directoryNameChanged();
    void fileNameChanged();
    void statusChanged();
    void progressChanged();
    void imageDataChanged();
};

#endif
