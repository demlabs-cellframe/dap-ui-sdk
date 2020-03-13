#ifndef CUSTOMLINEHEIGHTTEXTEDIT_H
#define CUSTOMLINEHEIGHTTEXTEDIT_H

#include <QTextEdit>
#include "AppStyleSheetHandler.h"
#include <QStyle>

/** @brief QGraphicsDropShadowEffect that can be adjusted from css
 *
 *  @details Set style in .css file in format
 *
 *> #elementName
 *> {
 *>
 *>    line-height: 100%; (100% or 100)
 *>}
 * To specify PlaceholderText parameters
 *> #elementName[state="0"]
 *> {
 *> color:#A1A1A1;
 *>
 *>}
 * To specify text parameters
 *> #elementName[state="1"]
 *> {
 *>color:#000000;
 *>
 *>}
 *
*/

class CustomLineHeightTextEdit: public QTextEdit
{
public:
    explicit CustomLineHeightTextEdit(QWidget *a_parent = Q_NULLPTR);
    /** @brief Sets the object name and updates the styles.
     *  @param placeholderText
     */
    void setObjectName(const QString &name);
    /** @brief Updates the styles.
     */
    void updateStyleSheets();
    /** @brief The redefined method returns plain text as html for setting the line spacing.
     *  @param placeholderText
     */
    void setPlaceholderText(const QString &placeholderText);
    /** @brief Returns text or empty string.
     */
    QString toPlainText() const;
protected:
    /** @brief The redefined method returns plain text as html for setting the line spacing.
     *  @param e
     */
    void focusOutEvent(QFocusEvent *e);
    /** @brief By clicking the mouse button, removes text from the screen and updates the settings for text.
     *  @param e
     */
    void mousePressEvent(QMouseEvent *e);

    const QString STATE_HOVER = "hover";
    const QString STATE_DEFAULT = "default";
    const QString STATE_END_EDIT = "endEdit";

private:
    /** @brief Adds a style to the text from the html line height.
     *  @param text
     */
    QString textToHtml(const QString &text);
    /** @brief The value of the row height from the style sheet.
     */
    QString m_lineHeight;
    /** @brief Default text.
    */
    QString m_placeholderText;

};

#endif // CUSTOMLINEHEIGHTTEXTEDIT_H
