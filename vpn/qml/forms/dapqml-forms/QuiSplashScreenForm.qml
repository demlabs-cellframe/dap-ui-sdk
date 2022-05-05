import QtQuick 2.12
import QtGraphicalEffects 1.5
import "qrc:/dapqml-widgets"

Item {
    id: root
    enabled: false
    property string formName: "SplashScreen"

    property color mainColor: "#FFFFFF"
    property string statusText: "Loading..."
    property string versionText: "..."
    property string stripesPng: "qrc:/stripes.png"
    property string logoPng: "qrc:/logo.png"

    /* funcs */
    function setStatusText(a_message) {
        statusText  = a_message;
    }

    function setVersion(a_version) {
        versionText = a_version;
    }

    function freeMem() {
        stripesPng  = "";
        logoPng     = "";
    }

    /* gradient background */
    Rectangle {
        id: rect
        z: 10
        anchors.fill: parent

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#E62CC7" }
            GradientStop { position: 1.0; color: "#F53172" }
        }
    }

    /* stripes */
    Image {
        z: 11
        anchors.fill: parent
        mipmap: true
        source: root.stripesPng
    }

    /* logo */
    DapQmlLabel {
        x: parent.width / 2 - width / 2
        y: 248
        z: 12
        width: 230
        height: 59
        qss: "splash-logo"
    }

    /* status text */
    DapQmlLabel {
        qss: "splash-status"
        z: 12
        width: parent.width
        text: root.statusText
        color: root.mainColor
    }

    /* version text */
    DapQmlLabel {
        qss: "splash-version"
        y: parent.height - height
        z: 12
        width: parent.width
        text: root.versionText
        color: root.mainColor
    }
}
