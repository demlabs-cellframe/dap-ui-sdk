import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    
    property string currentServer: ""
    property string errorMessage: ""
    property bool dialogVisible: false
    
    // Signals
    signal sigRetryCurrentServer()
    signal sigSelectNewServer()
    signal sigCloseDialog()
    
    // Functions
    function setVisible(visible) {
        dialogVisible = visible
    }
    
    function setCurrentServer(server) {
        currentServer = server
    }
    
    function setErrorMessage(message) {
        errorMessage = message
    }
    
    anchors.fill: parent
    color: "transparent"
    visible: dialogVisible
    z: 1000
    
    // Background overlay
    Rectangle {
        anchors.fill: parent
        color: "#80000000"
        opacity: dialogVisible ? 1.0 : 0.0
        
        Behavior on opacity {
            NumberAnimation { duration: 200 }
        }
        
        MouseArea {
            anchors.fill: parent
            onClicked: {
                // Prevent clicks from going through
            }
        }
    }
    
    // Dialog content
    Rectangle {
        id: dialog
        width: 400
        height: 280
        anchors.centerIn: parent
        color: "#FFFFFF"
        radius: 8
        border.color: "#E0E0E0"
        border.width: 1
        
        scale: dialogVisible ? 1.0 : 0.8
        opacity: dialogVisible ? 1.0 : 0.0
        
        Behavior on scale {
            NumberAnimation { duration: 200; easing.type: Easing.OutBack }
        }
        
        Behavior on opacity {
            NumberAnimation { duration: 200 }
        }
        
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 16
            
            // Title
            Text {
                text: qsTr("Connection Problem")
                font.pixelSize: 18
                font.weight: Font.Bold
                color: "#333333"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }
            
            // Warning icon and message
            RowLayout {
                Layout.fillWidth: true
                spacing: 12
                
                Rectangle {
                    width: 32
                    height: 32
                    color: "#FFA500"
                    radius: 16
                    
                    Text {
                        anchors.centerIn: parent
                        text: "⚠"
                        font.pixelSize: 18
                        color: "#FFFFFF"
                    }
                }
                
                Text {
                    text: root.errorMessage
                    font.pixelSize: 14
                    color: "#666666"
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                }
            }
            
            // Current server info
            Rectangle {
                Layout.fillWidth: true
                height: 40
                color: "#F5F5F5"
                radius: 4
                border.color: "#E0E0E0"
                border.width: 1
                
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    
                    Text {
                        text: qsTr("Current server:")
                        font.pixelSize: 12
                        color: "#666666"
                    }
                    
                    Text {
                        text: root.currentServer
                        font.pixelSize: 12
                        font.weight: Font.Bold
                        color: "#333333"
                        Layout.fillWidth: true
                    }
                }
            }
            
            // Message
            Text {
                text: qsTr("What would you like to do?")
                font.pixelSize: 14
                color: "#333333"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }
            
            // Buttons
            RowLayout {
                Layout.fillWidth: true
                spacing: 10
                
                Button {
                    text: qsTr("Retry Current Server")
                    Layout.fillWidth: true
                    
                    background: Rectangle {
                        color: parent.pressed ? "#0056B3" : (parent.hovered ? "#007BFF" : "#007BFF")
                        radius: 4
                        border.color: "#007BFF"
                        border.width: 1
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: 12
                        color: "#FFFFFF"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    
                    onClicked: {
                        root.sigRetryCurrentServer()
                    }
                }
                
                Button {
                    text: qsTr("Select New Server")
                    Layout.fillWidth: true
                    
                    background: Rectangle {
                        color: parent.pressed ? "#5A6268" : (parent.hovered ? "#6C757D" : "#6C757D")
                        radius: 4
                        border.color: "#6C757D"
                        border.width: 1
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: 12
                        color: "#FFFFFF"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    
                    onClicked: {
                        root.sigSelectNewServer()
                    }
                }
            }
            
            // Close button
            Button {
                text: qsTr("Close")
                Layout.fillWidth: true
                
                background: Rectangle {
                    color: parent.pressed ? "#E0E0E0" : (parent.hovered ? "#F0F0F0" : "transparent")
                    radius: 4
                    border.color: "#CCCCCC"
                    border.width: 1
                }
                
                contentItem: Text {
                    text: parent.text
                    font.pixelSize: 12
                    color: "#666666"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                
                onClicked: {
                    root.sigCloseDialog()
                }
            }
        }
    }
} 