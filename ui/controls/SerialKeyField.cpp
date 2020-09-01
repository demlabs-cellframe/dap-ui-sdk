#include "SerialKeyField.h"

SerialKeyField::SerialKeyField(QWidget *parent)
        :CustomLineEdit(parent)
{
    m_regExp=QRegExp("[a-z"+QString(VALIDATOR)+"]"
                   "{0,"+QString::number(MAX_COUNT_CHAR)+"}");
    QValidator* validator=new QRegExpValidator(m_regExp);
    setValidator(validator);
    setMaxLength(MAX_COUNT_CHAR);
    setInputMethodHints(Qt::ImhLatinOnly|Qt::ImhNoPredictiveText|Qt::ImhPreferUppercase|Qt::ImhUppercaseOnly);
    connect(this, &SerialKeyField::pasteTextCaused,&SerialKeyField::pasteEvent);
    connect(this, &SerialKeyField::textChanged,[this](QString text)
    {
        if (this->text().count()==MAX_COUNT_CHAR)
            emit textChangedAndFilledOut(text);
        else if(this->text().isEmpty())
            emit textChangedAndCleaned();
    });
}

bool SerialKeyField::isFilledOut()
{
    if (this->text().count()==MAX_COUNT_CHAR) return true;
    else return false;
}

void SerialKeyField::paste()
{
    emit pasteTextCaused(QApplication::clipboard()->text());
}

void SerialKeyField::replaceNextCharacter(const QString &text)
{
    QString strText{this->text()};
    int cursotPposition{this->cursorPosition()};
    strText.remove(cursotPposition,1);
    strText.insert(cursotPposition,text);
#ifndef Q_OS_ANDROID
    strText=strText.toUpper();
#endif
    if(m_regExp.exactMatch(strText))
    {
        this->setText(strText);
        emit textEdited(strText);
        this->setCursorPosition(cursotPposition+1);
    }
}

void SerialKeyField::checkoutFilledOutOrCleaned()
{
    if (this->cursorPosition()==MAX_COUNT_CHAR)
        emit textEditedAndFilledOut(this->text());
    else if(this->text().isEmpty())
        emit textEditedAndCleaned();
}

void SerialKeyField::inputMethodEvent(QInputMethodEvent *e)
{
#ifdef Q_OS_ANDROID
    if(!e->commitString().isEmpty())
    {
        if(this->text().count()>this->cursorPosition()
                && !this->hasSelectedText())
        {
            replaceNextCharacter(e->commitString().toUpper());
            checkoutFilledOutOrCleaned();
            return;
        }
        if(m_regExp.exactMatch(e->commitString()))
            e->setCommitString(e->commitString().toUpper());
        else e->setCommitString("");
    }
    else if (this->text().isEmpty())
        return;
    QLineEdit::inputMethodEvent(e);
    checkoutFilledOutOrCleaned();
#else
    QLineEdit::inputMethodEvent(e);
#endif
}

void SerialKeyField::keyPressEvent(QKeyEvent *e)
{
    if(e==QKeySequence::Paste)
    {
        emit pasteTextCaused(QApplication::clipboard()->text());
        return;
    }
#ifdef Q_OS_ANDROID
    if(m_keys.contains(e->key())
            && this->text().count()>this->cursorPosition()
            && !this->hasSelectedText())
    {
        replaceNextCharacter(e->text());
    }
    else QLineEdit::keyPressEvent(e);
    checkoutFilledOutOrCleaned();
#else
    if(e->text().isEmpty())
    {
        QLineEdit::keyPressEvent(e);
        return;
    }

    if(this->text().count()>this->cursorPosition()
            && !this->hasSelectedText())
        replaceNextCharacter(e->text());
    else
    {
        QKeyEvent upperEvent(e->type(), e->key(),
                    e->modifiers(), e->text().toUpper(),
                    e->isAutoRepeat(), e->count());
        QLineEdit::keyPressEvent(&upperEvent);
    }
    checkoutFilledOutOrCleaned();
#endif
}

void SerialKeyField::pasteEvent(QString clipboardText)
{
    clipboardText=clipboardText.toUpper();
    clipboardText.replace("-","");
    clipboardText.replace(" ","");
    if(m_regExp.exactMatch(clipboardText))
    {
        this->setText(clipboardText);
        emit textEdited(clipboardText);
        checkoutFilledOutOrCleaned();
    }
}
