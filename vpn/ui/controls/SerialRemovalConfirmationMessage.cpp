#include "SerialRemovalConfirmationMessage.h"

SerialRemovalConfirmationMessage::SerialRemovalConfirmationMessage(QWidget *a_parent)
    :CustomMessageBox(tr("Reset serial number?"), tr("CANCEL"),tr("RESET"),"",a_parent)
{

}

SerialRemovalConfirmationMessage::SerialRemovalConfirmationMessage(const QString& a_title, const QString& a_defaultButtonText,
                 const QString& a_escapeButtonText, const QString& a_text,
                 QWidget *a_parent)
    :CustomMessageBox(a_title, a_defaultButtonText,a_escapeButtonText,a_text,a_parent)
{

}

void SerialRemovalConfirmationMessage::closeEvent(QCloseEvent *)
{
    doDefaultAction();
}

void SerialRemovalConfirmationMessage::doEscapeAction()
{
    accept();
    qDebug()<<__FUNCTION__;
}

void SerialRemovalConfirmationMessage::doDefaultAction()
{
    reject();
    qDebug()<<__FUNCTION__;
}
