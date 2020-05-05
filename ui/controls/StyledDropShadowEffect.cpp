#include "StyledDropShadowEffect.h"
#include "AppStyleSheetHandler.h"
#include "../Utils.h"

StyledDropShadowEffect::StyledDropShadowEffect(QObject *a_parent /*= Q_NULLPTR*/)
    :QGraphicsDropShadowEffect(a_parent)
{
    this->updateStyle();
}

void StyledDropShadowEffect::updateStyle()
{
    StyleSheatSearchPar searchPar;
    searchPar.widgetName = "#" + parent()->objectName();
    searchPar.subcontrol = "Shadow";

    QString stylesheet = AppStyleSheetHandler::getWidgetStyleSheet(searchPar);

    //Color:
    QString colorStr = AppStyleSheetHandler::getValueFromStylesheet(stylesheet, "color");
    if (colorStr.isEmpty())
        this->setColor(QColor(0,0,0,0));
    else
        this->setColor(Utils::toColor(colorStr));

    //Offset:
    int offsetX = AppStyleSheetHandler::getValueFromStylesheet(stylesheet, "offset-x").toInt();
    int offsetY = AppStyleSheetHandler::getValueFromStylesheet(stylesheet, "offset-y").toInt();
    this->setOffset(offsetX, offsetY);
}
