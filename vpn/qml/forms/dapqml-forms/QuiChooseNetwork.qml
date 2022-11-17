import QtQuick 2.0
import DapQmlNodeDataModel 1.0

Item {

    id: root

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "ChooseNetwork"

    /// @brief item clicked
    signal sigSelect(name: string);

    property DapQmlNodeDataModel networkModel: DapQmlNodeDataModel {
      id: dataModel
    }

    QuiChooseNodeDataForm {
       dataModel: dataModel
       title:  qsTr("Choose network")
       onSigSelect: root.sigSelect(name);
    }

}
