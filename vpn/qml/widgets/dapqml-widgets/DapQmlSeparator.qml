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
        id: sep1
        x: 0
        width: 8
        height: 4

        mipmap: true
        //source: "qrc:/light/separator_left.png"
        DapQmlStyle { item: sep1; qss: "sep-left"; }
    }

    Image {
        id: sep2
        x: 8
        width: root.width - 16
        height: 4

        mipmap: true
        //source: "qrc:/light/separator_mid.png"
        DapQmlStyle { item: sep2; qss: "sep-mid"; }
    }

    Image {
        id: sep3
        x: root.width - width
        width: 8
        height: 4

        mipmap: true
        //source: "qrc:/light/separator_right.png"
        DapQmlStyle { item: sep3; qss: "sep-right"; }
    }

    DapQmlStyle { id: style; qss: root.qss; item: root }
}
