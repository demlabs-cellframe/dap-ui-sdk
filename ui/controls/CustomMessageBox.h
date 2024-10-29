#ifndef CUSTOMMESSAGEBOX_H
#define CUSTOMMESSAGEBOX_H

#include "CustomPopup.h"
#include "CustomLineHeightLabel.h"

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

    ~CustomMessageBox();

    void setDefaultButtonText(const QString&a_defaultButtonText);
    void setEscapeButtonText(const QString&a_escapeButtonText);
    void setText(const QString&a_text);
    void setTitleText(const QString&a_title);

    virtual void doDefaultAction();
    virtual void doEscapeAction();

    bool isAccepted();
    void reset();

signals:
    void accepted();
    void rejected();

public slots:
    void accept();
    void reject();

private slots:
    void onClickedDefaultButton();
    void onClickedEscapeButton();

protected:
    virtual void closeEvent(QCloseEvent*)override;
    virtual void initVariantUi(QWidget *) override;

private:
    void hideButton(QPushButton *btn);
    void showButton(QPushButton *btn);
    QScopedPointer<Ui::CustomMessageBox> m_ui;

    bool m_isAccepted = false;
};

#endif // CUSTOMMESSAGEBOX_H
