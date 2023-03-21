import QtQuick 2.0
import PageCtl 1.0
import DapQmlNodeDataModel 1.0

Item {

    id: root

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "ChooseOrder"

    /// @brief item clicked
    signal sigSelect(name: string);

    Timer {
        id: backTimer
        interval: 30
        running: false
        repeat: false
        onTriggered: PageCtl.slotForward (1, "qrc:/dapqml-forms/QuiLoginForm.qml");
    }

    property DapQmlNodeDataModel orderModel: DapQmlNodeDataModel {
      id: dataModel
    }

    QuiChooseNodeDataForm {
       dataModel: dataModel
       viewType: "orderView"
       title:  qsTr("Choose order")
       onSigSelect: {
           //PageCtl.slotBackwardAuto();
           root.sigSelect(name);
           backTimer.start();
       }
    }

}
