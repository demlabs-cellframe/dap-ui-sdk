#include "SerialRemovalConfirmationMessage.h"
#include <QKeyEvent>

SerialRemovalConfirmationMessage::SerialRemovalConfirmationMessage(QWidget *a_parent)
    :CustomMessageBox(tr("Reset serial number?"), tr("CANCEL"),tr("RESET"),"",a_parent)
{

}

void SerialRemovalConfirmationMessage::closeEvent(QCloseEvent *)
{
    if (keyboardGrabber() == this) releaseKeyboard();
    doDefaultAction();
}

void SerialRemovalConfirmationMessage::hideEvent(QHideEvent *e)
{
    if (keyboardGrabber() == this) releaseKeyboard();
    CustomMessageBox::hideEvent(e);
}

void SerialRemovalConfirmationMessage::showEvent(QShowEvent *e)
{
    if (keyboardGrabber() != this) grabKeyboard();
    CustomMessageBox::showEvent(e);
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

void SerialRemovalConfirmationMessage::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
        doEscapeAction();
    else CustomMessageBox::keyPressEvent(e);
}
