#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QStateMachine>

#include "DapUiScreen.h"
#include "AdaptiveScreen.h"

#include "ui_StartScreen.h"

class StartScreen : public AdaptiveScreen
{
    Q_OBJECT

public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    StartScreen(QWidget * a_parent);

//    virtual void activateScreen() override;

    virtual QString screenName() override;
    static const QString SCREEN_NAME;



    auto stateMachine(){ return &m_stateMachine; };
    void setupStateMachine();

protected:

    void setupControlsPtrs();

    QPushButton *m_btnSignIn {}; const QString BTN_SIGN_IN_NAME = "btnSignIn";
    QPushButton *m_btnSignUp {}; const QString BTN_SIGN_UP_NAME = "btnSignUp";

    virtual void initVariantUi(QWidget *a_widget) override;

signals:

    void transitionTo_SignIn();



private:

    struct {
        QState* rootState     {};
        QState* ctlConnecting {};
        QState* ctlConnected  {};
        bool check() { return rootState && ctlConnecting && ctlConnected; };
    } m_stateMachine;


    typedef void (*InitializerFunc)(StartScreen*);
    static InitializerFunc s_initializerFunc;

};

#endif // STARTSCREEN_H
