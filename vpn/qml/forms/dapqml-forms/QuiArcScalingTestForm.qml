/* INCLUDES */

import QtQuick 2.4
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.5
import com.DapQmlWidgets 1.0

/****************************************//**
 * @brief Arc Animation Scaling Test Form
 * @ingroup groupDapQmlForms
 *
 * Used to test Arc Widget Scaling
 *
 * @date 05.02.24
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root
    clip: true

    property real scaleValue: 1
    property real strokeValue: 5

    ColumnLayout {
        anchors.fill: parent

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            DapQmlArcAnimation {
                anchors.centerIn: parent
                width: scaledSize
                height: scaledSize
                strokeWidth: root.strokeValue
                qss: "c-dashboard-accent"

                property real scaledSize: parent.width / 2 * root.scaleValue
            }
        }

        /* stroke */

        Text {
            Layout.fillWidth: true
            Layout.preferredHeight: 20
            font.pixelSize: 20
            color: "gray"
            text: `Stroke: ${root.strokeValue}`
        }

        Slider {
            Layout.fillWidth: true
            Layout.preferredHeight: 32
            from: 1
            to: 25
            value: 5

            onValueChanged: root.strokeValue = value
        }

        /* scale */

        Text {
            Layout.fillWidth: true
            Layout.preferredHeight: 20
            font.pixelSize: 20
            color: "gray"
            text: `Scale: ${root.scaleValue}`
        }

        Slider {
            Layout.fillWidth: true
            Layout.preferredHeight: 32
            from: 0.1
            to: 10
            value: 1

            onValueChanged: root.scaleValue = value
        }
    }
}
