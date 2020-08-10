#include "CustomTextEdit.h"


CustomTextEdit::CustomTextEdit(QWidget *a_parent)
    :QTextEdit (a_parent)
{
    QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
}

void CustomTextEdit::createCustomPlaceholder()
{
    m_layoutCtrl = new QHBoxLayout(this);
    m_placeHolderCtrl = new QLabel(this);

    m_layoutCtrl->setMargin(0);
    m_layoutCtrl->setSpacing(0);
    m_layoutCtrl->addWidget(m_placeHolderCtrl);
    this->setLayout(m_layoutCtrl);
    m_placeHolderCtrl->setAlignment(Qt::AlignTop);
    m_placeHolderCtrl->setObjectName("placeholder");

    m_placeHolderCtrl->setWordWrap(true);

    setPlaceholderText(this->placeholderText());
}

void CustomTextEdit::setPlaceholderText(const QString &placeholderText)
{
    if(m_usingCustomPlaceholder)
    {
        m_placeHolderCtrl->setText(placeholderText);

        QTextEdit::setPlaceholderText(QString());
    }
    else
    {
        QTextEdit::setPlaceholderText(placeholderText);
    }
}

void CustomTextEdit::deleteCustomPlaceholder()
{
    if(m_placeHolderCtrl!=nullptr)
    {
        setPlaceholderText(m_placeHolderCtrl->text());
        delete m_placeHolderCtrl;
    }

    if(m_layoutCtrl!=nullptr)
        delete m_layoutCtrl;

}

void CustomTextEdit::focusOutEvent(QFocusEvent *e)
{
    Utils::setPropertyAndUpdateStyle(this, Properties::ACTIVE,false);

    QTextEdit::focusOutEvent(e);

    if(this->toPlainText().isEmpty())
    {
        if(m_placeHolderCtrl!=nullptr)
            m_placeHolderCtrl->show();
    }
}


void CustomTextEdit::focusInEvent(QFocusEvent *e)
{
    if(m_placeHolderCtrl!=nullptr)
        m_placeHolderCtrl->hide();

    Utils::setPropertyAndUpdateStyle(this, Properties::ACTIVE,true);
    QTextEdit::focusInEvent(e);

#ifdef Q_OS_ANDROID
    QApplication::inputMethod()->show();
#endif
}

void CustomTextEdit::setUsingCustomPlaceholder(bool a_usePlaceholder)
{
    if(m_usingCustomPlaceholder != a_usePlaceholder && a_usePlaceholder)
    {
        m_usingCustomPlaceholder = true;
        createCustomPlaceholder();
        return;
    }
    if(m_usingCustomPlaceholder != a_usePlaceholder && !a_usePlaceholder)
    {
        m_usingCustomPlaceholder = false;
        deleteCustomPlaceholder();
        return;
    }
}
