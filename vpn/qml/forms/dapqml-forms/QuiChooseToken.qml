import QtQuick 2.0
import DapQmlNodeDataModel 1.0

Item {

    id: root

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "ChooseToken"

    /// @brief item clicked
    signal sigSelect(int index, string name);

    property DapQmlNodeDataModel tokenModel: DapQmlNodeDataModel {
      id: dataModel
    }

    QuiChooseNodeDataForm {
       dataModel: dataModel
       title:  qsTr("Choose token")
       onSigSelect: root.sigSelect(index,name);
    }

}
