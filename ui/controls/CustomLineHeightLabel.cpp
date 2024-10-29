#include "CustomLineHeightLabel.h"
#include "AppStyleSheetHandler.h"

CustomLineHeightLabel::CustomLineHeightLabel(QWidget *a_parent)
    :QLabel (a_parent)
{
}

void CustomLineHeightLabel::setText(const QString &text)
{
    if(m_lineHeight.isEmpty())m_lineHeight = "100";
    QString textToHtml = QString("<p style = 'line-height:%1;'> %2 </p>").arg(m_lineHeight).arg(text);

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
    searchPar.widgetName = "#" + this->objectName();

    QString stylesheet = AppStyleSheetHandler::getWidgetStyleSheet(searchPar);
    //line-height:
    m_lineHeight = AppStyleSheetHandler::getValueFromStylesheet(stylesheet, "line-height");

}
