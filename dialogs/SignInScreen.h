#ifndef SIGNINSCREEN_H
#define SIGNINSCREEN_H

#include <QLineEdit>
#include <QLabel>

#include "AdaptiveScreen.h"
#include "ScreenInfo.h"


class SignInScreen : public AdaptiveScreen
{
    Q_OBJECT
public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    SignInScreen(QWidget * a_parent);

    virtual QString screenName() override;
    static const QString SCREEN_NAME;

protected:

    virtual void initVariantUi(QWidget *a_widget) override;
};


#endif // SIGNINSCREEN_H
