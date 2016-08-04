import QtQuick 2.0
import QtTest 1.0

Rectangle {
    id: rect
    width: 50; height: 50
    focus: true

    MouseArea {

     anchors.fill: rect
     onClicked: {
         console.log( "Simulated click: " + mouse.x + " " + mouse.y )
     }
    }

    TestCase {
        name: "KeyClick"
        when: windowShown

        function test_key_click() {
            mouseClick( rect, 10 , 10 )
       }
    }
}
