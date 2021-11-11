import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

TextField {
    property int borderWidth:0
    property int borderRadius:0

    style:
        TextFieldStyle
        {
            textColor: currTheme.textColor
            placeholderTextColor: currTheme.placeHolderTextColor

            background:
                Rectangle
                {
                    radius: borderRadius * pt
                    border.width: borderWidth
                    border.color: currTheme.borderColor
                    color: currTheme.backgroundElements
                }
        }


}
