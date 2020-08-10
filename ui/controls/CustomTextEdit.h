#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QTextEdit>
#include "AppStyleSheetHandler.h"
#include <QStyle>
#include "defines.h"
#include "Utilz.h"
#include <QScroller>
#include <QLabel>

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
    Q_OBJECT

    Q_PROPERTY(bool usingCustomPlaceholder WRITE setUsingCustomPlaceholder  DESIGNABLE true)
public:
    explicit CustomTextEdit(QWidget *a_parent = nullptr);

    void setUsingCustomPlaceholder(bool a_usePlaceholder);

    void setPlaceholderText(const QString &placeholderText);
protected:

    void focusOutEvent(QFocusEvent *e);

    void focusInEvent(QFocusEvent *e);

private:
    void createCustomPlaceholder();
    void deleteCustomPlaceholder();

    QHBoxLayout*    m_layoutCtrl = nullptr;
    QLabel*         m_placeHolderCtrl = nullptr;

    bool    m_usingCustomPlaceholder = false;
};

#endif // CUSTOMTEXTEDIT_H
