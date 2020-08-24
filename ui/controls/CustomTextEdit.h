#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QTextEdit>
#include "AppStyleSheetHandler.h"
#include <QStyle>
#include "defines.h"
#include "Utilz.h"
#include <QScroller>
#include <QLabel>
#include <QFontMetrics>
#include <QtMath>
#include <QResizeEvent>

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
    Q_PROPERTY(bool usingResizableSize WRITE setUsingResizableSize  DESIGNABLE true)
    Q_PROPERTY(QString normalHeight WRITE setNormalHeight DESIGNABLE true)
    Q_PROPERTY(int maximimLineCount WRITE setMaximimLineCount DESIGNABLE true)
public:
    explicit CustomTextEdit(QWidget *a_parent = nullptr);

    void setUsingCustomPlaceholder(bool a_usingPlaceholder);

    void setPlaceholderText(const QString &placeholderText);

    void setUsingResizableSize(bool a_using);

    void setNormalHeight(const QString &a_heightStr);
    void setMaximimLineCount(int a_maxLine);
protected:

    void focusOutEvent(QFocusEvent *e);

    void focusInEvent(QFocusEvent *e);

    void resizeEvent(QResizeEvent *e);

    void inputMethodEvent(QInputMethodEvent *e);
private:
    void createCustomPlaceholder();
    void deleteCustomPlaceholder();

    void setNewHeight(const QFont &a_font, const QString &a_text);

    void checkingTextPlaceholder();

    QHBoxLayout*    m_layoutCtrl = nullptr;
    QLabel*         m_placeHolderCtrl = nullptr;

    bool    m_usingCustomPlaceholder = false;

    int m_normalHeight = 0;

    int m_maxLineCount = 0;

    bool m_usingResizeableSize = false;

};

#endif // CUSTOMTEXTEDIT_H
