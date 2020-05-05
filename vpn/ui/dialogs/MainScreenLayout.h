#ifndef MAINSCREENLAYOUT_H
#define MAINSCREENLAYOUT_H

#include "MultiScreenAbstract.h"

#include "ui_MainScreenLayout.h"



class MainScreenLayout : public MultiScreenAbstract
{
    Q_OBJECT

public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    /// @param a_sw Application window stack.
    MainScreenLayout(QWidget *a_parent = nullptr);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

    NavigationPanel* findNavigationPanel(QWidget* a_widget = nullptr);

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
    virtual void initVariantUi(QWidget *a_widget) override;
signals:
    void loginScreenTransition();

    void goToSettings();
    void goToBugReport();
    void goToFAQ();

    void goBack();
};

#endif // MAINSCREENLAYOUT_H
