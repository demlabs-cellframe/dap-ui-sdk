#include "CustomMessageBox.h"
#include "ui_CustomMessageBox.h"


CustomMessageBox::CustomMessageBox(QWidget *a_parent)
    :CustomMessageBox("",tr("Yes"),tr("No"),"", a_parent)
{

}

CustomMessageBox::CustomMessageBox(const QString &a_title, const QString &a_defaultButtonText,
                                   const QString &a_escapeButtonText, const QString &a_text, QWidget *a_parent)
    :CustomPopup(a_parent)
{
    this->create(m_ui);
    m_ui->lblTitle->setText(a_title);
    m_ui->btnDefault->setText(a_defaultButtonText);
    m_ui->btnEscape->setText(a_escapeButtonText);
    m_ui->lblText->setText(a_text);
}

CustomMessageBox::~CustomMessageBox()
{

}

void CustomMessageBox::setDefaultButtonText(const QString &a_defaultButtonText)
{
    m_ui->btnDefault->setText(a_defaultButtonText);
}

void CustomMessageBox::setEscapeButtonText(const QString &a_escapeButtonText)
{
    m_ui->btnEscape->setText(a_escapeButtonText);
}

void CustomMessageBox::setText(const QString &a_text)
{
    a_text.isEmpty() ? m_ui->lblText->hide() : m_ui->lblText->show();
    m_ui->lblText->setText(a_text);
}

void CustomMessageBox::setTitleText(const QString &a_title)
{
    m_ui->lblTitle->setText(a_title);
}

void CustomMessageBox::doDefaultAction()
{
    qDebug()<<__FUNCTION__;
    accept();
}

void CustomMessageBox::doEscapeAction()
{
    qDebug()<<__FUNCTION__;
    reject();
}

void CustomMessageBox::accept()
{
    this->hide();
    emit accepted();
}

void CustomMessageBox::reject()
{
    this->hide();
    emit rejected();
}

void CustomMessageBox::escape()
{
    doEscapeAction();
}

void CustomMessageBox::onClickedDefaultButton()
{
    doDefaultAction();
}

void CustomMessageBox::onClickedEscapeButton()
{
    doEscapeAction();
}

void CustomMessageBox::closeEvent(QCloseEvent *)
{
    escape();
}

void CustomMessageBox::initVariantUi(QWidget *)
{
    m_ui->lblTitle->setObjectName("lblTitle");
    m_ui->btnDefault->setObjectName("btnDefault");
    m_ui->btnEscape->setObjectName("btnEscape");
    m_ui->lblText->setObjectName("lblText");
    m_ui->frmCustomMessageBox->setObjectName("frmMessageBox");

    m_ui->lblTitle->setAlignment(Qt::AlignCenter);
    m_ui->lblText->setAlignment(Qt::AlignCenter);

    connect(m_ui->btnDefault,SIGNAL(clicked()),this, SLOT(onClickedDefaultButton()));
    connect(m_ui->btnEscape,SIGNAL(clicked()),this, SLOT(onClickedEscapeButton()));
}

