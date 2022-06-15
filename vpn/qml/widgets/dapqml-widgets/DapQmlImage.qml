/* INCLUDES */

import QtQuick 2.12
import Scaling 1.0
import DapQmlImageItem 1.0

Item {
    id: root
    clip: true

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    property string scaledPixmap: ""
    property bool mipmap: true
    property bool smooth: true
    property bool antialiasing: false

    /// @}
    /****************************************//**
     * Scaled Image
     ********************************************/

    DapQmlImageItem {
        id: scaledImage
        visible: Scaling.isDesktop()
        anchors.fill: root
        scaledPixmap: (Scaling.isDesktop()) ? root.scaledPixmap : ""
    }

    /****************************************//**
     * Image
     ********************************************/
    Image {
        id: regularImage
        visible: Scaling.isAndroid()
        anchors.fill: root
        //mipmap: root.mipmap
        //smooth: root.smooth
        //antialiasing: root.antialiasing
        source: (Scaling.isAndroid()) ? root.scaledPixmap : ""
    }
}

/*-----------------------------------------*/
