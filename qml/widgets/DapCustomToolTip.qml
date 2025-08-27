import QtQuick
import QtQml
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

ToolTip {
    id: root
    signal updatePos

    property alias bottomRect: bottomRect
    property font textFont: mainFont.dapFont.medium12
    property string textColor: currTheme.white
    property string contentText
    property alias backgroundToolTip: backgroundToolTip
    property double scaleFactor: 1.0
    property int elideMode: Text.ElideRight

    contentItem: Text {
        width: parent.width
        color: textColor
        text: contentText
        font: textFont
        elide: elideMode
    }

    background: Item {
        Rectangle {
            id: backgroundToolTip
            anchors.fill: parent
            radius: 4
            color: currTheme.mainBackground
        }
        DropShadow {
            anchors.fill: backgroundToolTip
            source: backgroundToolTip
            color: currTheme.reflection
            horizontalOffset: -1
            verticalOffset: -1
            radius: 0
            samples: 0
            opacity: 1
            fast: true
            cached: true
        }
        DropShadow {
            anchors.fill: backgroundToolTip
            source: backgroundToolTip
            color: currTheme.shadowColor
            horizontalOffset: 2
            verticalOffset: 2
            radius: 10
            samples: 20
            opacity: 1
        }

        Rectangle {
            id: bottomRect
            anchors.horizontalCenter: backgroundToolTip.horizontalCenter
            color: backgroundToolTip.color

            width: 12 * scaleFactor
            height: 12 * scaleFactor
            rotation: 45
            Connections {
                target: root

                function onUpdatePos() {
                    if (root.y < 0) {
                        bottomRect.anchors.top = backgroundToolTip.bottom;
                        bottomRect.anchors.topMargin = -(bottomRect.width / 2);

                        bottomLineHide.anchors.bottom = backgroundToolTip.bottom;
                        shadow1.visible = true;
                        shadow2.visible = false;
                    } else {
                        bottomRect.anchors.bottom = backgroundToolTip.top;
                        bottomRect.anchors.bottomMargin = -(bottomRect.width / 2);

                        bottomLineHide.anchors.top = backgroundToolTip.top;
                        shadow1.visible = false;
                        shadow2.visible = true;
                    }
                }
            }
        }

        DropShadow {
            id: shadow1
            rotation: 45
            anchors.fill: bottomRect
            source: bottomRect
            color: currTheme.shadowColor
            horizontalOffset: 2
            verticalOffset: 2
            radius: 10
            samples: 20
            opacity: 1
            visible: bottomRect.visible
        }
        DropShadow {
            id: shadow2
            rotation: 45
            anchors.fill: bottomRect
            source: bottomRect
            color: currTheme.reflection
            horizontalOffset: -1
            verticalOffset: -1
            radius: 0
            samples: 0
            opacity: 1
            fast: true
            cached: true
            visible: bottomRect.visible
        }

        Rectangle {
            id: bottomLineHide
            anchors.left: backgroundToolTip.left
            anchors.right: backgroundToolTip.right
            anchors.bottom: backgroundToolTip.bottom
            height: bottomRect.height / 2
            color: backgroundToolTip.color
            visible: bottomRect.visible
            radius: 4
        }
    }
}
