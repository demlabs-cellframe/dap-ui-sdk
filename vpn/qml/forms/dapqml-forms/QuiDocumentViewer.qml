import QtQuick 2.0
import QtQuick.Controls 2.12
import "qrc:/dapqml-widgets"
import DapQmlStyle 1.0

Item {
    id: root
    property string formName: "DocumentViewer"

    /* functions */
    function show(a_docName, a_docContent) {
        title.text      = a_docName;
        content.text    = a_docContent;
    }

    /* title */
    DapQmlDialogTitle {
        id: title
        text: "Dialog title"
        qss: "dialog-title"
    }

    /* input scrollarea */
    ScrollView
    {
        id: scroll
        clip: true
        height: root.height - y
        contentWidth: width
        property int fontSize: 12
        DapQmlStyle { item: scroll; qss: "dv-content"; }

        //Component.onCompleted: scroll.setHorizontalScrollBarEnabled (false);

        Text {
            id: content
            width: scroll.width
            font.pixelSize: scroll.fontSize
            textFormat: Text.RichText
            wrapMode: Text.Wrap

            DapQmlStyle { item: content; qss: "dv-text"; }
        }
    }
}
