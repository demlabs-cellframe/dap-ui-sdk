import QtQuick 2.0
import "qrc:/dapqml-widgets"

Item {

    id: root
    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: ""
    property bool link: false
    property bool cellEnabled: false
    property bool buttonActive: false

    signal buttonClicked()

    function setSetConfirmButtonActive(active)
    {
        btnConfirn.enabled = active
    }

    /****************************************//**
     * Top separator
     ********************************************/

    DapQmlRectangle {
        x: loginSepsPlacer.x
        y: loginSepsPlacer.y
        width: loginSepsPlacer.width
        height: loginSepsPlacer.height
        DapQmlSeparator {
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74
            qss: "login-separator"
        }
        DapQmlDummy {
            id: loginSepsPlacer
            qss: "overview-separator-container"
        }
    }

    /****************************************//**
     * Connect button
     ********************************************/

    DapQmlPushButton {
        id: btnConfirn
        x: (parent.width - width) / 2
        z: 15
        qss: "overview-btn-confirm"
        text: qsTr("CONFIRM") + lang.notifier
        onClicked: root.buttonClicked()
        enabled: buttonActive
    }

}
