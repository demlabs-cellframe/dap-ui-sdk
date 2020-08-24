#include "CustomTextEdit.h"


CustomTextEdit::CustomTextEdit(QWidget *a_parent)
    :QTextEdit (a_parent)
{
    QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);

    connect(this, &QTextEdit::textChanged,[this]{
        this->setNewHeight(font(),toPlainText());
    });
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

    if(this->toPlainText().isEmpty())
    {
        if(m_placeHolderCtrl!=nullptr)
        {
            m_placeHolderCtrl->show();
            if(m_usingResizeableSize)
                setNewHeight(m_placeHolderCtrl->font(),m_placeHolderCtrl->text());
        }
        else
        {
            if(m_usingResizeableSize)
                setNewHeight(font(),placeholderText());
        }
    }
    else
    {
        if(m_usingResizeableSize)
            setNewHeight(font(),toPlainText());
    }

}


void CustomTextEdit::focusInEvent(QFocusEvent *e)
{
    if(m_placeHolderCtrl!=nullptr)
        m_placeHolderCtrl->hide();

    if(m_usingResizeableSize)
        setNewHeight(font(),toPlainText());

    Utils::setPropertyAndUpdateStyle(this, Properties::ACTIVE,true);
    QTextEdit::focusInEvent(e);

#ifdef Q_OS_ANDROID
    QApplication::inputMethod()->show();
#endif
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

void CustomTextEdit::setUsingResizableSize(bool a_using)
{
    m_usingResizeableSize = a_using;
}

void CustomTextEdit::resizeEvent(QResizeEvent *e)
{
    QTextEdit::resizeEvent(e);
    if(m_usingResizeableSize)
        if(e->oldSize().width() !=e->size().width())
        {
            if(this->toPlainText().isEmpty())
            {
                if(m_placeHolderCtrl!=nullptr)
                {
                    setNewHeight(m_placeHolderCtrl->font(),m_placeHolderCtrl->text());
                }
                else
                {
                    setNewHeight(font(),placeholderText());
                }
            }
            else
            {
                setNewHeight(font(),toPlainText());
            }
        }
}

void CustomTextEdit::inputMethodEvent(QInputMethodEvent *e)
{
    QTextEdit::inputMethodEvent(e);

    setNewHeight(font(), toPlainText() + e->preeditString());
 //   emit lineCount(toPlainText().length() + e->preeditString().length());
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

void CustomTextEdit::setNewHeight(const QFont &a_font, const QString &a_text)
{
    QFontMetrics textAnalized(a_font);

    int lineCount = qCeil(textAnalized.width(a_text)/static_cast<double>(this->width()));
    if(lineCount <= m_maxLineCount)
    {
        int height = ((lineCount - 1) * /*document()->size().height()*/textAnalized.height()) + m_normalHeight;
        if(height!=this->height())
        {
            this->setMinimumHeight(height);
            this->setMaximumHeight(height);
        }
    }

    if(lineCount >= m_maxLineCount && m_usingVerticalScrollBar)
    {
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    else
    {
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}

void CustomTextEdit::setUsingVerticalScrollBar(bool a_usingScrollBar)
{
    m_usingVerticalScrollBar = a_usingScrollBar;
}
