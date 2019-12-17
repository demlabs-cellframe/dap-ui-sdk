import QtQuick 2.0
import QtQuick.Controls 2.5

ItemDelegate {
    property string delegateContentText: ""
    width: parent.width
    height:{
        if(index == currentIndex) return 0
        else return 42*pt
    }
    contentItem: Text {
        id:textDelegateComboBox
        anchors.fill: parent
        anchors.topMargin: 8 * pt
        anchors.leftMargin: popup.visible ? sidePaddingActive : sidePaddingNormal
        verticalAlignment: Qt.AlignTop
        font.family: fontRobotoRegular.name
            font.pixelSize: fontSizeComboBox
            text: {if(index != currentIndex) return delegateContentText;}
            color: hovered ? hilightColorText : normalColorText
    }

    background: Rectangle {
        anchors.fill: parent
        anchors.bottomMargin: 10 * pt
        color: hovered ? hilightColor : normalColor
    }

    highlighted: parent.highlightedIndex === index

}
