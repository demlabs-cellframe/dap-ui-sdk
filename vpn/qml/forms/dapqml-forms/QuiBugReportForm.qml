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

    /* resizer */
    DapQmlRectangle {
        id: resizer
        qss: "bugrep-input-content"
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

            property real yy: (input.height - resizer.height) / 2

            DapQmlStyle { id: style; qss: "bugrep-input"; item: input }

            /* background image */
            DapQmlLabel {
                id: inputbg
                z: 1
                anchors.fill: input
                qss: "bugrep-bg"

                /* placeholder */
                DapQmlLabel {
                    id: placeholder
                    x: (input.width - width) / 2
                    y: input.yy
                    z: 2
                    width: resizer.width
                    height: resizer.height
                    horizontalAlign: Text.AlignLeft
                    verticalAlign: Text.AlignTop
                    text: "Please describe the details of problem you faced. What actions did you take and what happened."
                    qss: "bugrep-input-placeholder"
                    wrapMode: TextEdit.Wrap
                    visible: bugRepInputField.text.length == 0

//                    Component.onCompleted: StyleDebugTree.describe (
//                       "placeholder",
//                        ["x", "y", "width", "height"],
//                       this);
                }
            }

            /* input scrollarea */
            Flickable {
                id: bugRepInput
                x: (input.width - resizer.width) / 2
                y: input.yy
                z: 3
                clip: true
                contentWidth: width
                contentHeight: calcContentHeight()

                DapQmlStyle { item: bugRepInput; qss: "bugrep-input-content"; }

//                Component.onCompleted: StyleDebugTree.describe (
//                   "Flickable",
//                    ["x", "y", "width", "height"],
//                   this);

                function ensureVisible(r) {
                    if (contentX >= r.x)
                        contentX = r.x;
                    else if (contentX+width <= r.x+r.width)
                        contentX = r.x+r.width-width;
                    if (contentY >= r.y)
                        contentY = r.y;
                    else if (contentY+height <= r.y+r.height)
                        contentY = r.y+r.height-height;
                }

                function calcContentHeight() {
                    if (bugRepInputField.paintedHeight < inputbg.height)
                        return inputbg.height;
                    else
                        return bugRepInputField.paintedHeight;
                }

                /* input */
                TextEdit {
                    id: bugRepInputField
                    z: 4
                    objectName: "bugRepInputField"
                    anchors.fill: parent
                    wrapMode: TextEdit.Wrap
                    clip: true
                    font.pixelSize: fontSize
                    font.weight: fontWeight

                    property int fontSize: 16
                    property int fontWeight: Font.Normal
                    property int maximumLength: 200
                    property string previousText: text

                    DapQmlStyle { item: bugRepInputField; qss: "bugrep-input-textarea"; }

                    onCursorRectangleChanged: bugRepInput.ensureVisible(cursorRectangle)

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
            visible: false

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
            onClicked: { root.mode = 0; root.sigCancel(); }
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
