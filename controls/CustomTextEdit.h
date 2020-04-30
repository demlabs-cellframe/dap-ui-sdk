#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QTextEdit>
#include "AppStyleSheetHandler.h"
#include <QStyle>
#include "defines.h"
#include "Utils.h"
#include <QScroller>

/** @brief QGraphicsDropShadowEffect that can be adjusted from css
 *
 *  @details Set style in .css file in format
 *     If the field is filled in and out of focus.
 *> #elementName[filled="true/false"]
 *> {
 *> color:#A1A1A1;
 *>
 *>}
 * If the field is in focus.
 * #elementName[active="true/false"]
 *
 *
*/

class CustomTextEdit: public QTextEdit
{
public:
    explicit CustomTextEdit(QWidget *a_parent = Q_NULLPTR);

protected:

    void focusOutEvent(QFocusEvent *e);

    void inputMethodEvent(QInputMethodEvent *event);

    void focusInEvent(QFocusEvent *e);

};

#endif // CUSTOMTEXTEDIT_H
