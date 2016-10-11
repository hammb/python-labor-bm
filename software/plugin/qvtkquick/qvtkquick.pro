QT += qml quick concurrent

TEMPLATE = lib
TARGET = $$basename(_PRO_FILE_PWD_)plugin                   #convention: qml plugin target names ends always with 'plugin'

#include associated standard module - comment out if not needed
include($$modules_dir/$$basename(_PRO_FILE_PWD_)/$$basename(_PRO_FILE_PWD_)'.'pri)

#include standard plugin files - add additional files if needed
SOURCES += $$files($$PWD/$$basename(_PRO_FILE_PWD_)plugin.cpp)
HEADERS += $$files($$PWD/$$basename(_PRO_FILE_PWD_)plugin.h)
OTHER_FILES += qmldir

#include VTK
CONFIG += vtk libs

#configure as qt plugin
CONFIG += qt plugin

#add c++11 support
CONFIG += c++11

pluginPath = $$replace(_PRO_FILE_PWD_, $$plugin_dir/, )     #the plugin path is relative to the standard 'plugin' source path
uri = $$replace(pluginPath, /, .)                           #the uri notation has '.' instead of '/'
installPath = $$[QT_INSTALL_QML]/$$pluginPath               #standard plugin install path

#copy the dynamic lib into the source dir, that the QtCreator can resolve the uri
DLLDESTDIR = $$_PRO_FILE_PWD_
