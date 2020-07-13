#ifndef BUGREPORTRESULTSCREEN_H
#define BUGREPORTRESULTSCREEN_H

#include "AdaptiveScreen.h"
#include "ui_BugReportResultScreen.h"

class BugReportResultScreen : public AdaptiveScreen
{
    Q_OBJECT

public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    BugReportResultScreen(QWidget *a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

public slots:
    void setStatusMessage(const QString& a_message);

signals:
    void goBack();

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    virtual void initVariantUi(QWidget *a_widget) override;

private:

    QScopedPointer<Ui::BugReportResultScreen> m_ui;
};

#endif // BUGREPORTRESULTSCREEN_H
