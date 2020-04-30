#ifndef BUGREPORTSCREEN_H
#define BUGREPORTSCREEN_H

#include <QStateMachine>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

#include "AdaptiveScreen.h"
#include "vpnDefine.h"
#include "ui_BugReportScreen.h"
#include "CustomLineHeightLabel.h"
#include "defines.h"
#include "StyledDropShadowEffect.h"
#include "CustomLineEdit.h"
#include "CustomTextEdit.h"

class BugReportScreen : public AdaptiveScreen
{
    Q_OBJECT

public:

    /// Overloaded constructor.
    /// @param a_parent Parent.
    /// @param a_sw Application window stack.
    BugReportScreen(QWidget *a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

public slots:

    void setEmail   (const QString &a_email   );
    void setReport(const QString &a_report);
    void checkFieldsAndSendReport();

signals:
    void sendReportRequest();
    void goBack();

    void wrongEmail();
    void wrongReport();

    void emailEdited    (const QString &email   );
    void reportEdited   (const QString &report);



protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
    virtual void initVariantUi(QWidget *a_widget) override;

    const QString LBL_STATUS_MESSAGE    = "lblStatusMessage";
    const QString BTN_SEND              = "btnSend";
    const QString EDT_EMAIL             = "edtEmail";
    const QString EDT_MESSAGE           = "edtMessage";
    const QString LBL_CAPTION           = "lblCaption";
    const QString VLT_BUG_REPORT        = "vltBugReport";

private:
    QString m_email;
    QString m_report_message;

    bool checkEmail();
    bool checkReport();
};

#endif // BUGREPORTSCREEN_H
