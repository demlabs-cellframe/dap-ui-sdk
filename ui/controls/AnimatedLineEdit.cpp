#include "AnimatedLineEdit.h"

//static const int ANIMATION_DURATION {200};

const QString AnimatedLineEdit::LABEL_OBJECT_NAME("label");

/** @brief constructor
 *  @param a_parent object parent
 */
AnimatedLineEdit::AnimatedLineEdit(QWidget *a_parent) :
    QLineEdit(a_parent),
    m_label("text", this),
    m_stateMachine(QState::ParallelStates, this),
    m_st_content(new QState(&m_stateMachine)),
    m_st_content_filled(new QState(m_st_content)),
    m_st_content_empty(new QState(m_st_content)),
    m_st_state(new QState(&m_stateMachine)),
    m_st_state_focus(new QState(m_st_state)),
    m_st_state_notFocus(new QState(m_st_state))
{
    m_label.setObjectName(LABEL_OBJECT_NAME);
    m_label.setAlignment(Qt::AlignTop);

    setAlignment(Qt::AlignBottom);

    setupStateMachine();

    connect(this, &AnimatedLineEdit::focusOut, [this]{
        if (text().isEmpty())
            emit focusOutEmpty();
        else
            emit focusOutFilled();
    });
}

/** @brief Reimplemented QLineEdit::setPlaceholderText method. Sets the text of label.
 *  @param a_text New text
 */
void AnimatedLineEdit::setPlaceholderText(const QString &a_text)
{
    m_label.setText(a_text);
}

/** @brief Reimplemented QLineEdit::setObjectName method. Updates assigned properties.
 *  @param a_objName new object name
 */
void AnimatedLineEdit::setObjectName(QString a_objName)
{
    QLineEdit::setObjectName(a_objName);

    updateLabelProperties();
}

/** @brief Reimplemented QLineEdit::resizeEvent method. Updates label sizes.
 *  @param a_event Resize event
 */
void AnimatedLineEdit::resizeEvent(QResizeEvent *a_event)
{
    QLineEdit::resizeEvent(a_event);

    updateLabelPositionProperty();
}

/** @brief Reimplemented QLineEdit::focusInEvent method. Emit focusIn signal.
 *  @param a_event Focus event
 */
void AnimatedLineEdit::focusInEvent(QFocusEvent *a_event)
{
    emit focusIn();
    QLineEdit::focusInEvent(a_event);
}

/** @brief Reimplemented QLineEdit::focusOutEvent method. Emit focusOut signal.
 *  @param a_event Focus event
 */
void AnimatedLineEdit::focusOutEvent(QFocusEvent *a_event)
{
    emit focusOut();
    QLineEdit::focusOutEvent(a_event);
}

/** @brief Reimplemented QLineEdit::showEvent method. Sets up initial state and start state machine.
 *  @param a_event Show event
 */
void AnimatedLineEdit::showEvent(QShowEvent *a_event)
{
    m_labelFontSize = m_label.font().pixelSize();

    m_st_content->setInitialState(text().isEmpty() ? m_st_content_empty : m_st_content_filled);

    m_stateMachine.start();

    QLineEdit::showEvent(a_event);

}

/** @brief Reimplemented QLineEdit::hideEvent method. Stops state machine.
 *  @param a_event Hide event
 */
void AnimatedLineEdit::hideEvent(QHideEvent *a_event)
{
    QLineEdit::hideEvent(a_event);

    m_stateMachine.stop();
}

/** @brief Setup state machine. Is called from constructor
 */
void AnimatedLineEdit::setupStateMachine()
{
    m_st_state  ->setInitialState(m_st_state_notFocus);

    // ======================= Animations ===========================

    QPropertyAnimation *anLabelColor    = new QPropertyAnimation(this, "labelColor");
    QPropertyAnimation *anLabelFontSize = new QPropertyAnimation(this, "labelFontSize");
    QPropertyAnimation *anLabelPosition = new QPropertyAnimation(&m_label, "pos");

    for (auto curAnimation :{anLabelColor, anLabelFontSize, anLabelPosition}) {
        curAnimation->setDuration(ANIMATION_DURATION);
        curAnimation->setEasingCurve(QEasingCurve::OutCirc);
    }

    QParallelAnimationGroup *anParallelGrp = new QParallelAnimationGroup(this);
    anParallelGrp->addAnimation(anLabelFontSize);
    anParallelGrp->addAnimation(anLabelPosition);

    // ====================== Transitions ============================

    m_st_content_filled->addTransition(this, SIGNAL(focusOutEmpty()), m_st_content_empty)->addAnimation(anParallelGrp);
    m_st_content_empty ->addTransition(this, SIGNAL(focusIn())      , m_st_content_filled)->addAnimation(anParallelGrp);

    m_st_state_focus   ->addTransition(this, SIGNAL(focusOut())     , m_st_state_notFocus)->addAnimation(anLabelColor);
    m_st_state_notFocus->addTransition(this, SIGNAL(focusIn())      , m_st_state_focus)->addAnimation(anLabelColor);

    // ===============================================================

    m_st_content_filled->assignProperty(&m_label, "pos", QPoint(0, 0));
}

