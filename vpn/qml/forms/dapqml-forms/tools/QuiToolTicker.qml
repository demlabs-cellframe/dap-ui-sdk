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

    property bool tickerVisible: TickerUpdateCtl.tickerVisible

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    Behavior on y { PropertyAnimation { duration: 100 }}

    onTickerVisibleChanged: {
        if (tickerVisible)
            showTicker();
    }

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function showTicker() {
        y = 0;
        visible = true;
    }

    function hideTicker() {
        y = -1 * height;
        TickerUpdateCtl.tickerVisible = false;
    }

    function tickerClicked() {
        if (TickerUpdateCtl.tickerVisible)
            Qt.openUrlExternally (TickerUpdateCtl.tickerUrl);
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
        qss: "ticker-lable-rect"
        visible: true
        anchors.left: parent.left

        /* text */
        DapQmlLabel {
            id: tickerLabel
            width: contentWidth
            qss: "ticker-label"
            text: TickerUpdateCtl.tickerMessage
            z: 2
            horizontalAlign: Text.AlignHCenter
            mipmap: false

            onWidthChanged: _updateTickerAnim()

            NumberAnimation  {
                id: tickerAnimation
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
