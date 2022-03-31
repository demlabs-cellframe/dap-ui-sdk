import QtQuick 2.0
import DapQmlStyle 1.0

Rectangle {
    id: root
    color: "transparent"
    property string qss
    DapQmlStyle { id: style; qss: root.qss; item: root }
}
