#ifndef PASSWORDLINEEDIT_H
#define PASSWORDLINEEDIT_H

#include "CustomLineEditBase.h"
#include <QStyle>
#include <QEvent>
#include "Utilz.h"
#include "defines.h"

/*
*
*thisWidget
*{
*    qproperty-visibleIcon: true;       for visible left icon
*    qproperty-visibleButton: true;     for visible right button
*
*   If you do not specify these 2 lines, the widget will be exactly the same as its parent,
*                                               and the settings blocks below will not be needed.
*
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
*
* Properties at the time of typing.
*
*thisWidget:focus,
*{
*    some property
*}
*
* Properties when focus is lost or the end of the input.
*
*thisWidget[state = "endEdit"]
*{
*    some property
*}
*
*Properties when an error signal is sent.
*
*thisWidget[state = "error"]
*{
*    some property
*}
*
*Properties when an error signal is sent.
*
*thisWidget #control[state = "error"]
*{
*    some property
*}
*/

class PasswordLineEdit : public CustomLineEditBase
{
    Q_OBJECT

public:
    PasswordLineEdit(QWidget *parent = nullptr);
private:
    void setSettingsButton();
};

#endif // PASSWORDLINEEDIT_H
