#include "CustomLineHeightTextEdit.h"


CustomLineHeightTextEdit::CustomLineHeightTextEdit(QWidget *a_parent)
    :QTextEdit (a_parent)
{
}

void CustomLineHeightTextEdit::setPlaceholderText(const QString &placeholderText)
{
    Utils::setPropertyAndUpdateStyle(this,Properties::STATE,STATE_DEFAULT);

    m_placeholderText = placeholderText;
    QTextEdit::setText(textToHtml(placeholderText));
    //setReadOnly(true);
}

void CustomLineHeightTextEdit::mousePressEvent(QMouseEvent *e)
{
    if(property("state").toString()==STATE_DEFAULT)
    {
        Utils::setPropertyAndUpdateStyle(this,Properties::STATE,STATE_HOVER);

        QString htmlText = QString("<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.0//EN' 'http://www.w3.org/TR/REC-html40/strict.dtd'>"
                                   "<html><head><meta name='qrichtext' content='1' /><style type='text/css'>"
                                   "p, li { white-space: pre-wrap; }"
                                   "</style></head><body>"
                                   "<p style='line-height:%1;'></p></body></html>").arg(m_lineHeight);

        QTextEdit::setText(htmlText);
    }
    QTextEdit::mousePressEvent(e);
}

void CustomLineHeightTextEdit::focusOutEvent(QFocusEvent *e)
{
    if(toPlainText().isEmpty())
    {
        Utils::setPropertyAndUpdateStyle(this,Properties::STATE,STATE_DEFAULT);

        QTextEdit::setText(textToHtml(m_placeholderText));
    }
    else
    {
        Utils::setPropertyAndUpdateStyle(this,Properties::STATE,STATE_END_EDIT);

    }
    QTextEdit::focusOutEvent(e);
}

QString CustomLineHeightTextEdit::toPlainText() const
{
    if(property("state").toString()==STATE_DEFAULT)
        return QString("");

    return QTextEdit::toPlainText();

}

void CustomLineHeightTextEdit::setObjectName(const QString &name)
{
    QObject::setObjectName(name);

    updateStyleSheets();
}

void CustomLineHeightTextEdit::updateStyleSheets()
{
    StyleSheatSearchPar searchPar;
    searchPar.widgetName = "#" + this->objectName();

    QString stylesheet = AppStyleSheetHandler::getWidgetStyleSheet(searchPar);
    m_lineHeight = AppStyleSheetHandler::getValueFromStylesheet(stylesheet, "line-height");
}

QString CustomLineHeightTextEdit::textToHtml(const QString &text)
{
    if(m_lineHeight.isEmpty())m_lineHeight = "100";
    return QString("<p style = 'line-height:%1;'> %2 </p>").arg(m_lineHeight).arg(text);
}
