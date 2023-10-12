/* INCLUDES */
import QtQuick 2.11
import QtQml 2.12
import QtQuick.Shapes 1.4
import DapQmlStyle 1.0

/****************************************//**
 * @brief Dap QML Arc Animation Widget
 * @ingroup groupDapQmlWidgets
 *
 * ### Structure
 *
 * - Shape
 * -- SpaePath
 * --- PathAngleArc
 * ---- NumberAnimation
 * - Rectangle 1 + 2
 *
 * @date 07.06.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root

    DapQmlStyle { qss: root.qss; item: root }

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief widget qss style
    property string qss

    /// @brief stroke color
    property string color: "#000000"

    /// @brief stroke width
    property int strokeWidth: 5

    /// @}
    /****************************************//**
     * Content
     ********************************************/

    Shape {
        id: arcAnim
        anchors.fill: parent
        layer.enabled: true
        layer.samples: 6

        ShapePath {
            fillColor: "transparent"
            strokeColor: root.color
            strokeWidth: root.strokeWidth
            capStyle: ShapePath.FlatCap

            PathAngleArc {
                id: arcPath
                centerX: arcAnim.width / 2
                centerY: arcAnim.height / 2
                radiusX: arcAnim.width / 2 - root.strokeWidth / 2
                radiusY: arcAnim.height / 2 - root.strokeWidth / 2
                startAngle: 90
                sweepAngle: 180

                NumberAnimation on startAngle {
                    from: 0
                    to: 360
                    running: true
                    loops: Animation.Infinite
                    duration: 2000
                }

                onStartAngleChanged: {
                    let angle   = startAngle / 180 * Math.PI;
                    let centerX = root.width / 2 - root.strokeWidth / 2;
                    let centerY = root.height / 2 - root.strokeWidth / 2;
                    dot1.x = centerX + arcPath.radiusX * Math.cos(angle);
                    dot1.y = centerY + arcPath.radiusY * Math.sin(angle);
                    dot2.x = centerX + arcPath.radiusX * Math.cos(angle + Math.PI);
                    dot2.y = centerY + arcPath.radiusY * Math.sin(angle + Math.PI);
                }
            } // PathAngleArc
        } // ShapePath
    } // Shape

    Rectangle {
        id: dot1
        width: root.strokeWidth
        height: root.strokeWidth
        radius: width
        color: root.color
    }

    Rectangle {
        id: dot2
        width: root.strokeWidth
        height: root.strokeWidth
        radius: width
        color: root.color
    }
}

/*-----------------------------------------*/
