import QtQuick 2.4
import QtQuick.Controls 2.0

import Qt5Compat.GraphicalEffects

ComboBox
{
    id: dapComboBoxWithRange

    ///@details dapNormalColor Item color in normal state.
    property string dapNormalColor
    ///@detalis dapHilightColor Item color in selected state.
    property string dapHilightColor
    ///@detalis dapNormalTopColor Top string color in normal state.
    property string dapNormalTopColor
    ///@details dapHilightTopColor Top string color in selected state.
    property string dapHilightTopColor
    ///@details dapWidthPopupComboBoxNormal Width of the combo box in the normal state.
    property int dapWidthPopupComboBoxNormal
    ///@details dapWidthPopupComboBoxActive Width of the ComboBox in the active state.
    property int dapWidthPopupComboBoxActive
    ///@details dapHeightComboBoxNormal Height of the ComboBox in the normal state.
    property int dapHeightComboBoxNormal
    ///@details dapHeightComboBoxActive Height of the ComboBox in the active state.
    property int dapHeightComboBoxActive
    ///@details dapSidePaddingNormal: Sets the indent from the edge of the right and left edges of the parent in the normal state.
    property int dapSidePaddingNormal
    ///@details dapSidePaddingActive Sets the indent from the edge of the right and left edges of the parent in the active state.
    property int dapSidePaddingActive
    ///@details dapPaddingTopItemDelegate Indent above from item delegate.
    property int dapPaddingTopItemDelegate
    ///@details dapHeightListElement List item height.
    property int dapHeightListElement
    ///@details dapIntervalListElement Spacing between items in a list.
    property int dapIntervalListElement
    ///@details dapBottomIntervalListElement Spacing from bottom to bottom.
    property int dapBottomIntervalListElement
    ///@details dapTopEffect Using an effect for the top element.
    property bool dapTopEffect
    ///@details dapIndicatorImageNormal Indicator picture address for normal state.
    property string dapIndicatorImageNormal
    ///@details dapIndicatorImageActive Indicator picture address for active state.
    property string dapIndicatorImageActive
    ///@details dapIndicatorWidth Indicator width.
    property int dapIndicatorWidth
    ///@details dapIndicatorHeight Indicator height.
    property int dapIndicatorHeight
    ///@details dapIndicatorLeftInterval Space between indicator border and text border.
    property int dapIndicatorLeftInterval
    ///@details dapColorTopNormalDropShadow Color of the shadow effect of the combo box when minimized.
    property string dapColorTopNormalDropShadow
    ///@details dapColorDropShadow Unboxed shadow color in expanded state.
    property string dapColorDropShadow

    ///@details dapNormalColorText Text color in normal state.
    property string dapNormalColorText
    ///@details dapHilightColorText Text color in selected state.
    property string dapHilightColorText
    ///@details dapNormalColorTopText Text color in the main line in normal state.
    property string dapNormalColorTopText
    ///@details dapHilightColorTopText Text color in the main line in active state.
    property string dapHilightColorTopText
    ///@details dapSelectedRangeColorTopText Text color in the main line for active range element.
    property string dapSelectedRangeColorTopText
    ///@details dapUnselectedRangeColorTopText Text color in the main line for not active range element.
    property string dapUnselectedRangeColorTopText
    ///@details dapTextFont Main font of comboBox
    property var dapTextFont
    ///@details dapMainLineText Text of the main line without unneccesary part.
    property string dapMainLineText
    ///@details dapIsDefaultNeedToAppend Sign to add default data to the beginning of model
    property bool dapIsDefaultNeedToAppend: false
    ///@details dapDefaultMainLineText External value which is added to comboBox model
    property string dapDefaultMainLineText: ""
    ///@detalis dapEndRowPadding The width of padding at the end of one row at ComboBox where it is need.
    property int dapEndRowPadding
    ///@details dapComboBoxTextMetric ID of comboBox Text Metric
    property alias dapComboBoxTextMetric: comboBoxTextMetric
    ///@details dapComboBoxFontMetric ID of comboBox Font Metric
    property alias dapComboBoxFontMetric: comboBoxFontMetric
    ///@details dapIsRange Sign of setting two data (range) to main line
    property bool dapIsRange: false
    ///@details dapRangeElementWidth Width of one text element of range
    property int dapRangeElementWidth
    ///@details dapRangeSpacing Space between range element and range separator
    property int dapRangeSpacing
    ///@details dapRangeDefaultText Default text - while range element will not selected
    property string dapRangeDefaultText
    ///@details  dapActiveRangeTextFont Font for active element of range text at main line
    property var dapActiveRangeTextFont
    ///@details  dapInactiveRangeTextFont Font for not active element of range text at main line
    property var dapInactiveRangeTextFont
    ///@details dapCalendars Item with properties for two calendars: for select minimum of range and maximum of range
    property DapCalendar dapCalendars
    ///@details varResultText Temporary variable with currentText of comboBox
    property string varResultText: (textMinimumOfRange.text + textSeparator.text + textMaximumOfRange.text)
    ///@details dapResultText CurrentText of comboBox
    property string dapResultText: textMinimumOfRange.text

    width: popup.visible ? dapWidthPopupComboBoxActive : dapWidthPopupComboBoxNormal
    height: popup.visible ? dapHeightComboBoxActive : dapHeightComboBoxNormal
    anchors.verticalCenter: parent.verticalCenter

    //To do elide
    TextMetrics
    {
        id: comboBoxTextMetric
        font: dapTextFont
        elide: Text.ElideRight
    }
    // To check elide
    FontMetrics
    {
        id: comboBoxFontMetric
        font: dapTextFont
    }

    //Icon icon near the text (arrow)
    indicator:
        Image
        {
            source: parent.popup.visible ? dapIndicatorImageActive : dapIndicatorImageNormal
            width: dapIndicatorWidth
            height: dapIndicatorHeight
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: popup.visible ? dapSidePaddingActive : dapSidePaddingNormal
        }

    //Defining the background for the main line
    background:
    Rectangle
    {
        anchors.fill: parent
        color: "transparent"
        radius: 4

        Rectangle
        {
            id: backGrnd
            radius: 4
            width: parent.width
            height: dapComboBoxWithRange.popup.visible ?
                        parent.height + popupBackGrnd.height :
                        parent.height

            y: dapComboBoxWithRange.popup.visible &&
               dapComboBoxWithRange.popup.y < 0 ?
                 - popupBackGrnd.height :
                 0

            color: dapComboBoxWithRange.popup.visible ?
                       dapHilightTopColor :
                       dapNormalTopColor
        }

        DropShadow
        {
            anchors.fill: backGrnd
            horizontalOffset: currTheme.hOffset
            verticalOffset: currTheme.vOffset
            radius: currTheme.radiusShadow
            color: currTheme.shadowColor
            source: backGrnd
            samples: 10
            cached: true
            visible: dapComboBoxWithRange.popup.visible
        }

    }

    //contentItem is setting by children
    contentItem:
        Rectangle
        {
            id: mainLineRectangle
            anchors.fill: parent
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: popup.visible ? dapSidePaddingActive : dapSidePaddingNormal
            color: "transparent"
            Rectangle
            {
                id: rectangleMinimumOfRange
                width: dapIsRange ? dapRangeElementWidth : parent.width
                height: parent.height
                color: "transparent"
                Text
                {
                    id: textMinimumOfRange
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    font: dapIsRange ? (dapMinimumOfRangeCalendar.visible ? dapActiveRangeTextFont : dapInactiveRangeTextFont) : dapTextFont
                    color: dapIsRange ?
                               (dapMinimumOfRangeCalendar.visible ?
                                    dapSelectedRangeColorTopText :
                                    (popup.visible ? dapHilightColorTopText : dapUnselectedRangeColorTopText)
                                ) :
                               (popup.visible ? dapHilightColorTopText : dapNormalColorTopText)
                    text: dapIsRange ?
                              (dapMinimumOfRangeCalendar.dapCalendarResult !== "" ?
                                   dapMinimumOfRangeCalendar.dapCalendarResult :
                                   dapRangeDefaultText
                               ) :
                              dapMainLineText
                    MouseArea
                    {
                        id: mouseAreaMinimumOfRange
                        anchors.fill: parent
                        enabled: dapIsRange
                        onClicked: dapMinimumOfRangeCalendar.visible = !popup.visible
                    }
                    DapCalendar
                    {
                        id: dapMinimumOfRangeCalendar
                        x: parent.x - width*(1/scale-1)*0.5
                        y: parent.y - height - height*(1/scale-1)*0.5

                        dapLeftPadding: dapCalendars.dapLeftPadding
                        dapRightPadding: dapCalendars.dapRightPadding
                        dapTopPadding: dapCalendars.dapTopPadding
                        dapBottomPadding: dapCalendars.dapBottomPadding
                        dapTitleTopPadding: dapCalendars.dapTitleTopPadding
                        dapTitleBottomPadding: dapCalendars.dapTitleBottomPadding
                        dapButtonInterval: dapCalendars.dapButtonInterval
                        dapTitleWidth: dapCalendars.dapTitleWidth
                        dapDayWidth: dapCalendars.dapDayWidth
                        dapDayHeight: dapCalendars.dapDayHeight
                        dapDayLeftInterval: dapCalendars.dapDayLeftInterval
                        dapDayTopInterval: dapCalendars.dapDayTopInterval
                        dapCalendarFont: dapCalendars.dapCalendarFont
                        dapCalendarBackgroundColor: dapCalendars.dapCalendarBackgroundColor
                        dapNormalTextColor: dapCalendars.dapNormalTextColor
                        dapSelectedTextColor: dapCalendars.dapSelectedTextColor
                        dapInvalidTextColor: dapCalendars.dapInvalidTextColor
                        dapNormalBackgroundColor: dapCalendars.dapNormalBackgroundColor
                        dapSelectedBackgroundColor: dapCalendars.dapSelectedBackgroundColor
                        dapDayOfWeeksFormat: dapCalendars.dapDayOfWeeksFormat
                        dapClickMonthImage: dapCalendars.dapClickMonthImage
                        dapClickYearImage: dapCalendars.dapClickYearImage


                        dapShadowColor: dapColorDropShadow
                        visible: false
                    }
                }

            }
            Rectangle
            {
                id: rectangleSeparator
                anchors.left: rectangleMinimumOfRange.right
                anchors.leftMargin: dapIsRange ? dapRangeSpacing : 0
                width: dapIsRange ? textSeparator.width : 0
                height: parent.height
                color: "transparent"
                Text
                {
                    id: textSeparator
                    width: contentWidth
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    font: dapIsRange ? dapActiveRangeTextFont : dapTextFont
                    color: (popup.visible ? dapHilightColorTopText : dapSelectedRangeColorTopText)
                    text: dapIsRange ? '-' : ''
                }
            }
            Rectangle
            {
                id: rectangleMaximumOfRange
                anchors.left: rectangleSeparator.right
                anchors.leftMargin: dapIsRange ? dapRangeSpacing : 0
                width: dapIsRange ? rectangleMinimumOfRange.width : 0
                height: parent.height
                color: "transparent"
                Text
                {
                    id: textMaximumOfRange
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    font: dapIsRange ? (dapMaximumOfRangeCalendar.visible ? dapActiveRangeTextFont : dapInactiveRangeTextFont) : dapTextFont
                    color: dapIsRange ?
                               (dapMaximumOfRangeCalendar.visible ?
                                    dapSelectedRangeColorTopText :
                                    (popup.visible ? dapHilightColorTopText : dapUnselectedRangeColorTopText)
                                ) :
                               (popup.visible ? dapHilightColorTopText : dapNormalColorTopText)
                    text: dapIsRange ?
                              (dapMaximumOfRangeCalendar.dapCalendarResult !== "" ?
                                   dapMaximumOfRangeCalendar.dapCalendarResult :
                                   dapRangeDefaultText
                               ) : ''
                    MouseArea
                    {
                        id: mouseAreaMaximumOfRange
                        anchors.fill: parent
                        enabled: dapIsRange
                        onClicked: dapMaximumOfRangeCalendar.visible = !popup.visible
                    }
                    DapCalendar
                    {
                        id: dapMaximumOfRangeCalendar

                        x: parent.x - width*(1/scale-1)*0.5
                        y: parent.y - height - height*(1/scale-1)*0.5

                        dapLeftPadding: dapCalendars.dapLeftPadding
                        dapRightPadding: dapCalendars.dapRightPadding
                        dapTopPadding: dapCalendars.dapTopPadding
                        dapBottomPadding: dapCalendars.dapBottomPadding
                        dapTitleTopPadding: dapCalendars.dapTitleTopPadding
                        dapTitleBottomPadding: dapCalendars.dapTitleBottomPadding
                        dapButtonInterval: dapCalendars.dapButtonInterval
                        dapTitleWidth: dapCalendars.dapTitleWidth
                        dapDayWidth: dapCalendars.dapDayWidth
                        dapDayHeight: dapCalendars.dapDayHeight
                        dapDayLeftInterval: dapCalendars.dapDayLeftInterval
                        dapDayTopInterval: dapCalendars.dapDayTopInterval
                        dapCalendarFont: dapCalendars.dapCalendarFont
                        dapCalendarBackgroundColor: dapCalendars.dapCalendarBackgroundColor
                        dapNormalTextColor: dapCalendars.dapNormalTextColor
                        dapSelectedTextColor: dapCalendars.dapSelectedTextColor
                        dapInvalidTextColor: dapCalendars.dapInvalidTextColor
                        dapNormalBackgroundColor: dapCalendars.dapNormalBackgroundColor
                        dapSelectedBackgroundColor: dapCalendars.dapSelectedBackgroundColor
                        dapDayOfWeeksFormat: dapCalendars.dapDayOfWeeksFormat
                        dapClickMonthImage: dapCalendars.dapClickMonthImage
                        dapClickYearImage: dapCalendars.dapClickYearImage
                        dapShadowColor: dapColorDropShadow
                        visible: false
                        dapMinimumDate: dapMinimumOfRangeCalendar.dapCalendar.selectedDate
                    }
                }
            }

        }


    //Customize drop-down list with shadow effect
    popup:
        Popup
        {
            id: dapComboboxPopup
            x: -width*(1/scale-1)*0.5
            y: dapComboBoxWithRange.height - height*(1/scale-1)*0.5
            width: dapComboBoxWithRange.width
            padding: 0

            parent: dapComboBoxWithRange

            scale: mainWindow.scale

            contentItem:
                ListView
                {
                    id: popupList
                    clip: true
                    implicitHeight: contentHeight
                    model: popup.visible ? delegateModel : null
                    ScrollIndicator.vertical: ScrollIndicator {}
                }

            background:
                Rectangle
                {
                    id: popupBackGrnd
                    anchors.fill: parent
                    color: currTheme.secondaryBackground
                    radius: 4
                }
        }

    }
