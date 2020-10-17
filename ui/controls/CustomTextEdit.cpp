#include "CustomTextEdit.h"


CustomTextEdit::CustomTextEdit(QWidget *a_parent)
    :QTextEdit (a_parent)
{
    QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);

//    connect(this->document()->documentLayout(),&QAbstractTextDocumentLayout::documentSizeChanged, [=](const QSizeF &newSize){
//        qWarning()<<"New size Document \n"<< newSize<<"normal height = "<<document()->lineCount();
//        if(m_autoChangingSize)
//            this->setNewHeight(newSize);
//    });
#ifdef Q_OS_ANDROID
    connect(this, &CustomTextEdit::textChanged,this,[this]()
    {
        sendNumberOfCharacters(toPlainText().length());
    });
#endif
}

void CustomTextEdit::createCustomPlaceholder()
{
    m_layoutCtrl = new QHBoxLayout(this);
    m_placeHolderCtrl = new QLabel(this);

    m_layoutCtrl->setMargin(0);
    m_layoutCtrl->setSpacing(0);
    m_layoutCtrl->addWidget(m_placeHolderCtrl);
    this->setLayout(m_layoutCtrl);
    m_placeHolderCtrl->setAlignment(Qt::AlignTop);
    m_placeHolderCtrl->setObjectName("placeholder");

    m_placeHolderCtrl->setWordWrap(true);
    m_placeHolderCtrl->setVisible(this->toPlainText().isEmpty());

    setPlaceholderText(this->placeholderText());
}

void CustomTextEdit::setPlaceholderText(const QString &placeholderText)
{
    if(m_usingCustomPlaceholder)
    {
        m_placeHolderCtrl->setText(placeholderText);

        QTextEdit::setPlaceholderText(QString());
    }
    else
    {
        QTextEdit::setPlaceholderText(placeholderText);
    }
}

void CustomTextEdit::deleteCustomPlaceholder()
{
    if(m_placeHolderCtrl!=nullptr)
    {
        setPlaceholderText(m_placeHolderCtrl->text());
        delete m_placeHolderCtrl;
    }

    if(m_layoutCtrl!=nullptr)
        delete m_layoutCtrl;

}

void CustomTextEdit::focusOutEvent(QFocusEvent *e)
{
    Utils::setPropertyAndUpdateStyle(this, Properties::ACTIVE,false);

    QTextEdit::focusOutEvent(e);

#ifdef Q_OS_ANDROID
    emit focusChanged(true);
    QApplication::inputMethod()->hide();
#endif
    if(this->toPlainText().isEmpty())
    {
        if(m_placeHolderCtrl!=nullptr)
        {
            m_placeHolderCtrl->show();
        }
    }

    if(m_autoChangingSize)
        sizingCondition();
}


void CustomTextEdit::focusInEvent(QFocusEvent *e)
{
    QTextEdit::focusInEvent(e);

    if(m_placeHolderCtrl!=nullptr)
        m_placeHolderCtrl->hide();

    Utils::setPropertyAndUpdateStyle(this, Properties::ACTIVE,true);

#ifdef Q_OS_ANDROID
    emit focusChanged(false);
    QApplication::inputMethod()->show();
#endif

    if(m_autoChangingSize)
    {
        if(toPlainText().isEmpty() && m_placeHolderCtrl == nullptr)
        {
            setNewHeightForPlaceholder(font(),placeholderText());
        }
        else
        {
            setNewHeight(document()->documentLayout()->documentSize());
        }
    }
}

void CustomTextEdit::setUsingCustomPlaceholder(bool a_usingPlaceholder)
{
    if (m_usingCustomPlaceholder == a_usingPlaceholder)
        return;
    m_usingCustomPlaceholder = a_usingPlaceholder;

    if(a_usingPlaceholder)
        createCustomPlaceholder();

    else
        deleteCustomPlaceholder();
}

void CustomTextEdit::setAutoChangingSize(bool a_using)
{
    m_autoChangingSize = a_using;
}

