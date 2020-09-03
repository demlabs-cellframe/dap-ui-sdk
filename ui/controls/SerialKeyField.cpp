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
    connect(this, &SerialKeyField::textEdited,[this](QString text)
    {
        Q_UNUSED(text);
        if (this->text().count()==MAX_COUNT_CHAR)
            emit textEditedAndFilledOut(this->text());
        else if(this->text().isEmpty())
            emit textEditedAndCleaned();
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

void SerialKeyField::inputMethodEvent(QInputMethodEvent *e)
{
#ifdef Q_OS_ANDROID
    if(!e->commitString().isEmpty())
    {
        if(!this->hasSelectedText()
                && this->text().count()>this->cursorPosition())
        {
            replaceNextCharacter(e->commitString().toUpper());
            return;
        }

        else if (this->hasSelectedText()
                 && this->text().count() > this->cursorPosition())
        {
            int newCursorPosition{this->cursorPosition() - this->selectedText().count() + 1};
            if(m_regExp.exactMatch(e->commitString()))
                e->setCommitString(e->commitString().toUpper());
            else e->setCommitString("");
            QLineEdit::inputMethodEvent(e);
            this->setCursorPosition(newCursorPosition);
            return;
        }

        if(m_regExp.exactMatch(e->commitString()))
            e->setCommitString(e->commitString().toUpper());
        else
            e->setCommitString("");
    } 
    else
    {
        if (this->text().isEmpty()) return;

        if (this->text().count() > this->cursorPosition())
        {
            static int newCursorPosition{};
            static bool selected{};
            if (this->hasSelectedText())
            {
                selected=true;
                newCursorPosition=this->selectionStart();
            }
            else
            {
                if (selected){}
                else newCursorPosition=this->cursorPosition()-1;
                selected=false;
            }
            int countText{this->text().count()};
            QLineEdit::inputMethodEvent(e);
            if(this->text().count()<countText) this->setCursorPosition(newCursorPosition);
            return;
        }
    }

    QString thisText{this->text()};
    QLineEdit::inputMethodEvent(e);
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
            && this->text().count()>this->cursorPosition())
    {
        if(!this->hasSelectedText()) replaceNextCharacter(e->text());
        else
        {
            int newCursorPosition{this->cursorPosition()-this->selectedText().count()+1};
            QLineEdit::keyPressEvent(e);
            this->setCursorPosition(newCursorPosition);
        }
    }
    else if(e->key()==Qt::Key_Backspace
            && this->text().count()>this->cursorPosition())
    {
        int newCursorPosition{};
        this->hasSelectedText() ? newCursorPosition=this->selectionStart()
                : newCursorPosition=this->cursorPosition()-1;
        QLineEdit::keyPressEvent(e);
        this->setCursorPosition(newCursorPosition);
    }
    else QLineEdit::keyPressEvent(e);
#else
    if(e->text().isEmpty())
    {
        QLineEdit::keyPressEvent(e);
        return;
    }

    if(this->text().count()>this->cursorPosition())
    {
        if(!this->hasSelectedText())
        {
            if(e->key()==Qt::Key_Backspace
                    || e->key()==Qt::Key_Delete)
            {
                int newCursorPosition{this->cursorPosition()};
                QLineEdit::keyPressEvent(e);
                e->key()==Qt::Key_Backspace ? this->setCursorPosition(newCursorPosition-1)
                                            : this->setCursorPosition(newCursorPosition);
            }
            else replaceNextCharacter(e->text());
        }

        else
        {
            if(e->key()==Qt::Key_Backspace
                    || e->key()==Qt::Key_Delete)
            {
                int newCursorPosition{this->selectionStart()};
                QLineEdit::keyPressEvent(e);
                this->setCursorPosition(newCursorPosition);
            }
            else
            {
                int newCursorPosition{this->cursorPosition()-this->selectedText().count()+1};
                QKeyEvent upperEvent(e->type(), e->key(),
                            e->modifiers(), e->text().toUpper(),
                            e->isAutoRepeat(), e->count());
                QLineEdit::keyPressEvent(&upperEvent);
                this->setCursorPosition(newCursorPosition);
            }
        }
    }

    else
    {
        QKeyEvent upperEvent(e->type(), e->key(),
                    e->modifiers(), e->text().toUpper(),
                    e->isAutoRepeat(), e->count());
        QLineEdit::keyPressEvent(&upperEvent);
    }
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
    }
}
