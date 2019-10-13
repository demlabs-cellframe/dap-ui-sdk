#include "StyledDropShadowEffect.h"
#include "DapStyleHolder.h"
#include "Utils.h"

StyledDropShadowEffect::StyledDropShadowEffect(QObject *a_parent /*= Q_NULLPTR*/)
    :QGraphicsDropShadowEffect(a_parent)
{
    this->updateStyle();
}

void StyledDropShadowEffect::updateStyle()
{
    DapStyleHolder *styleHolder = DapStyleHolder::qAppCssStyleHolder();

    StyleSheatSearchPar searchPar;
    searchPar.widgetName = "#" + parent()->objectName();
    searchPar.subcontrol = "Shadow";

    QString stylesheet = styleHolder->getWidgetStyleSheet(searchPar);

    //Color:
    QString colorStr = styleHolder->getValueFromStylesheet(stylesheet, "color");
    if (colorStr.isEmpty())
        this->setColor(QColor(0,0,0,0));
    else
        this->setColor(Utils::toColor(colorStr));

    //Offset:
    int offsetX = styleHolder->getValueFromStylesheet(stylesheet, "offset-x").toInt();
    int offsetY = styleHolder->getValueFromStylesheet(stylesheet, "offset-y").toInt();
    this->setOffset(offsetX, offsetY);
}
