import QtQuick 2.4
import QtQuick.Controls
import QtQuick.Controls.Styles 1.4

DapCalendarForm
{
    id: root
    dapCalendar.minimumDate:
    {
        var time = new Date(dapMinimumDate)
        time.setHours(0)
        time.setMinutes(0)
        return time
    }
    dapCalendar.style:
        CalendarStyle
        {
            id: dapCalendarStyle
            gridVisible: false

            background: Item {
                implicitWidth: Math.max(250, Math.round(implicitHeight * 14))
                implicitHeight: Math.max(250, Math.round(implicitHeight * 14))
            }



            navigationBar:
                Item
                {
                    id: rectangleNavigationBar
                    height: buttonPreviousYear.height + dapTitleTopPadding + dapTitleBottomPadding
//                    color: dapCalendarBackgroundColor

                    Item
                    {
                        anchors.top: parent.top
                        anchors.topMargin: dapTitleTopPadding
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: dapTitleBottomPadding

                        DapButton
                        {
                            id: buttonPreviousYear
                            width: (dapCalendar.width - dapTitleWidth - 2 * dapButtonInterval) / 4
                            height: width
                            widthImageButton: width
                            heightImageButton: height
                            anchors.left: parent.left
                            checkable: true
                            colorBackgroundNormal: dapCalendarBackgroundColor
                            colorBackgroundHover: colorBackgroundNormal
                            normalImageButton: dapClickYearImage
                            hoverImageButton: normalImageButton
                            onClicked: control.showPreviousYear()
                        }
                        DapButton
                        {
                            id: buttonPreviousMonth
                            width: buttonPreviousYear.width
                            height: width
                            widthImageButton: width
                            heightImageButton: height
                            anchors.left: buttonPreviousYear.right
                            anchors.leftMargin: dapButtonInterval
                            checkable: true
                            colorBackgroundNormal: dapCalendarBackgroundColor
                            colorBackgroundHover: colorBackgroundNormal
                            normalImageButton: dapClickMonthImage
                            hoverImageButton: normalImageButton
                            onClicked: control.showPreviousMonth()
                        }
                        Label
                        {
                            id: calendarHeadText
                            text: qsTr(styleData.title.replace(' ', ', '))
                            color:  dapNormalTextColor
                            horizontalAlignment: Text.AlignHCenter
                            font: dapCalendarFont
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: buttonPreviousMonth.right
                            width: dapTitleWidth
                        }
                        DapButton
                        {
                            id: buttonNextMonth
                            width: buttonPreviousYear.width
                            height: width
                            widthImageButton: width
                            heightImageButton: height
                            anchors.left: calendarHeadText.right
                            checkable: true
                            colorBackgroundNormal: dapCalendarBackgroundColor
                            colorBackgroundHover: colorBackgroundNormal
                            normalImageButton: dapClickMonthImage
                            hoverImageButton: normalImageButton
                            imageMirror: true
                            onClicked: control.showNextMonth()
                        }
                        DapButton
                        {
                            id: buttonNextYear
                            width: buttonPreviousYear.width
                            height: width
                            widthImageButton: width
                            heightImageButton: height
                            anchors.left: buttonNextMonth.right
                            anchors.leftMargin: dapButtonInterval
                            checkable: true
                            colorBackgroundNormal: dapCalendarBackgroundColor
                            colorBackgroundHover: colorBackgroundNormal
                            normalImageButton: dapClickYearImage
                            hoverImageButton: normalImageButton
                            imageMirror: true
                            onClicked: control.showNextYear()
                        }
                    }
                }

            dayOfWeekDelegate:
                Rectangle
                {
                    id: rectangleDayOfWeekDelegate
                    width: dapDayWidth
                    height: dapDayHeight
                    color: dapCalendarBackgroundColor
                    Label
                    {
                        text: qsTr(dapCalendar.__locale.dayName(styleData.dayOfWeek, dapDayOfWeeksFormat))
                        anchors.centerIn: parent
                        color:  dapNormalTextColor
                        horizontalAlignment: Text.AlignHCenter
                        font: dapCalendarFont
                    }
                }

            dayDelegate:
                Rectangle
                {
                    id: rectangleDayDelegate
                    width: dapDayWidth + (styleData.index % 7 === 6 ? 0 : dapDayLeftInterval)
                    height: dapDayHeight + (styleData.index >= 35 ? 0 : dapDayTopInterval)
                    color: dapCalendarBackgroundColor
                    Rectangle
                    {
                        anchors.left: parent.left
                        anchors.top: parent.top
                        width: dapDayWidth
                        height: dapDayHeight
                        border.color: ((styleData.date.getDate() === new Date(Date.now()).getDate()) &&
                                       (styleData.date.getMonth() === new Date(Date.now()).getMonth()) &&
                                       (styleData.date.getFullYear() === new Date(Date.now()).getFullYear())) ?
                                          dapSelectedBackgroundColor :
                                          (styleData.selected ? dapSelectedBackgroundColor : dapNormalBackgroundColor)
                        border.width: 1
                        color: styleData.selected ? dapSelectedBackgroundColor : dapNormalBackgroundColor

                        Label
                        {
                            text: styleData.date.getDate()
                            anchors.centerIn: parent
                            color: styleData.selected ?
                                       dapSelectedTextColor :
                                       ((styleData.valid && styleData.visibleMonth) ? dapNormalTextColor : dapInvalidTextColor)
                            horizontalAlignment: Text.AlignHCenter
                            font: dapCalendarFont
                        }
                    }
                }
        }

    dapCalendar.onDoubleClicked:
    {
        dapCalendarResult = Qt.formatDate(dapCalendar.selectedDate, dapCalendarResultFormat);
        visible = false;
    }
    dapCalendar.onMinimumDateChanged:
    {
        if(dapCalendarResult !== "" && dapCalendar.selectedDate < dapMinimumDate )
        {
            dapCalendar.selectedDate = dapMinimumDate;
            dapCalendarResult = Qt.formatDate(dapCalendar.selectedDate, dapCalendarResultFormat);
        }
    }
}
