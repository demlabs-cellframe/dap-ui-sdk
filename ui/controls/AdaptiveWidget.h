#ifndef ADAPTIVEWIDGET_H
#define ADAPTIVEWIDGET_H

#include <QStackedWidget>
#include <ScreenInfo.h>
#include <QState>
#include <QStyle>
#include <QPushButton>
#include "Utilz.h"

#ifdef ANDROID
    #define DEFAULT_VARIANT ScreenInfo::Rotation::Vertical
#else
    #define DEFAULT_VARIANT ScreenInfo::Rotation::Horizontal
#endif

class AdaptiveWidget : public QStackedWidget
{
public:
    AdaptiveWidget(QWidget *a_parent = nullptr);

    QWidget* variant(ScreenInfo::Rotation a_rotation = DEFAULT_VARIANT);


protected:
    template<class T>
    inline void create();

    inline QWidget *createEmpty();


    template<class T>
    void setupWidgetForm(QWidget * a_widget);

    virtual void initVariantUi(QWidget * a_widget) = 0;

    template<class T>
    QPushButton* connectBtnToSignall(const QString& a_buttonName, void (T::*signal)(), QWidget* a_widget = nullptr);

    void setChildProperties(const QString& a_objName, const QString& a_property, const QVariant& a_value);
    void updateChildStyle(const QString& a_objName);

    template <class T /*= QWidget*/>
    inline QList<T*> getTheSameWidgets(const QString& a_objName = QString());

    template <class T /*= QWidget*/>
    void assignWidgetPropertyForState(QState *a_state, const QString& a_objName, const QString& a_property,  const QVariant& a_value);

private:
    QMap<ScreenInfo::Rotation, QWidget*> m_variants;

};


/////////////////////////////////////////////////////////////////////////////////////////

QWidget *AdaptiveWidget::createEmpty()
{
    QWidget *currentWidget = new QWidget(this);

    this->addWidget(currentWidget);
    this->m_variants.insert(DEFAULT_VARIANT, currentWidget);

    return currentWidget;
}

template<class T>
inline void AdaptiveWidget::create()
{
    ///TODO: add horisontal rotation for mobile.
    QWidget *currentWidget = createEmpty();

    this->setupWidgetForm<T>(currentWidget);
    this->initVariantUi(currentWidget);
}

template<class T>
void AdaptiveWidget::setupWidgetForm(QWidget * a_widget)
{
    static T * l_uiForm = nullptr;
    delete l_uiForm;
    l_uiForm = new T;
    l_uiForm->setupUi(a_widget);

    //TODO: delete l_uiForm after use
}

template<class T>
QPushButton* AdaptiveWidget::connectBtnToSignall(const QString& a_buttonName, void (T::*signal)(), QWidget* a_widget /*= nullptr*/)
{
    if (!a_widget)
        a_widget = this->variant();

    QPushButton* button; Utils::findChild(a_widget, a_buttonName, button);

    T* reciever = qobject_cast<T*>(this);
    Q_ASSERT(reciever);

    connect(button, &QPushButton::clicked, reciever, signal);
    return button;
}

template <class T /*= QWidget*/>
inline QList<T*> AdaptiveWidget::getTheSameWidgets(const QString& a_objName)
{
    QList<T*> widgetsList;
    T* foundWidget = this->currentWidget()->findChild<T*>(a_objName);
    if (foundWidget)
        widgetsList.append(foundWidget);

    ///TODO: add horisontal rotation for mobile.

//        for (auto l_rotation: ScreenInfo::allRotations())
//        {
//            T* foundWidget = page(l_rotation)->findChild<T*>(a_objName);
//            if (foundWidget)
//                widgets.append(foundWidget);
//        }
    return widgetsList;
}

template<class T>
void AdaptiveWidget::assignWidgetPropertyForState(QState *a_state, const QString &a_objName, const QString &a_property, const QVariant &a_value)
{
    for (auto obj: getTheSameWidgets<T>(a_objName)) {
        a_state->assignProperty(obj, qPrintable(a_property), a_value);
    }
}

#endif // ADAPTIVEWIDGET_H
