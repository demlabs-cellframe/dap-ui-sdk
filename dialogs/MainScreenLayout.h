#ifndef MAINSCREENLAYOUT_H
#define MAINSCREENLAYOUT_H

#include "DapUIAnimationScreenAbstract.h"

#include "ui_MainScreenLayout.h"


class MainScreenLayout : public DapUIAnimationScreenAbstract
{
    Q_OBJECT

public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    /// @param a_sw Application window stack.
    MainScreenLayout(QObject * a_parent, QStackedWidget * a_sw);

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
    virtual void initUi(QWidget * a_w, ScreenRotation a_rotation) override;

signals:
    void loginScreenTransition();
};

#endif // MAINSCREENLAYOUT_H
