#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H


#include <QComboBox>
#include "StyledDropShadowEffect.h"

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
    void setObjectName(const QString &name);
    void showPopup();
signals:
    void showCustomWindow();
protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
private:
    StyledDropShadowEffect *m_styledshadow;

};
#endif // CUSTOMCOMBOBOX_H
