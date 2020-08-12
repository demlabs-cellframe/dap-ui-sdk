#include "CustomMessageBox.h"
#include "ui_CustomMessageBox.h"


CustomMessageBox::CustomMessageBox(QWidget *a_parent)
    :CustomPopup(a_parent)
{
    this->create(m_ui);
}

CustomMessageBox::CustomMessageBox(const QString &a_title, const QString &a_defaultButtonText,
                                   const QString &a_escapeButtonText, const QString &a_text, QWidget *a_parent)
    :CustomPopup(a_parent),
      m_title(a_title),
      m_defaultButtonText(a_defaultButtonText),
      m_escapeButtonText(a_escapeButtonText),
      m_text(a_text)
{
    this->create(m_ui);
}

void CustomMessageBox::setDefaultButtonText(const QString &a_defaultButtonText)
{
    m_defaultButtonText=a_defaultButtonText;
    m_ui->btnDefault->setText(m_defaultButtonText);
}

void CustomMessageBox::setEscapeButtonText(const QString &a_escapeButtonText)
{
    m_escapeButtonText=a_escapeButtonText;
    m_ui->btnEscape->setText(m_escapeButtonText);
}

void CustomMessageBox::setText(const QString &a_text)
{
    m_text=a_text;
    m_ui->lblText->setText(m_text);
}

void CustomMessageBox::setTitleText(const QString &a_title)
{
    m_title=a_title;
    m_ui->lblTitle->setText(m_title);
}

void CustomMessageBox::doDefaultAction()
{
    qDebug()<<__FUNCTION__;
}

void CustomMessageBox::doEscapeAction()
{
    qDebug()<<__FUNCTION__;
}

void CustomMessageBox::accept()
{
    this->hide();
    doDefaultAction();
}

void CustomMessageBox::reject()
{
    this->hide();
    doEscapeAction();
}

void CustomMessageBox::escape()
{
    this->hide();
    doEscapeAction();
}

void CustomMessageBox::onClickedDefaultButton()
{
    emit accepted();
}

void CustomMessageBox::onClickedEscapeButton()
{
    emit rejected();
}

void CustomMessageBox::closeEvent(QCloseEvent *)
{
    escape();
}

void CustomMessageBox::initVariantUi(QWidget *a_widget)
{
    m_ui->lblTitle->setText(m_title);
    m_ui->btnDefault->setText(m_defaultButtonText);
    m_ui->btnEscape->setText(m_escapeButtonText);
    m_ui->lblText->setText(m_text);
    connect(m_ui->btnDefault,SIGNAL(clicked()),this, SLOT(onClickedDefaultButton()));
    connect(m_ui->btnEscape,SIGNAL(clicked()),this, SLOT(onClickedEscapeButton()));
    connect(this,SIGNAL(accepted()),SLOT(accept()));
    connect(this,SIGNAL(rejected()),SLOT(reject()));
}