/** @brief Update position of label when LineEdit isn't filled . Call when sizes have changed
 */
void AnimatedLineEdit::updateLabelPositionProperty() {
    int labelPosY = height() - m_label.height();
    m_st_content_empty->assignProperty(&m_label, "pos", QPoint(0, labelPosY));
}

/** @brief Getter for labelFontSize property
 * @return labelFontSize
 */
int AnimatedLineEdit::labelFontSize() const
{
    return m_labelFontSize;
}

/** @brief Setter for labelFontSize property.
 *  @details Sets stylesheet with font size for label
 */
void AnimatedLineEdit::setLabelFontSize(int a_labelFontSize)
{
    QString fontSizeStylesheet = QString("#%1{font-size:%2px}")
                                            .arg(LABEL_OBJECT_NAME)
                                            .arg(a_labelFontSize);
    setStyleSheet(fontSizeStylesheet);

    m_labelFontSize = a_labelFontSize;
}

/** @brief Getter for labelColor property
 * @return labelColor
 */
QColor AnimatedLineEdit::labelColor() const
{
    return m_labelColor;
}

/** @brief Setter for labelColor property.
 *  @details Sets stylesheet with text color for label
 */
void AnimatedLineEdit::setLabelColor(const QColor &a_labelColor)
{
    m_label.setStyleSheet("color:" + a_labelColor.name());

    m_labelColor = a_labelColor;
}

/** @brief Get from stylesheet properties font-size and color for different states of label and assign to States.
 *
 *  @todo First search with #objectName. Then by class name
 */
void AnimatedLineEdit::updateLabelProperties()
{

    QString objName;
    if (!objectName().isEmpty()) {
        objName = "(?:#%1|.%2|\\s?%2)";
        objName = objName.arg(objectName());
    }
    else
        objName = ".%1";
    objName = objName.arg(metaObject()->className());

    //  Not focus properties ======================

    StyleSheatSearchPar labelSearchPar;
    labelSearchPar.widgetName = objName;
    labelSearchPar.subcontrol = "label";
    QString labelStyleSheet = AppStyleSheetHandler::getWidgetStyleSheet(labelSearchPar);
    QString strColor = AppStyleSheetHandler::getValueFromStylesheet(labelStyleSheet, "color");
    m_st_state_notFocus->assignProperty(this, "labelColor", QColor(strColor));

    //  Focus properties ======================

    labelSearchPar.pseudoClass = "focus";
    QString labelFocusStyleSheet = AppStyleSheetHandler::getWidgetStyleSheet(labelSearchPar);
    strColor = AppStyleSheetHandler::getValueFromStylesheet(labelFocusStyleSheet, "color");

    m_st_state_focus->assignProperty(this, "labelColor", QColor(strColor));

    // Filled properties ======================

    assignFontSizeFromStylesheet(labelStyleSheet, m_st_content_filled);

    // Empty properties ======================

    QString lineEditStyleSheet = AppStyleSheetHandler::getWidgetStyleSheet(objName);
    assignFontSizeFromStylesheet(lineEditStyleSheet, m_st_content_empty);
}

/** @brief Find in a_stylesheet the value of font-size property and assign it to a_state
 *  @param a_stylesheet Stylesheet withowt "{}"
 *  @param a_state state for what property will assign
 */
void AnimatedLineEdit::assignFontSizeFromStylesheet(const QString &a_stylesheet, QState *a_state)
{
    QString strFontSize = AppStyleSheetHandler::getValueFromStylesheet(a_stylesheet, "font-size");

    int fontSize = 0;

    //find number
    QRegularExpression regExp("(\\d+)");
    if (regExp.indexIn(strFontSize, 0) != -1)
        fontSize = regExp.cap(1).toInt();

    if (fontSize)
       a_state->assignProperty(this, "labelFontSize", fontSize);
    else
        qWarning()<< "font-size of AnimatedLineEdit is not found or incorrect";
}
