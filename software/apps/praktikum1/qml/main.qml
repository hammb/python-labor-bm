import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1

import pipeline 1.0
import qvtkquick 1.0

ApplicationWindow {
    id: root
    width: 1000
    height: 1000
    color: "black"
    visible: true

    ImageReslicePipeline {
        id: myDicomPipeline
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        selectFolder: true

        onAccepted: {
            myDicomPipeline.loadDicom(fileDialog.folder.toString().replace( "file:///", "" ));
            renderWindow.update();
            renderWindow.renderer.resetCamera();
        }
    }

    RenderWindow {
        id: renderWindow
        width: parent.width
        height: parent.height
        pipe: myDicomPipeline
        interactor.enabled: true

        Rectangle {
            id: border
            color: "transparent"
            border.width: 1
            border.color: "white"
        }

        MouseArea {
            anchors.fill: parent
            onWheel: {
                myDicomPipeline.sliceNumber = myDicomPipeline.sliceNumber + (wheel.angleDelta.y > 0 ? 1 : -1);
                renderWindow.update();
            }
        }
    }


    Button {
        id: loadButton
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 24
        text: "Load DICOM-Image"
        onClicked: fileDialog.open()
    }
}
