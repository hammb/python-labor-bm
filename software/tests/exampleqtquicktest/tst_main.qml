import QtQuick 2.2
import QtTest 1.0

Rectangle {
    id: root
    width: 116; height: 100

    TestCase {
        name: "MathTests"

        function test_math() {
            compare(2 + 2, 4, "2 + 2 = 4")
        }

    }

}
