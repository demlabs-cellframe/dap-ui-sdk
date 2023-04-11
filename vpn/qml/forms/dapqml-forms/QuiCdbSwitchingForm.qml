/* INCLUDES */

import QtQuick 2.0
import QtQuick.Controls 2.12
import PageCtl 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Certificate Form
 * @ingroup groupDapQmlForms
 *
 * @date 11.22
 * @author Stanislav
 *******************************************/


Item {
    id: root
    clip: true

    /// @}
    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "CdbSwitching"

    signal sigShowCdbManager();


    QuiSplashScreenForm  {
        id: splash
        z: 100
        anchors.fill: parent
        opacity: 1
        formName: ""
        onSigShowCdbManager: root.sigShowCdbManager()
    }
}
