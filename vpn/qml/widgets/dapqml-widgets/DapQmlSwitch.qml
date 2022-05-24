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
    }

    function _setStyle() {
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

    onCheckedChanged: _setStyle()

    /* background */
    DapQmlLabel {
        id: bg
        x: centerHor(this)
        y: centerVer(this)
        z: 0
        width: root.width - 12 * (root.width / 270)
        height: root.height - 36 * (root.height / 174)
        qss: "switch-bg-off"

        onClicked: toggle()
    }

    /* toggler */
    DapQmlLabel {
        id: tgl
        x: (checked === false) ? (-12 * (root.width / 270)) : (root.width - width + 12 * (root.width / 270))
        y: 0
        z: 1
        width: root.height
        height: root.height
        qss: "switch-toggle-off"

        onClicked: toggle()
        Behavior on x { PropertyAnimation { duration: 250; easing.type: Easing.InQuad } }
    }

    /* clicked */
    MouseArea {
        anchors.fill: parent
        onClicked: toggle()
    }

    DapQmlStyle { id: style; qss: root.qss; item: root }
}
