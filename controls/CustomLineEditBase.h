#ifndef CUSTOMLINEEDITBASE_H
#define CUSTOMLINEEDITBASE_H

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QStyle>
#include <QSpacerItem>
#include "Utils.h"
#include "defines.h"
#include <QEvent>

/*
*
*thisWidget
*{
*    qproperty-visibleIcon: true;       for visible left icon
*    qproperty-visibleButton: true;     for visible right button
*
*   If you do not specify these 2 lines, the widget will be exactly the same as its parent,
*                                               and the settings blocks below will not be needed.
*   To offset the text field, use
*                   padding-left: (width image) + margin to text field
*                   padding-right: (width image) + margin to text field
*   .......... Other settings as usual
*}
*
*thisWidget #image (QLabel)
*{
*    min-height: 29pt;
*    max-height: 29pt;
*    min-width: 29pt;
*    max-width: 29pt;
*       ....... and etc
*
*    image: url(image path);
*}
*
*thisWidget #control (QPushButton)
*{
*    min-height: 20pt;
*    max-height: 20pt;
*    min-width: 20pt;
*    max-width: 20pt;
*       ....... and etc
*    image: url(:/pics/delete_icon.svg);
*}
*/

class CustomLineEditBase : public QLineEdit
{
    Q_OBJECT

    Q_PROPERTY(bool visibleIcon WRITE setVisibleIcon DESIGNABLE true)
    Q_PROPERTY(bool visibleButton WRITE setVisibleButton DESIGNABLE true)

public:
    CustomLineEditBase(QWidget* parent = nullptr);
    /// Setting the error signal.
    /// @param a_wrong
    void setWrongState(bool a_wrong);

protected:
    void focusOutEvent(QFocusEvent *e);
    void focusInEvent(QFocusEvent *e);


    /// Sets the visibility of the icon.
    /// @param a_visible This parameter is taken from css settings.
    void setVisibleIcon(bool &a_visible);
    /// Sets the visibility of the button.
    /// @param a_visible This parameter is taken from css settings.
    void setVisibleButton(bool a_visible);
    ///Widget for icon
    QLabel *lblIcon;
    ///Widget for button
    QPushButton *btnControl;
    QHBoxLayout *hblLineEdit;

    QSpacerItem *spacer;
private:


};

#endif // CUSTOMLINEEDITBASE_H
