import QtQuick 2.0
import "qrc:/dapqml-widgets"

Item {
    id: root
    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "NodeMaxPrice"
    property string titleString: qsTr("Max amount to pay")
    property bool buttonActive: true

    function setAmount(amount)
    {
        tokenAmount.text = amount
    }

    function setSetConfirmButtonActive(active)
    {
        confirm.enabled = active
    }

    signal textChanged(text: string)
    signal buttonClicked()

    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: titleString
        qss: "dialog-title"
    }

    /****************************************//**
     * Input line
     ********************************************/

    DapQmlRectangle {
        id: countryFilterField
        x: 25
        y: title.y + title.height * 2
//        height: 32
//        width: parent.width - 70
        qss: "priceCount-amount"

        DapQmlLineEdit2 {
            id: countryFilterLine
            objectName: "countryFilterLine"
            x: 2
            y: 2
            z: 15
            height: parent.height - 4
            width: parent.width - 20
            focus: true

            mainText: ""
            mainQss: "priceCount-text"
            onTextChanged: {
                root.textChanged(mainText)
            }
            onTextEdited: {
                root.textChanged(mainText)
            }
        }
    }

    /****************************************//**
     * Token amount
     ********************************************/

    DapQmlLabel {
        id: tokenAmount
        qss: "priceCount-total"
        text: "0.0000"
        color: "#A4A3C0"
        horizontalAlign: Text.AlignHCenter
    }


    /****************************************//**
     * confirm button
     ********************************************/

    DapQmlPushButton {
        id: confirm
        x: (parent.width - width) / 2
        z: 15
        qss: "priceCount-btn-confirm push-button"
        text: qsTr("CONFIRM") + lang.notifier
        onClicked: root.buttonClicked()
        enabled: false
    }

}
