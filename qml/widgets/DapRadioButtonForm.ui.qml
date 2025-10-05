import QtQuick 2.4
import QtQuick.Controls 2.0
import Qt5Compat.GraphicalEffects
RadioButton
{
    ///@detalis textButton Text RadioButton.
    property alias nameRadioButton: nameButton.text
    ///@detalis fontText Font setting.
    property alias fontRadioButton:nameButton.font
    ///@detalis nameTextColor Text color.
    property alias nameTextColor: nameButton.color
    ///@detalis widthRadioButton Width RadioButton.
    property alias widthRadioButton: customRadioButton.implicitWidth
    ///@detalis heightRadioButton Height RadioButton.
    property alias heightRadioButton: customRadioButton.implicitHeight
    ///@detalis backgroundColor RadioButton background color
    property alias backgroundColor:backgroundColor.color
    ///@detalis spaceIndicatorText The gap between the indicator and the text.
    property int spaceIndicatorText
    ///@detalis indicatorBorder Border indicator.
//    property alias indicatorBorder: indicatorRadioButton.border
    ///@detalis indicatorBorderColor Border color indicator.
    property string indicatorBorderColor
    ///@detalis indicatorBackgroundColor Background color indicator.
    property string indicatorBackgroundColor
    ///@detalis indicatorInnerColorActiv Color of the inner circle in checked condition.
    property string indicatorInnerColorActiv
    ///@detalis indicatorInnerColorNormal Color of the inner circle in normal condition.
    property string indicatorInnerColorNormal
    ///@detalis indicatorSize The size of the main circle of the indicator.
    property int indicatorSize
    ///@detalis indicatorInnerSize The size of the inner circle of the indicator.
    property int indicatorInnerSize   


    id: customRadioButton

    ///Text Options.
    contentItem:
        Text
        {
            id: nameButton
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: customRadioButton.indicator.width + spaceIndicatorText
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
            color: currTheme.textColor
            horizontalAlignment: Text.AlignLeft
            text: qsTr("template")
        }
    ///Indicator Options.
    indicator:
        Image {
        anchors.verticalCenter: parent.verticalCenter
        width: indicatorInnerSize
        height: indicatorInnerSize
        mipmap: true

        source: checked ? "qrc:/resources/icons/" + pathTheme + "/radio_btn_on.png" : "qrc:/resources/icons/" + pathTheme + "/radio_btn_off.png"
        visible: true
    }

    ///Background options.
    background: Rectangle
    {
        id:backgroundColor
        anchors.fill:parent
        color:"transparent"
    }

    checked: false
    autoExclusive: true
}
