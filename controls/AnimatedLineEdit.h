#ifndef ANIMATEDLINEEDIT_H
#define ANIMATEDLINEEDIT_H

#include <QObject>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QStateMachine>
#include <QDebug>
#include <QResizeEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSignalTransition>

#include "DapStyleHolder.h"

/** @brief QLineEdit with animated QLabel subControls (placeholder text)
 *
 *  @details Move label top when is filled
 * Set style in .css file in format
 *> .AnimatedLineEdit {
 *>     color: ...;
 *>     font-size: ...px;
 *>          ...
 *> }
 *> .AnimatedLineEdit::label {
 *>     color: ...;
 *>     font-size: ...px;
 *> }
 *> .AnimatedLineEdit::label:focus {
 *>     color: ...;
 *> }
*/
class AnimatedLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QColor labelColor READ labelColor WRITE setLabelColor)
    Q_PROPERTY(int labelFontSize READ labelFontSize WRITE setLabelFontSize)

public:
    explicit AnimatedLineEdit(QWidget *a_parent = nullptr);

    void setPlaceholderText(const QString &a_text);

    void setObjectName(QString a_objName);

    void updateLabelProperties();

    QColor labelColor() const;
    void setLabelColor(const QColor &a_placeHolderColor);

    int labelFontSize() const;
    void setLabelFontSize(int a_labelFontSize);

signals:
    void focusOut();        ///< Signal is emitted when focus out
    void focusOutEmpty();   ///< Signal is emitted when focus out and Line Edit is empty
    void focusOutFilled();  ///< Signal is emitted when focus out and Line Edit is filled
    void focusIn();         ///< Signal is emitted when get focus

protected:
    virtual void resizeEvent(QResizeEvent *a_event);
    virtual void focusInEvent(QFocusEvent *a_event);
    virtual void focusOutEvent(QFocusEvent *a_event);

    virtual void showEvent(QShowEvent *a_event);
    virtual void hideEvent(QHideEvent *a_event);

    void setupStateMachine();
    void updateLabelPositionProperty();

private:

    static const int ANIMATION_DURATION {200};
    static const QString LABEL_OBJECT_NAME;

    QLabel  m_label;

    //============ State Machine ============
    QStateMachine m_stateMachine;
    QState *m_st_content;
    QState *m_st_content_filled;
    QState *m_st_content_empty;
    QState *m_st_state;
    QState *m_st_state_focus;
    QState *m_st_state_notFocus;

    //properties:
    QColor  m_labelColor;
    int     m_labelFontSize;

    void assignFontSizeFromStylesheet(const QString &a_stylesheet, QState *a_state);
};



#endif // ANIMATEDLINEEDIT_H
