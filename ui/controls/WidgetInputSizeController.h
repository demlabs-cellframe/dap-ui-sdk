/*Controller class. Manages the installed set of widgets. When the input fields come into focus,
 * it turns on the keyboard and makes the set of widgets invisible. When the keyboard disappears
 * or a nickname outside the input fields makes the widget set visible and removes the focus if necessary. */

#ifndef WIDGETINPUTSIZECONTROLLER_H
#define WIDGETINPUTSIZECONTROLLER_H

#include <QWidget>
#include "CustomLineEditBase.h"
#include "CustomTextEdit.h"
#include <QApplication>

class WidgetInputSizeController : public QObject
{
    Q_OBJECT
public:
    WidgetInputSizeController(QObject *a_parent = nullptr);

    /// Adds widgets that should disappear/appear.
    void addDisappearingWidget(QWidget *a_widget);
    ///Adds widgets to control input fields.
    void addWidgetEmitsSignal(CustomLineEditBase *a_widget);
    ///The widget on which we focus in the case of a click outside the input field or the folding keyboard.
    void addWidgetForFocus(QWidget *a_widget);

private:
    QList<QWidget*> m_disappearingWidget;

    QWidget *m_widgetToGoShow = nullptr;
signals:
    void sigShowWidget();

private slots:
    void setVisibleWidgets(bool a_visible);
};

#endif // WIDGETINPUTSIZECONTROLLER_H
