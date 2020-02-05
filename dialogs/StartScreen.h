#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include "DapUiScreen.h"
#include "Screen.h"

#include "ui_StartScreen.h"

class StartScreen : public Screen
{
    Q_OBJECT

public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    /// @param a_sw Application window stack.
    StartScreen(QWidget * a_parent);

//    virtual void activateScreen() override;

//    virtual QString screenName() override;
//    static const QString SCREEN_NAME;

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
    virtual void initUi(QWidget * a_w, ScreenInfo::Rotation a_rotation) override;

    QPushButton *btnSignIn;
    QPushButton *btnSignUp;

signals:
    void transitionTo_SignIn();

private:
    void setupStateMachine();
};

#endif // STARTSCREEN_H
