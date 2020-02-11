#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H


#include <QComboBox>

/** @brief QComboBox with changing downArrow when hover on control.

*To indicate the selection of an object in css
*
*#ComboBoxName[hoverState = "1"]::drop-down,
*{
*    image: url(path to icon);
*}
*
* Everything else unchanged
*/
class CustomComboBox : public QComboBox
{
    Q_OBJECT
public:
    CustomComboBox(QWidget *parent = Q_NULLPTR);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
};
#endif // CUSTOMCOMBOBOX_H