void CustomTextEdit::resizeEvent(QResizeEvent *e)
{

    if(m_autoChangingSize)
    {
        if(e->oldSize().width() !=e->size().width())
        {
            if(this->toPlainText().isEmpty())
            {
                m_normalLineHeight = document()->size().height();
           }
            sizingCondition();
        }
    }
    QTextEdit::resizeEvent(e);
}

void CustomTextEdit::inputMethodEvent(QInputMethodEvent *e)
{
    if(e->commitString()=="\n" && m_redefineEnterButton)
    {
        emit pressedEnter();
    }
    else
        QTextEdit::inputMethodEvent(e);



    if(m_autoChangingSize)
    {
        if(toPlainText().isEmpty()&&e->preeditString().isEmpty() && m_placeHolderCtrl == nullptr)
        {
            setNewHeightForPlaceholder(font(),placeholderText());
        }
        else
        {
            setNewHeight(document()->documentLayout()->documentSize());
        }
    }
#ifdef Q_OS_ANDROID
    sendNumberOfCharacters(toPlainText().length() + e->preeditString().length());
#endif
}

void CustomTextEdit::setNormalHeight(const QString &a_heightStr)
{
    int height = a_heightStr.split("px")[0].toInt();
    m_normalHeight = height;
}

void CustomTextEdit::setMaximimLineCount(int a_maxLine)
{
    m_maxLineCount = a_maxLine;
}

void CustomTextEdit::setNewHeightForPlaceholder(const QFont &a_font, const QString &a_text)
{
    QFontMetrics textAnalized(a_font);

    int lineCount = qCeil(textAnalized.width(a_text)/static_cast<double>(this->width() - textAnalized.width("w")));
    int height = 0;
    if(lineCount <= m_maxLineCount)
    {
        height = static_cast<int>(((lineCount - 1) * m_normalLineHeight) + m_normalHeight);
    }
    else
    {
        height = static_cast<int>(((m_maxLineCount - 1) * m_normalLineHeight) + m_normalHeight);
    }

    if(height!=this->height())
    {
        this->setMinimumHeight(height);
        this->setMaximumHeight(height);
    }
}

void CustomTextEdit::setNewHeight(const QSizeF &newSize)
{
    int height = 0;
    if(newSize.height() <= m_normalLineHeight)
        height = m_normalHeight;
    else {
        height = static_cast<int>(newSize.height() - m_normalLineHeight + m_normalHeight);
    }

    if(height/m_maxLineCount>m_normalLineHeight)
        height = static_cast<int>(((m_maxLineCount - 1) * m_normalLineHeight) + m_normalHeight);

    this->setMinimumHeight(height);
    this->setMaximumHeight(height);
}

void CustomTextEdit::keyPressEvent(QKeyEvent *e)
{
    if(e->text()=="\n" && m_redefineEnterButton)
        emit pressedEnter();
    else
        QTextEdit::keyPressEvent(e);

    if(m_autoChangingSize)
    {
        if(toPlainText().isEmpty() && m_placeHolderCtrl == nullptr)
        {
            setNewHeightForPlaceholder(font(),placeholderText());
        }
        else
        {
            setNewHeight(document()->documentLayout()->documentSize());
        }
    }
#ifdef Q_OS_ANDROID
    sendNumberOfCharacters(toPlainText().length());
#endif
}

void CustomTextEdit::sizingCondition()
{
    if(this->toPlainText().isEmpty())
    {
        if(m_placeHolderCtrl!=nullptr)
        {
            setNewHeightForPlaceholder(m_placeHolderCtrl->font(),m_placeHolderCtrl->text());
        }
        else
        {
            setNewHeightForPlaceholder(font(),placeholderText());
        }
    }
    else
    {
        setNewHeight(document()->documentLayout()->documentSize());
    }
}

void CustomTextEdit::setRedefineEnterButton(bool a_redefine)
{
    m_redefineEnterButton = a_redefine;
}

void CustomTextEdit::sendNumberOfCharacters(int a_number)
{
    if(a_number != m_numberCharacter)
    {
        m_numberCharacter = a_number;
        emit numberCharacterChange(a_number);
    }
}
