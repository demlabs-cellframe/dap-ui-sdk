#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "DapUIAnimationScreenAbstract.h"

#include "ui_MainScreenLayout.h"


class MainScreen : public DapUIAnimationScreenAbstract
{
    Q_OBJECT

public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    /// @param a_sw Application window stack.
    MainScreen(QObject * a_parent, QStackedWidget * a_sw);

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
    virtual void initUi(QWidget * a_w, ScreenRotation a_rotation) override;

signals:
    void loginScreenTransition();
};

#endif // MAINSCREEN_H
