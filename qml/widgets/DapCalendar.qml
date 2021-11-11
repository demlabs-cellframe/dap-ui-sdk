import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

DapCalendarForm
{
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

            navigationBar:
                Rectangle
                {
                    id: rectangleNavigationBar
                    height: buttonPreviousYear.height + dapTitleTopPadding + dapTitleBottomPadding
                    color: dapCalendarBackgroundColor

                    Rectangle
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
                            normalImageButton: dapPreviousYearButtonImage
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
                            normalImageButton: dapPreviousMonthButtonImage
                            hoverImageButton: normalImageButton
                            onClicked: control.showPreviousMonth()
                        }
                        Label
                        {
                            id: calendarHeadText
                            text: styleData.title.replace(' ', ', ')
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
                            normalImageButton: dapNextMonthButtonImage
                            hoverImageButton: normalImageButton
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
                            normalImageButton: dapNextYearButtonImage
                            hoverImageButton: normalImageButton
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
                    Label
                    {
                        text: dapCalendar.__locale.dayName(styleData.dayOfWeek, dapDayOfWeeksFormat)
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
                        border.width: 1 * pt
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
