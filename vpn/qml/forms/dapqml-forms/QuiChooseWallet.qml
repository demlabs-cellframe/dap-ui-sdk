import QtQuick 2.0
import PageCtl 1.0
import DapQmlNodeDataModel 1.0

Item {

    id: root

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "ChooseWallet"

    /// @brief item clicked
    signal sigSelect(name: string);

    Timer {
        id: backTimer
        interval: 100
        running: false
        repeat: false
        onTriggered: PageCtl.slotBackwardAuto()
    }

    property DapQmlNodeDataModel walletModel: DapQmlNodeDataModel {
      id: dataModel
    }

    QuiChooseNodeDataForm {
       dataModel: dataModel
       title:  qsTr("Choose wallets")
       onSigSelect: {
           root.sigSelect(name);
           backTimer.start();
       }
    }

}
