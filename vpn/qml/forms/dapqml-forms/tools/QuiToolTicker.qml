/* INCLUDES */

import QtQuick 2.15
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Qui Tool Ticker
 * @date 26.05.23
 * @author Mikhail Shilenko
 *******************************************/

DapQmlRectangle {
    id: root
    y: -1 * height
    qss: "ticker"
    visible: false

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    property string tickerMessage
    property string tickerUrl
    property bool tickerIsHidden: true

    Behavior on y { PropertyAnimation { duration: 100 }}

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function showTicker() {
        y = 0;
        tickerIsHidden = false;
    }

    function hideTicker() {
        y = -1 * height;
        tickerIsHidden = true;
    }

    function tickerClicked() {
        if (!tickerIsHidden)
            Qt.openUrlExternally (tickerUrl);
    }

    function _updateTickerAnim() {
        tickerAnimation.from    = tickerLableRect.width;
        tickerAnimation.to      = 0 - tickerLabel.contentWidth;
        tickerAnimation.running = true;
    }
    /// @}
    /****************************************//**
     * @name CONTENT
     ********************************************/

    /* Rectangle and text */

    DapQmlRectangle {
        id: tickerLableRect
        objectName: "tickerLableRect"
        qss: "ticker-lable-rect"
        visible: true
        anchors.left: parent.left

        /* text */
        DapQmlLabel {
            id: tickerLabel
            objectName: "tickerLabel"
            width: contentWidth
            qss: "ticker-label"
            text: tickerMessage
            z: 2
            horizontalAlign: Text.AlignHCenter
            mipmap: false

            onWidthChanged: _updateTickerAnim()

            NumberAnimation  {
                id: tickerAnimation
                objectName: "tickerAnimation"
                target: tickerLabel
                properties: "x"
                running: false
                duration: 10000
                loops: Animation.Infinite
            }
        }

        MouseArea {
            anchors.fill: tickerLableRect
            z : 3
            cursorShape: Qt.PointingHandCursor
            onClicked: tickerClicked()
        }

        DapQmlRectangle {
            id: tickerLabelBackgraund
            qss: "ticker-label-background"
            anchors.fill: parent
        }
    }

    /* Close button */

    DapQmlRectangle {
        id: tickerCloseRect
        qss: "ticker-close-rect"
        visible: true
        anchors.right: parent.right

        DapQmlPushButton {
            id: tickerCloseButton
            qss: "ticker-close-button"
            x: parent.width - width - y
            y: (parent.height - height) / 2
            z: 14

            onClicked: hideTicker()
        }

        DapQmlRectangle {
            id: tickerCloseBackground
            qss: "ticker-label-background"
            anchors.fill: parent
        }
    }
}

/*-----------------------------------------*/
