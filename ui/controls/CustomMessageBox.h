#ifndef CUSTOMMESSAGEBOX_H
#define CUSTOMMESSAGEBOX_H

#include "CustomPopup.h"

namespace Ui {
class CustomMessageBox;
}

class CustomMessageBox : public CustomPopup
{
    Q_OBJECT

public:
    CustomMessageBox(QWidget* a_parent=nullptr);
    CustomMessageBox(const QString& a_title, const QString& a_defaultButtonText = "Yes",
                     const QString& a_escapeButtonText = "No", const QString& a_text = "",
                     QWidget *a_parent = nullptr);

    void setDefaultButtonText(const QString&a_defaultButtonText);
    void setEscapeButtonText(const QString&a_escapeButtonText);
    void setText(const QString&a_text);
    void setTitleText(const QString&a_title);

    virtual void doDefaultAction();
    virtual void doEscapeAction();

signals:
    void accepted();
    void rejected();

public slots:
    void accept();
    void reject();
    void escape();

private slots:
    void onClickedDefaultButton();
    void onClickedEscapeButton();

protected:
    virtual void closeEvent(QCloseEvent*)override;
    virtual void initVariantUi(QWidget *a_widget) override;
private:
    //Ui::CustomMessageBox *ui;
    QScopedPointer<Ui::CustomMessageBox> m_ui;

    QString m_title{},
    m_defaultButtonText{"Yes"},
    m_escapeButtonText{"No"},
    m_text{};
};

#endif // CUSTOMMESSAGEBOX_H
