#ifndef CUSTOMBUTTONCOMBOBOX_H
#define CUSTOMBUTTONCOMBOBOX_H

#include <QPushButton>

#include "CustomComboBox.h"

class CustomButtonComboBox : public CustomComboBox
{
public:
    CustomButtonComboBox(QWidget *a_parent = Q_NULLPTR);

    void setObjectName(const QString &a_name);

    void setButtonControll(QPushButton* a_button);
    QPushButton *buttonControll() const;

public slots:
    void setCurrentText(const QString &text);

protected:
    virtual void paintEvent(QPaintEvent *e) override;

private:
    QPushButton* m_button = nullptr;

    static const QString BUTTON_NAME_SUFFIX;
};

#endif // CUSTOMBUTTONCOMBOBOX_H
