#copy the executable to the source folder
DLLDESTDIR = $$_PRO_FILE_PWD_

#Include modules here that will be tested
includeAllFiles($$apps_dir/exampleqtapplication)

#remove main files from other projects
SOURCES ~= s/.*main.cpp/

#Include project files
INCLUDEPATH += $$PWD

SOURCES += \
        main.cpp

HEADERS += \


RESOURCES += \
        res.qrc


#additional Qt modules
QT += quick qml

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH += $$qml_dir
QML_IMPORT_PATH += $$plugin_dir

#use c++11 compiler flags
CONFIG += c++11 

#test lib flags
CONFIG += qmltestcase
QT += testlib
