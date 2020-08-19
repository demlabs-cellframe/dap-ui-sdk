#include "WidgetInputSizeController.h"

WidgetInputSizeController::WidgetInputSizeController(QObject *a_parent)
    :QObject (a_parent)
{
    connect(this,&WidgetInputSizeController::sigShowWidget,[this]{
        this->setVisibleWidgets(true);

        if(m_widgetToGoShow!=nullptr)
            m_widgetToGoShow->setFocus();
    });

    connect(QApplication::inputMethod(),&QInputMethod::keyboardRectangleChanged,[=]{
        if(QApplication::inputMethod()->isVisible() && QApplication::inputMethod()->keyboardRectangle().height() == 0.0)
        {
            setVisibleWidgets(true);

            if(m_widgetToGoShow!=nullptr)
                m_widgetToGoShow->setFocus();
        }
    });
}

void WidgetInputSizeController::addWidgetForFocus(QWidget *a_widget)
{
    m_widgetToGoShow = a_widget;
}

void WidgetInputSizeController::addDisappearingWidget(QWidget *a_widget)
{
    m_disappearingWidget.append(a_widget);
}


void WidgetInputSizeController::addWidgetEmitsSignal(CustomLineEditBase *a_widget)
{
    connect(a_widget,&CustomLineEditBase::changeVisibilityVitrulKeyboard,this,&WidgetInputSizeController::setVisibleWidgets);

}

void WidgetInputSizeController::setVisibleWidgets(bool a_visible)
{

    if(!m_disappearingWidget.isEmpty())
    {
        for(auto widget:m_disappearingWidget)
        {
            widget->setVisible(a_visible);
        }


        if(!a_visible)
            QApplication::inputMethod()->show();
        else
        {
            QApplication::inputMethod()->hide();

        }
    }
}
