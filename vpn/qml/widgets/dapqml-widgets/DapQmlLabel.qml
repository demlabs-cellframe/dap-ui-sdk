/* INCLUDES */

import QtQuick 2.12
import DapQmlStyle 1.0
import Brand 1.0

/****************************************//**
 * @brief Dap QML Label Widget
 * @ingroup groupDapQmlWidgets
 * @note most of properties is related to text label item
 *
 * ### Structure
 *
 * Widget with text label or image
 *
 * @date 06.06.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root
    clip: true

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    property string qss
    property string text: ""
    property color color
    property string scaledPixmap: ""
    property var fontFamiliy: Brand.fontName()
    property int fontSize: 12
    property int fontWeight: Font.Normal
    property int horizontalAlign: Text.AlignHCenter
    property int verticalAlign: Text.AlignVCenter
    property int wrapMode: Text.NoWrap
    property int textFormat: Text.AutoText
    property bool mipmap: true
    property bool smooth: true
    property bool antialiasing: false

    property real bottomPadding
    property real leftPadding
    property real padding
    property real rightPadding
    property real topPadding
    property real lineHeight: 1

    property real contentWidth: label.contentWidth
    property real contentHeight: label.contentHeight

    property Text label: label

    DapQmlStyle { id: style; qss: root.qss; item: root }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief label clicked
    signal clicked();

    /// @}
    /****************************************//**
     * Image
     ********************************************/

    DapQmlImage {
        id: icon
        anchors.fill: root
        //source: root.scaledPixmap
        //mipmap: root.mipmap
        //smooth: root.smooth
        //antialiasing: root.antialiasing
        scaledPixmap: root.scaledPixmap
    }

    /****************************************//**
     * Text label
     ********************************************/

    Text {
        id: label
        anchors.fill: root

        bottomPadding: root.bottomPadding
        leftPadding: root.leftPadding
        padding: root.padding
        rightPadding: root.rightPadding
        topPadding: root.topPadding

        horizontalAlignment: root.horizontalAlign
        verticalAlignment: root.verticalAlign

        text: root.text
        color: root.color
        wrapMode: root.wrapMode
        textFormat: root.textFormat
        lineHeight: root.lineHeight

        font {
            family: root.fontFamiliy
            pixelSize: root.fontSize
            weight: root.fontWeight
        }
    }

    /****************************************//**
     * Mouse area
     ********************************************/

    MouseArea {
        anchors.fill: root
        onClicked: root.clicked()
    }
}

/*-----------------------------------------*/
