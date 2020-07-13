#ifndef BUGREPORTLOADINGSCREEN_H
#define BUGREPORTLOADINGSCREEN_H

#include <QStateMachine>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QRegExpValidator>
#include <QMovie>

#include "AdaptiveScreen.h"
#include "vpnDefine.h"
#include "ui_BugReportScreen.h"
#include "CustomLineHeightLabel.h"
#include "defines.h"
#include "StyledDropShadowEffect.h"
#include "CustomLineEdit.h"
#include "CustomTextEdit.h"

#include "ui_BugReportLoadingScreen.h"


class BugReportLoadingScreen : public AdaptiveScreen
{
    Q_OBJECT

public:

    /// Overloaded constructor.
    /// @param a_parent Parent.
    /// @param a_sw Application window stack.
    BugReportLoadingScreen(QWidget *a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

signals:
    void cancelled();

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
    virtual void initVariantUi(QWidget *a_widget) override;

private:
    QMovie m_movie;

    QScopedPointer<Ui::BugReportLoadingScreen> m_ui;

protected:
    virtual void showEvent(QShowEvent *a_event) override;
    virtual void hideEvent(QHideEvent *a_event) override;
    virtual void resizeEvent(QResizeEvent *a_event) override;
};







#endif // BUGREPORTSCREEN_H
