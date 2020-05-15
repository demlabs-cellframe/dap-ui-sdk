#ifndef LOGINSCREENLAYOUT_H
#define LOGINSCREENLAYOUT_H

#include "NavigationPanelBack.h"
#include "MultiScreenAbstract.h"

#include "ui_LoginScreenLayout.h"

class LoginScreenLayout : public MultiScreenAbstract
{
    Q_OBJECT

public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    LoginScreenLayout(QWidget *a_parent = nullptr);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

    NavigationPanelBack* findNavigationPanel(QWidget* a_widget = nullptr);

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    virtual void initVariantUi(QWidget *a_widget) override;

signals:
    void goBack();

};

#endif // LOGINSCREENLAYOUT_H
