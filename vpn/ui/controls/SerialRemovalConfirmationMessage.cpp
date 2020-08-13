#include "SerialRemovalConfirmationMessage.h"

SerialRemovalConfirmationMessage::SerialRemovalConfirmationMessage(QWidget *parent)
    :CustomMessageBox(parent)
{

}

SerialRemovalConfirmationMessage::SerialRemovalConfirmationMessage(const QString &a_title,
     const QString &a_defaultButtonText, const QString &a_escapeButtonText,
     const QString &a_text, QWidget *a_parent)
    : CustomMessageBox(a_title,a_defaultButtonText,a_escapeButtonText,a_text,a_parent)
{

}

void SerialRemovalConfirmationMessage::escape()
{
    doDefaultAction();
    qDebug()<<__FUNCTION__;
}

void SerialRemovalConfirmationMessage::closeEvent(QCloseEvent *)
{
    escape();
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
