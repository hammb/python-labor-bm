#include "qvtkquickplugin.h"

#include "QVtkQuickDicomReader.h"
#include "QVtkQuickImageData.h"
#include "QVtkQuickImageViewerItem.h"
#include "QVtkQuickRenderWindowItem.h"
#include "QVtkQuickInteractor.h"
#include "QVtkQuickRenderer.h"
#include "QVtkQuickCamera.h"

void QVtkQuickPlugin::initializeEngine(QQmlEngine * engine, const char * uri)
{
    Q_UNUSED(uri)
    Q_UNUSED(engine)

}

void QVtkQuickPlugin::registerTypes(const char *uri)
{
    Q_UNUSED(uri)

    qmlRegisterType<QVtkQuickImageViewerItem>("qvtkquick", 1, 0, "ImageViewer");
    qmlRegisterType<QVtkQuickRenderWindowItem>("qvtkquick", 1, 0, "RenderWindow");
    qmlRegisterType<QVtkQuickDicomReader>("qvtkquick", 1, 0, "DicomReader");
    qmlRegisterUncreatableType<QVtkQuickImageData>("qvtkquick", 1, 0, "ImageData", "Internal type");
    qmlRegisterUncreatableType<QVtkQuickInteractor>("qvtkquick", 1, 0, "Interactor", "Internal type");
    qmlRegisterUncreatableType<QVtkQuickRenderer>("qvtkquick", 1, 0, "Renderer", "Internal type");
    qmlRegisterUncreatableType<QVtkQuickCamera>("qvtkquick", 1, 0, "Camera", "Internal type");
}



