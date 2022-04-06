import QtQuick 2.0
import DapQmlStyle 1.0

Rectangle {
    id: root
    color: "transparent"

    /* vars */
    property string qss
    property string borderColor
    property int borderWidth

    /* links */
    border.color: borderColor
    border.width: borderWidth

    /* style */
    DapQmlStyle { id: style; qss: root.qss; item: root }
}
