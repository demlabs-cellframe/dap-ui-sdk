#include "CustomTextEdit.h"


CustomTextEdit::CustomTextEdit(QWidget *a_parent)
    :QTextEdit (a_parent)
{
    QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
}

void CustomTextEdit::recreateCustomTextEdit()
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

    if(!placeholderText().isEmpty())
    {
        m_placeHolderCtrl->setText(placeholderText());
        setPlaceholderText(QString());
    }
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

void CustomTextEdit::setUseCustomPlaceholder(bool a_usePlaceholder)
{
    if(m_useCustomPlaceholder != a_usePlaceholder && a_usePlaceholder)
    {
        m_useCustomPlaceholder = a_usePlaceholder;
        recreateCustomTextEdit();
    }

}
