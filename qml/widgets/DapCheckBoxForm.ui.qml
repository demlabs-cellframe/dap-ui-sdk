import QtQuick 2.4
import QtQuick.Controls 2.0

CheckBox {
    id: customCheckBox

    property string checkboxOn:""
    property string checkboxOff:""
    ///@detalis textButton Text RadioButton.
    property alias nameCheckbox: nameButton.text
    ///@detalis fontText Font setting.
    property alias fontCheckbox:nameButton.font
    ///@detalis nameTextColor Text color.
    property alias nameTextColor: nameButton.color
    ///@detalis widthRadioButton Width RadioButton.
    property alias widthCheckbox: customCheckBox.implicitWidth
    ///@detalis heightRadioButton Height RadioButton.
    property alias heightCheckbox: customCheckBox.implicitHeight
    ///@detalis backgroundColor RadioButton background color
    property alias backgroundColor:backgroundColor.color
    ///@detalis spaceIndicatorText The gap between the indicator and the text.
    property int spaceIndicatorText
    ///@detalis indicatorBorderColor Border color indicator.
//    property string indicatorBorderColor
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

    ///Text Options.
    contentItem:
        Text
        {
            id: nameButton
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: customCheckBox.indicator.width + spaceIndicatorText
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
//            anchors.top: parent.top
            color: "#3E3853"
            horizontalAlignment: Text.AlignLeft
            text: qsTr("template")
        }
//    ///Indicator Options.
    indicator:

        ///Indicator inner options.
        Image
        {
            width: indicatorInnerSize
            height: indicatorInnerSize
            anchors.top: parent.top
//            x: 0
//            y: parent.height / 2 - height / 2
            y: parent.height
            source: checked ? checkboxOn : checkboxOff
        }

    ///Background options.
    background: Rectangle
    {
        id:backgroundColor
        anchors.fill:parent
        color:"transparent"
    }

    checked: false
}