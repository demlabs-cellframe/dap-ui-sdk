#include "SerialRemovalConfirmationMessage.h"

SerialRemovalConfirmationMessage::SerialRemovalConfirmationMessage(QWidget *a_parent)
    :CustomMessageBox(tr("Reset serial number?"), tr("CANCEL"),tr("RESET"),"",a_parent)
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
