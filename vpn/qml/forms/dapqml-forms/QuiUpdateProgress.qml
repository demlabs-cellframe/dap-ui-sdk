import QtQuick 2.0
import DapQmlStyle 1.0
import "qrc:/dapqml-widgets"

Item {

    id: root

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "UpdateProgress"
    property string titleString: qsTr("Updating")
    property bool buttonActive: true

    /// @brief item clicked
    signal buttonClicked()

    function setProgressString(progress)
    {
        overview.progressString = progress
    }

    function setProgressVolume(progress)
    {
        overview.progressVolume = progress
    }

    function setProgressInfo(infoRow)
    {
        overview.progressInfo = infoRow
    }

    function setSetConfirmButtonActive(active)
    {
        overview.buttonActive = active
    }

    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: root.titleString
        qss: "dialog-title"
    }

    QuiCircleProgressBase {
        id: overview
        // form name
        formName: root.formName
        // buttons clicked
        onButtonClicked: root.buttonClicked()
        // button enable
        buttonActive: false//root.buttonActive
        // arrows on buttons
        link: true
    }

}
