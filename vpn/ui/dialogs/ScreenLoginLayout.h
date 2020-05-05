#ifndef LOGINLAYOUT_H
#define LOGINLAYOUT_H

#include <QWidget>
#include "DapUiScreen.h"
#include "DapUIAnimationScreenAbstract.h"
#include "ScreenLogin.h"
#include "StartScreen.h"
#include <type_traits>


#include "ui_login_layout.h"

namespace Ui {
class LoginLayout;
}

/**
* Class wich hold logo part and Login/SignUp screens
*/
class ScreenLoginLayout : public DapUIAnimationScreenAbstract
{
    Q_OBJECT

public:
    explicit ScreenLoginLayout(QObject *a_parent, QStackedWidget *a_sw);

    ScreenLogin *screenLogin() const;

protected:
    void initUi(QWidget *a_w, ScreenRotation a_rotation);

private:
    ScreenLogin *m_screenLogin; ///<Login screen pointer
};


#endif // SCREENLOGINLAYOUT_H
