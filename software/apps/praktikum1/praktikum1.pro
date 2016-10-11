#copy the executable to the source folder
DLLDESTDIR = $$_PRO_FILE_PWD_

#Include sub modules here
#include($$modules_dir/qvtkquick/qvtkquick.pri)

#Include project files
INCLUDEPATH += $$PWD

SOURCES += main.cpp \
    imagereslicepipeline.cpp

HEADERS += \
    imagereslicepipeline.h

RESOURCES += qml/qml.qrc

OTHER_FILES +=

#additional Qt modules
QT += quick qml concurrent

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH += $$qml_dir
QML_IMPORT_PATH += $$plugin_dir

#use c++11 compiler flags
CONFIG += c++11 

#include vtk libraries
CONFIG += vtk libs

#Android specific variables - will be ignored in other configuration
ANDROID_PACKAGE_SOURCE_DIR =
ANDROID_EXTRA_LIBS =
OTHER_FILES += $$files($$PWD/android/*)
