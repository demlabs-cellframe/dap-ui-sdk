#include "StyledDropShadowEffect.h"
#include "AppStyleSheetHandler.h"
#include "Utils.h"
#include <QWidget>

StyledDropShadowEffect::StyledDropShadowEffect(QObject *a_parent)
    :QGraphicsDropShadowEffect(a_parent)
{
    defaultShadow = nullptr;
    hoverShadow = nullptr;

    updateStyleProperties();
    a_parent->installEventFilter(this);

    connectToParentEvents();

}
StyledDropShadowEffect::~StyledDropShadowEffect()
{
    delete defaultShadow;
    delete hoverShadow;
}

///@details Connects connections
void StyledDropShadowEffect::connectToParentEvents()
{
    connect(this,&StyledDropShadowEffect::mouseEnter,[=]{
        this->updateStyle(ShadowState::HOVER_SHADOW);
    });

    connect(this,&StyledDropShadowEffect::mouseLaeve,[=]{
        this->updateStyle(ShadowState::DEFAULT_SHADOW);
    });

    connect(this->parent(),&QObject::objectNameChanged,[=]{
        this->updateStyleProperties();
    });
}

///@details Collecting data from css
void StyledDropShadowEffect::updateStyleProperties()
{
    StyleSheatSearchPar searchPar;
    searchPar.widgetName = "#" + parent()->objectName();
    searchPar.subcontrol = "shadow";
    if(AppStyleSheetHandler::getWidgetStyleSheet(searchPar) !="")
    {
        if(!defaultShadow)
            defaultShadow = new ShadowProperties();
        setDataShadowProperties(AppStyleSheetHandler::getWidgetStyleSheet(searchPar),defaultShadow);
    }
    updateStyle(DEFAULT_SHADOW);

    searchPar.pseudoClass = "hover";
    if(AppStyleSheetHandler::getWidgetStyleSheet(searchPar) !="")
    {
        if(!hoverShadow)
            hoverShadow = new ShadowProperties();
        setDataShadowProperties(AppStyleSheetHandler::getWidgetStyleSheet(searchPar),hoverShadow);
    }
}

///@details Setting and setting the default shadow
void StyledDropShadowEffect::updateStyle(ShadowState a_state)
{
    switch (a_state)
    {
    case DEFAULT_SHADOW:
        if(defaultShadow)
            setShadowProperties(defaultShadow);
                break;
    case HOVER_SHADOW:
        if(hoverShadow)
            setShadowProperties(hoverShadow);
                break;
    default: break;
    }

    this->update();
//    qobject_cast<QWidget*>(this->parent())->setGraphicsEffect(this);
}

///@details Saving data to a shadow structure
/// @param  a_property String with settings from css.
/// @param data Data structure with shadow settings.
void StyledDropShadowEffect::setDataShadowProperties(const QString &stylesheet, ShadowProperties* data)
{
    //Color:
    QString colorStr = AppStyleSheetHandler::getValueFromStylesheet(stylesheet, "color");
    if (colorStr.isEmpty())
        data->color = QColor(0,0,0,0);
    else
        data->color = Utils::toColor(colorStr);
    data->blur = Utils::toIntValue(AppStyleSheetHandler::getValueFromStylesheet(stylesheet, "blur"));
    //Offset:
    data->x = Utils::toIntValue(AppStyleSheetHandler::getValueFromStylesheet(stylesheet, "x"));
    data->y = Utils::toIntValue(AppStyleSheetHandler::getValueFromStylesheet(stylesheet, "y"));
}

///@details Set shadow options
/// @param data Data structure with shadow settings.
void StyledDropShadowEffect::setShadowProperties(ShadowProperties *data)
{
    this->setColor(data->color);
    this->setBlurRadius(data->blur);
    this->setOffset(data->x, data->y);
}

bool StyledDropShadowEffect::eventFilter(QObject *watched, QEvent *event)
{
    switch (event->type())
    {
        case QEvent::Enter:
            emit mouseEnter(); break;
        case QEvent::Leave:
            emit mouseLaeve(); break;
        default: break;
    }
    return QObject::eventFilter(watched,event);
}
