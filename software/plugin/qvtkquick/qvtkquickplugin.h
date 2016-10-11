#ifndef Q_VTK_QUICK_PLUGIN_H
#define Q_VTK_QUICK_PLUGIN_H

#include <QtQuick>

class QVtkQuickPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void initializeEngine(QQmlEngine * engine, const char * uri);
    void registerTypes(const char *uri);
};

#endif
