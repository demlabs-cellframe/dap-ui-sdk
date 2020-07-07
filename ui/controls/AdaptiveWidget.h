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

    template<class T>
    inline void create(QScopedPointer<T> &a_ui);

    inline QWidget *createEmpty();

    template<class T>
    void setupWidgetForm(QWidget * a_widget);

    template<class T>
    void setupWidgetForm(QWidget * a_widget, QScopedPointer<T> &a_ui);

    virtual void initVariantUi(QWidget * a_widget) = 0;

    template<class T>
    QPushButton* connectBtnToSignall(const QString& a_buttonName, void (T::*a_signal)(), QWidget* a_widget = nullptr);

    template<class T>
    void connectBtnToSignall(QPushButton* a_button, void (T::*a_signal)());

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
void AdaptiveWidget::create(QScopedPointer<T> &a_ui)
{
    ///TODO: add horisontal rotation for mobile.
    QWidget *currentWidget = createEmpty();

    this->setupWidgetForm<T>(currentWidget, a_ui);
    this->initVariantUi(currentWidget);
}

template<class T>
void AdaptiveWidget::setupWidgetForm(QWidget * a_widget)
{
    QScopedPointer<T> l_uiForm(new T);
    l_uiForm->setupUi(a_widget);
}

template<class T>
void AdaptiveWidget::setupWidgetForm(QWidget *a_widget, QScopedPointer<T> &a_ui)
{
    a_ui.reset(new T);
    a_ui->setupUi(a_widget);
}

template<class T>
QPushButton* AdaptiveWidget::connectBtnToSignall(const QString& a_buttonName, void (T::*a_signal)(), QWidget* a_widget /*= nullptr*/)
{
    if (!a_widget)
        a_widget = this->variant();

    QPushButton* button; Utils::findChild(a_widget, a_buttonName, button);

    this->connectBtnToSignall(button, a_signal);

    return button;
}

template<class T>
void AdaptiveWidget::connectBtnToSignall(QPushButton *a_button, void (T::*signal)())
{
    Q_ASSERT(a_button);

    T* reciever = qobject_cast<T*>(this);
    Q_ASSERT(reciever);

    connect(a_button, &QPushButton::clicked, reciever, signal);
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
