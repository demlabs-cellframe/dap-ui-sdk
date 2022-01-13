import QtQuick 2.12
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import DapQmlStyle 1.0

Rectangle {
    id: root
    width: 354
    height: 4
    color: "transparent"
    property string qss

    Image {
        x: 0
        width: 8
        height: 4

        mipmap: true
        source: "qrc:/light/separator_left.png"
    }

    Image {
        x: 8
        width: root.width - 16
        height: 4

        mipmap: true
        source: "qrc:/light/separator_mid.png"
    }

    Image {
        x: root.width - width
        width: 8
        height: 4

        mipmap: true
        source: "qrc:/light/separator_right.png"
    }

    DapQmlStyle { id: style; qss: root.qss; item: root }
}
