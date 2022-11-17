import QtQuick 2.0
import DapQmlNodeDataModel 1.0

Item {

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "NodeTransactionProcessing"

    /// @brief item clicked
    signal sigSelect(int index, string name);

    property DapQmlNodeDataModel walletModel: DapQmlNodeDataModel {
      id: dataModel
    }

    QuiChooseNodeDataForm {
       dataModel: dataModel
       title:  qsTr("Transaction processing")
    }

}
