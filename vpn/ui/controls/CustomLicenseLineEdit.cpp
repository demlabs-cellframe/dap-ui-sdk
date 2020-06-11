#include "CustomLicenseLineEdit.h"

CustomLicenseLineEdit::CustomLicenseLineEdit(QWidget *parent) : QLineEdit(parent)
{

}

QString CustomLicenseLineEdit::text() const
{
    return QLineEdit::text().trimmed();
}

void CustomLicenseLineEdit::paste()
{
    pasteEvent();
}

void CustomLicenseLineEdit::keyPressEvent(QKeyEvent *event)
{
    int     key  = event->key();
    QString text = event->text().toUpper();

    if (event == QKeySequence::Paste) {
        pasteEvent();
        return;
    }

    if (!text.isEmpty()) {
        if (hasSelectedText()) {
            QRegExp re("\\s+");
            if (re.exactMatch(text) && (selectionStart() == 0)) {
                key  = Qt::Key_Delete;
                text = "";
            }
        } else if (text.trimmed().isEmpty() && (this->cursorPosition() == 0)) {
            return;
        }
    }

    QKeyEvent newEvent(
        event->type(),
        key,
        event->modifiers(),
        text,
        event->isAutoRepeat(),
        event->count());
    QLineEdit::keyPressEvent(&newEvent);
}

void CustomLicenseLineEdit::pasteEvent()
{
    QString inputBuffer = QApplication::clipboard()->text().toUpper();
    bool trim = false;

    inputBuffer.replace("-", "");

    if (this->cursorPosition() == 0)
        trim = true;
    if (hasSelectedText() && (this->selectionStart() == 0))
        trim = true;

    if (trim)
        inputBuffer.remove(QRegExp("^\\s+"));

    if(inputBuffer.length() == LICENSE_LENGTH) {
        emit sigPasteAllLicense(std::move(inputBuffer));
        return;
    }

    insert(inputBuffer);
}

#ifdef Q_OS_ANDROID
void CustomLicenseLineEdit::inputMethodEvent(QInputMethodEvent *e)
{
    if(e->preeditString().length() == this->maxLength())
    {
        QEvent event(QEvent::FocusOut);
        QApplication::sendEvent(this,&event);
    }

    QLineEdit::inputMethodEvent(e);
}

void CustomLicenseLineEdit::focusInEvent(QFocusEvent *e)
{
    if(e->reason() == Qt::MouseFocusReason)
        clear();

    emit onFocusLicense();

    QLineEdit::focusInEvent(e);
}

#endif
