import QtQuick 2.0
import DapQmlStyle 1.0

Item {
    id: root
    width: 270
    height: 174
    property bool checked: false
    property string qss

    /* signals */
    signal clicked();

    /* functions */
    function setState(value) {
        checked = value;
        bg.qss  = (!checked) ? "switch-bg-off" : "switch-bg-on";
        tgl.qss = (!checked) ? "switch-toggle-off" : "switch-toggle-on";
    }

    function toggle() {
        setState(!checked);
        root.clicked();
    }

    function centerHor(item) {
        return root.width / 2 - item.width / 2;
    }

    function centerVer(item) {
        return root.height / 2 - item.height / 2;
    }

    /* background */
    DapQmlLabel {
        id: bg
        x: centerHor(this)
        y: centerVer(this)
        z: 0
        width: root.width - 24 * (root.width / 270)
        height: root.height - 48 * (root.height / 174)
        qss: "switch-bg-off"
    }

    /* toggler */
    DapQmlLabel {
        id: tgl
        x: (checked === false) ? (0) : (root.width - width)
        y: 0
        z: 1
        width: root.height
        height: root.height
        qss: "switch-toggle-off"

        Behavior on x { PropertyAnimation { duration: 250; easing.type: Easing.InQuad } }
    }

    /* clicked */
    MouseArea {
        anchors.fill: parent
        onClicked: toggle()
    }

    DapQmlStyle { id: style; qss: root.qss; item: root }
}
