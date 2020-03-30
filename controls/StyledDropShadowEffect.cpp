#include "StyledDropShadowEffect.h"
#include "AppStyleSheetHandler.h"
#include "Utils.h"
#include <QWidget>

StyledDropShadowEffect::StyledDropShadowEffect(QObject *a_parent /*= Q_NULLPTR*/)
    :QGraphicsDropShadowEffect(a_parent)
{
    updateStyleProperties();
    installEventFilter(this);
}

///@details Collecting data from css
void StyledDropShadowEffect::updateStyleProperties()
{
    StyleSheatSearchPar searchPar;
    searchPar.widgetName = "#" + parent()->objectName();
    searchPar.subcontrol = "shadow";
    setDataShadowProperties(AppStyleSheetHandler::getWidgetStyleSheet(searchPar),defaultShadow);

    updateStyle(DEFAULT_SHADOW);

    searchPar.pseudoClass = "hover";
    setDataShadowProperties(AppStyleSheetHandler::getWidgetStyleSheet(searchPar),hoverShadow);
}

///@details Setting and setting the default shadow
void StyledDropShadowEffect::updateStyle(StyleShedow a_style)
{
    switch (a_style)
    {
    case DEFAULT_SHADOW:setShadowProperties(defaultShadow);break;
    case HOVER_SHADOW:setShadowProperties(hoverShadow);break;
    default:break;
    }

    this->update();
//    qobject_cast<QWidget*>(this->parent())->setGraphicsEffect(this);
}

///@details Saving data to a shadow structure
/// @param  a_property String with settings from css.
/// @param data Data structure with shadow settings.
void StyledDropShadowEffect::setDataShadowProperties(const QString &stylesheet, ShadowProperties &data)
{
    //Color:
    QString colorStr = AppStyleSheetHandler::getValueFromStylesheet(stylesheet, "color");
    if (colorStr.isEmpty())
        data.color = QColor(0,0,0,0);
    else
        data.color = Utils::toColor(colorStr);
    data.blur = Utils::toIntValue(AppStyleSheetHandler::getValueFromStylesheet(stylesheet, "blur"));
    //Offset:
    data.x = Utils::toIntValue(AppStyleSheetHandler::getValueFromStylesheet(stylesheet, "x"));
    data.y = Utils::toIntValue(AppStyleSheetHandler::getValueFromStylesheet(stylesheet, "y"));
}

///@details Set shadow options
/// @param data Data structure with shadow settings.
void StyledDropShadowEffect::setShadowProperties(ShadowProperties &data)
{
    this->setColor(data.color);
    this->setBlurRadius(data.blur);
    this->setOffset(data.x, data.y);
}

bool StyledDropShadowEffect::eventFilter(QObject *watched, QEvent *event)
{
    //if (event->type() == QEvent::KeyPress) {
        qDebug()<<"Opa opa!!!!!!!!!!!!!!!!!!";
    //}
        return QObject::eventFilter(watched,event);
}

void StyledDropShadowEffect::customEvent(QEvent *event)
{
    //if (event->type() == QEvent::KeyPress) {
        qDebug()<<"Opa opa!!!!!!!!!!!!!!!!!!";
    //}
        return QObject::customEvent(event);
}
