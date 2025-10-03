import QtQuick 2.11
import QtQuick.Shapes 1.4
import QtQuick.Layouts 1.4
import Qt5Compat.GraphicalEffects
import StyleDebugTree 1.0
import "qrc:/dapqml-widgets"

Item {
    id: root
    clip: true

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: ""
    property bool link: false
    property bool cellEnabled: false
    property bool buttonActive: false
    property int    progressVolume: 0
    property string progressString: "100%"
    property string progressInfo: "0/0"
    property string buttonText: "CLOSE"
    property string message: "Update completed successfully!"
    property bool messageVisible: false

    signal buttonClicked()

    function setSetConfirmButtonActive(active)
    {
        btnConfirn.enabled = active
    }

    /****************************************//**
     * Progress circle
     ********************************************/

    DapQmlRectangle {
        x: progressCircle.x
        y: progressCircle.y
        width: progressCircle.width
        height: progressCircle.height

        Shape {
            id: outerLine
            anchors.fill: parent
            visible: false
            // multisample, decide based on your scene settings
            layer.enabled: true
            layer.samples: 4

            ShapePath {
                fillColor: "transparent"
                //strokeColor: progressCircle.color
                strokeColor: "white"//"#A4A3C0"
                strokeWidth: progressCircle.strokeWidth
                capStyle: ShapePath.FlatCap

                PathAngleArc {
                    centerX: progressCircle.width/2
                    centerY: progressCircle.height/2
                    radiusX: progressCircle.width/2 - progressCircle.strokeWidth
                    radiusY: progressCircle.height/2 - progressCircle.strokeWidth
                    startAngle: 0
                    sweepAngle: 360
                }
            }
        }

        InnerShadow {
            anchors.fill: outerLine
            radius: 10
            samples: 16
            horizontalOffset: 3
            verticalOffset: 3
            color: "black"
            opacity: 0.2
            source: outerLine
        }

        Shape {
            id: innerLine
            anchors.fill: parent
            // multisample, decide based on your scene settings
            layer.enabled: true
            layer.samples: 4

            ShapePath {
                fillColor: "transparent"
                //strokeColor: progressCircle.color
                strokeColor: "#E60778"
                strokeWidth: progressCircle.strokeWidth
                capStyle: ShapePath.RoundCap

                PathAngleArc {
                    centerX: progressCircle.width/2
                    centerY: progressCircle.height/2
                    radiusX: progressCircle.width/2 - progressCircle.strokeWidth
                    radiusY: progressCircle.height/2 - progressCircle.strokeWidth
                    startAngle: 0 - 90
                    sweepAngle: root.progressVolume * 360 / 100
                }
            }
        }

        DapQmlRectangle {
            id: progressInfo
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            qss: "progress-circle-info"

            DapQmlDummy {
                id: progressCircleSpacing
                qss: "progress-circle-spacing"
                onHeightChanged: progressInfoColumnLayout.spacing = height
            }

            ColumnLayout {
                id: progressInfoColumnLayout
                anchors.fill: parent
                spacing: progressCircleSpacing.height

                Component.onCompleted:
                    StyleDebugTree.describe (
                       "progress-circle-ColumnLayout",
                        ["x", "y", "width", "height", "spacing"],
                       this);

                DapQmlLabel {
                    id: progressInfoPercent
                    Layout.fillWidth: true
                    Layout.preferredHeight: contentHeight

                    qss: "progress-circle-percent"
                    text: root.progressString
                }

                DapQmlLabel {
                    id: progressInfoMbytes
                    Layout.fillWidth: true
                    Layout.preferredHeight: contentHeight
                    qss: "progress-circle-mbytes"
                    text: root.progressInfo
                }
            }
        }

        DapQmlDummy {
            id: progressCircle
            qss: "progress-circle-arc"
            property string color: "red"
            property int strokeWidth: 8
        }
    }

    /****************************************//**
     * message
     ********************************************/

    DapQmlLabel {
        qss: "progress-message"
        visible: root.messageVisible
        text: root.message
        clip: false
    }

    /****************************************//**
     * Connect button
     ********************************************/

    DapQmlPushButton {
        id: btnConfirn
        x: (parent.width - width) / 2
        z: 15
        qss: "progress-circle-btn-confirm push-button"
        text: root.buttonText
        onClicked: root.buttonClicked()
        enabled: buttonActive
    }

}
