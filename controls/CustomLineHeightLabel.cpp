#include "CustomLineHeightLabel.h"
#include "AppStyleSheetHandler.h"

CustomLineHeightLabel::CustomLineHeightLabel(QWidget *a_parent)
    :QLabel (a_parent)
{
}

void CustomLineHeightLabel::setText(const QString &text)
{
    if(m_HightLine.isEmpty())m_HightLine = "100";
    QString textToHtml = QString("<p style = 'line-height:%1;'>").arg(m_HightLine);
    textToHtml += text;
    textToHtml += "</p>";

    QLabel::setText(textToHtml);

}

void CustomLineHeightLabel::setObjectName(const QString &name)
{
    QObject::setObjectName(name);

    updateStyleSheets();
}

void CustomLineHeightLabel::updateStyleSheets()
{

    StyleSheatSearchPar searchPar;
    searchPar.widgetName = "#"+ parent()->objectName() + " #" + this->objectName();

    QString stylesheet = AppStyleSheetHandler::getWidgetStyleSheet(searchPar);
    //line-height:
    m_HightLine = AppStyleSheetHandler::getValueFromStylesheet(stylesheet, "line-height");

}
