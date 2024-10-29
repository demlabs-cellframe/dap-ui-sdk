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
#include <QAbstractTextDocumentLayout>

/**The widget is implemented:
 * Ability to automatically change the height depending on the amount of text.
 *
 *  Required settings.
 *  qproperty-autoChangingSize: true;
 *  qproperty-normalHeight: 48pt;
 *  qproperty-maximimLineCount: 5;
 *
 * Processing the Enter button .
 *
 * qproperty-redefineEnterButton: true;
 *
 *
 * redefined placeholder
 *
 * qproperty-usingCustomPlaceholder: true;
 *
 * Properties of the placeholder
 * .... #plaseholder  (QLabel)
 * {
 *
 *
 * }
 *
 *
*/

class CustomTextEdit: public QTextEdit
{
    Q_OBJECT

    Q_PROPERTY(bool usingCustomPlaceholder WRITE setUsingCustomPlaceholder  DESIGNABLE true)
    Q_PROPERTY(bool autoChangingSize WRITE setAutoChangingSize  DESIGNABLE true)
    Q_PROPERTY(QString normalHeight WRITE setNormalHeight DESIGNABLE true)
    Q_PROPERTY(int maximimLineCount WRITE setMaximimLineCount DESIGNABLE true)
    Q_PROPERTY(bool redefineEnterButton WRITE setRedefineEnterButton DESIGNABLE true)

public:
    explicit CustomTextEdit(QWidget *a_parent = nullptr);

    ///Sets up a custom placeholder.
    void setUsingCustomPlaceholder(bool a_usingPlaceholder);

    ///Sets the placeholder text.
    void setPlaceholderText(const QString &placeholderText);

    ///Sets the widget height to be changed automatically.
    void setAutoChangingSize(bool a_using);

    ///Sets the widget height for a single row, including all internal margins. if m_autoChangingSize = true
    void setNormalHeight(const QString &a_heightStr);

    ///The maximum number of rows is set. if m_autoChangingSize = true
    void setMaximimLineCount(int a_maxLine);

    ///Enter processing is set.
    void setRedefineEnterButton(bool a_redefine);
protected:

    void focusOutEvent(QFocusEvent *e);

    void focusInEvent(QFocusEvent *e);

    void resizeEvent(QResizeEvent *e);

    void inputMethodEvent(QInputMethodEvent *e);

    void keyPressEvent(QKeyEvent *e);
signals:
    ///Sent when the number of characters changes.
    void numberCharacterChange(int a_count);

    void focusChanged(bool a_focus);
    ///Sent when you click on Enter
    void pressedEnter();
public:
    ///Sets the height of the widget when entering text. if m_autoChangingSize = true
    void setNewHeight(const QSizeF &newSize);
    ///Sets the height of the placeholder widget. if m_autoChangingSize = true
    void setNewHeightForPlaceholder(const QFont &a_font, const QString &a_text);

    void createCustomPlaceholder();
    void deleteCustomPlaceholder();

    void sizingCondition();

    void checkingTextPlaceholder();
    ///Checks whether the number of characters has changed and sends a signal numberCharacterChange.
    void sendNumberOfCharacters(int a_number);

    QHBoxLayout*    m_layoutCtrl = nullptr;
    ///Custom placeholder.
    QLabel*         m_placeHolderCtrl = nullptr;
    ///key for using a custom placeholder.
    bool    m_usingCustomPlaceholder = false;
    ///The height of the widget when it is empty or only one line is used.  if m_autoChangingSize = true
    int m_normalHeight = 0;
    ///Height of a single row. if m_autoChangingSize = true
    qreal m_normalLineHeight = 0;
    ///Maximum number of rows. if m_autoChangingSize = true
    int m_maxLineCount = 0;
    ///The current number of characters. Used as a buffer.
    int m_numberCharacter = 0;
    ///Key points to auto change the height.
    bool m_autoChangingSize = false;
    ///A key that indicates whether the Enter key is redefined.
    bool m_redefineEnterButton = false;
};

#endif // CUSTOMTEXTEDIT_H
