#ifndef ADAPTIVEWIDGET_H
#define ADAPTIVEWIDGET_H

#include "QStackedWidget"
#include <ScreenInfo.h>
#include <QState>

class AdaptiveWidget : public QStackedWidget
{
public:
    AdaptiveWidget(QWidget *a_parent = nullptr);

    QWidget* variant(ScreenInfo::Rotation a_rotation = ScreenInfo::Rotation::Horizontal);

protected:

    template<class T>
    void setupWidgetForm(QWidget * a_widget)
    {
        static T * l_uiForm = nullptr;
        delete l_uiForm;
        l_uiForm = new T;
        l_uiForm->setupUi(a_widget);
    }

    virtual void initVariantUi(QWidget * a_widget) = 0;

    template<class T>
    inline void create()
    {
        ///TODO: add horisontal rotation for mobile.
        QWidget *currentWidget (new QWidget(this));

        this->addWidget(currentWidget);
        this->setupWidgetForm<T>(currentWidget);

        initVariantUi(currentWidget);
    }

    void setChildProperties(const QString& a_objName, const QString& a_property, const QVariant& a_value)
    {
        for (auto widget : getTheSameWidgets<QWidget>(a_objName)) {
            widget->setProperty(a_property.toLatin1().constData(), a_value);
        }
    }

    template <class T /*= QWidget*/>
    inline QList<T*> getTheSameWidgets(const QString& a_objName)
    {
        QList<T*> widgets;
        T* foundWidget = currentWidget()->findChild<T*>(a_objName);
        if (foundWidget)
            widgets.append(foundWidget);

        ///TODO: add horisontal rotation for mobile.

//        for (auto l_rotation: ScreenInfo::allRotations())
//        {
//            T* foundWidget = page(l_rotation)->findChild<T*>(a_objName);
//            if (foundWidget)
//                widgets.append(foundWidget);
//        }     
        return widgets;
    }

    template <class T /*= QWidget*/>
    void assignWidgetPropertyForState(QState *a_state, const QString& a_objName, const QString& a_property,  const QVariant& a_value);

private:
    QMap<ScreenInfo::Rotation, QWidget*> m_variants;

};

template<class T>
void AdaptiveWidget::assignWidgetPropertyForState(QState *a_state, const QString &a_objName, const QString &a_property, const QVariant &a_value)
{
    for (auto obj: getTheSameWidgets<T>(a_objName)) {
        a_state->assignProperty(obj, qPrintable(a_property), a_value);
    }
}

#endif // ADAPTIVEWIDGET_H
