#include "StyledDropShadowEffect.h"
#include "AppStyleSheetHandler.h"
#include "Utils.h"

/** @brief constructor. Update style
 *  @param a_parent object parent
*/
StyledDropShadowEffect::StyledDropShadowEffect(QObject *a_parent /*= Q_NULLPTR*/)
    :QGraphicsDropShadowEffect(a_parent)
{
    this->updateStyle();
}

/** @brief get stylesheet from app and update shadow style
*/
void StyledDropShadowEffect::updateStyle()
{
    StyleSheatSearchPar searchPar;
    searchPar.widgetName = "#" + parent()->objectName();
    searchPar.subcontrol = "shadow";

    QString stylesheet = AppStyleSheetHandler::getWidgetStyleSheet(searchPar);

    //Color:
    QString colorStr = AppStyleSheetHandler::getValueFromStylesheet(stylesheet, "color");
    if (colorStr.isEmpty())
        this->setColor(QColor(0,0,0,0));
    else
        this->setColor(Utils::toColor(colorStr));

    int blur = Utils::toIntValue(AppStyleSheetHandler::getValueFromStylesheet(stylesheet, "blur"));
    this->setBlurRadius(blur);

    //Offset:
    int x = Utils::toIntValue(AppStyleSheetHandler::getValueFromStylesheet(stylesheet, "x"));
    int y = Utils::toIntValue(AppStyleSheetHandler::getValueFromStylesheet(stylesheet, "y"));

    this->setOffset(x, y);
}
