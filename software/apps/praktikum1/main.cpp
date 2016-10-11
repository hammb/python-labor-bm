#include <QGuiApplication>
#include <QtQuick>

#include "imagereslicepipeline.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ImageReslicePipeline>("pipeline", 1, 0, "ImageReslicePipeline");

    QQmlApplicationEngine engine;
    engine.addPluginPath("../../plugin");
    engine.addImportPath("../../plugin");
    engine.load(QUrl("qrc:/main.qml"));

    return app.exec();
}
