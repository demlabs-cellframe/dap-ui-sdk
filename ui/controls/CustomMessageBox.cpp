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
    a_defaultButtonText.isEmpty() ? hideButton(m_ui->btnDefault)
                                  : m_ui->btnDefault->setText(a_defaultButtonText);
    a_escapeButtonText.isEmpty() ? hideButton(m_ui->btnEscape)
                                 : m_ui->btnEscape->setText(a_escapeButtonText);
    m_ui->lblText->setText(a_text);
}

CustomMessageBox::~CustomMessageBox()
{

}

void CustomMessageBox::setDefaultButtonText(const QString &a_defaultButtonText)
{ 
    a_defaultButtonText.isEmpty() ? hideButton(m_ui->btnDefault)
                                  : showButton(m_ui->btnDefault);
    m_ui->btnDefault->setText(a_defaultButtonText);
}

void CustomMessageBox::setEscapeButtonText(const QString &a_escapeButtonText)
{
    a_escapeButtonText.isEmpty() ? hideButton(m_ui->btnEscape)
                                 : showButton(m_ui->btnEscape);
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
    doEscapeAction();
}

void CustomMessageBox::initVariantUi(QWidget *)
{
    connect(m_ui->btnDefault,SIGNAL(clicked()),this, SLOT(onClickedDefaultButton()));
    connect(m_ui->btnEscape,SIGNAL(clicked()),this, SLOT(onClickedEscapeButton()));
}

void CustomMessageBox::hideButton(QPushButton *btn)
{
    btn->hide();
    m_ui->horizontalSpacer->changeSize(0,0);
}

void CustomMessageBox::showButton(QPushButton *btn)
{
    btn->show();
    if(m_ui->btnDefault->isHidden()||m_ui->btnEscape->isHidden())
        m_ui->horizontalSpacer->changeSize(0,0);
    else m_ui->horizontalSpacer->changeSize(0,0,QSizePolicy::Expanding);
}


