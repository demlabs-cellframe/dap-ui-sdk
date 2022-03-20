import QtQuick 2.4
import DapQmlStyle 1.0
import QtQuick.Controls 2.12
import StyleDebugTree 1.0
import "qrc:/dapqml-widgets"

Item {
    id: root
    property int mode: 0
    property string formName: "BugReport"

    /* defs */
    enum Mode
    {
        Write,
        Loading,
        Result
    }

    /* signals */
    signal sigSend()
    signal sigCancel();
    signal sigResultBack()

    /* functions */
    function setmode(a_mode) {
        mode    = a_mode;
        switch(a_mode) {
        case QuiBugReportForm.Mode.Write:
        case QuiBugReportForm.Mode.Loading:
        case QuiBugReportForm.Mode.Result:
        }
    }

    function setResultText(a_text) {
        bugrepResult.text   = a_text;
    }

    /* title */
    DapQmlDialogTitle {
        text: "Bug report"
        qss: "dialog-title"
    }

    /* INPUT */
    Item {
        anchors.fill: parent
        visible: root.mode == 0

        /* text edit */
        Item {
            id: input

            DapQmlStyle { id: style; qss: "bugrep-input"; item: input }

            MouseArea {
                id: mouseAreaInput
                anchors.fill: input
                cursorShape: Qt.IBeamCursor
                enabled: false
                onClicked: bugRepInputField.forceActiveFocus()
            }

            /* background image */
            DapQmlLabel {
                anchors.fill: input
                qss: "bugrep-bg"
                //source: "qrc:/light/report_bg.png"

                Component.onCompleted: StyleDebugTree.describe (
                   "Bug rep image",
                    ["x", "y", "width", "height"],
                   this);

            }

            /* input scrollarea */
            ScrollView
            {
                id: bugRepInput
                clip: true
                contentWidth: bugRepInput.width

                DapQmlStyle { item: bugRepInput; qss: "bugrep-input-content"; }

                Component.onCompleted: StyleDebugTree.describe (
                   "Bug rep scrollview",
                    ["x", "y", "width", "height"],
                   this);

                /* input */
                TextEdit {
                    id: bugRepInputField
                    anchors.fill: parent
                    clip: true
                    property int maximumLength: 200
                    property string previousText: text
                    wrapMode: TextEdit.Wrap

                    onTextChanged: {
                        if (text.length > maximumLength) {
                            var cursor = cursorPosition;
                            text = previousText;
                            if (cursor > text.length) {
                                cursorPosition = text.length;
                            } else {
                                cursorPosition = cursor-1;
                            }
                        }
                        previousText = text

                        letterAmount.text = text.length + "/200"
                    }

                    Component.onCompleted: StyleDebugTree.describe (
                       "Bug rep input",
                        ["x", "y", "width", "height"],
                       this);
                }
            }
        }

        /* letter counter */
        DapQmlLabel {
            id: letterAmount
            qss: "bugrep-letter-counter"
            text: "0/200"
            color: "#A4A3C0"
            horizontalAlign: Text.AlignRight
        }

        /* attach */
        DapQmlLabel {
            id: attach
            qss: "bugrep-attach-btn"
            text: "Click here to attach a screenshot"
            color: "#DA0B82"

            MouseArea {
                id: mouseArea
                anchors.fill: attach
                cursorShape: Qt.PointingHandCursor
                enabled: false
            }
        }

        /* send button */
        DapQmlPushButton {
            qss: "bugrep-send-btn push-button"
            text: "SEND REPORT"
            onClicked: { root.mode = 1; root.sigSend(); }
        }
    }

    /* SENDING */
    Item {
        anchors.fill: parent
        visible: root.mode == 1

        /* info */
        DapQmlLabel {
            qss: "bugrep-sending"
            text: "Sending..."
        }

        /* animated spinner */
        AnimatedImage {
            id: animation
            source: "qrc:/dapqml-forms-asset/Spinner.gif"
            DapQmlStyle { qss: "bugrep-animation"; item: animation }
        }

        /* cancel */
        DapQmlPushButton {
            qss: "bugrep-send-btn push-button"
            text: "CANCEL"
            onClicked: { root.mode = 2; root.sigCancel(); }
        }
    }

    /* STATUS */
    Item {
        anchors.fill: parent
        visible: root.mode == 2

        /* status text */
        DapQmlLabel {
            id: bugrepResult
            qss: "bugrep-status"
            wrapMode: Text.WordWrap
            text: "Bug report # 368647 sent successfully"
        }

        /* back */
        DapQmlPushButton {
            qss: "bugrep-send-btn push-button"
            text: "BACK"
            onClicked: { root.mode = 0; root.sigResultBack(); }
        }
    }
}
