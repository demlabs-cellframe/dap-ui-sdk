#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H


#include <QComboBox>

class CustomComboBoxPopup;


class CustomComboBox : public QComboBox
{
    Q_OBJECT
public:
    CustomComboBox(QWidget *parent = Q_NULLPTR);

    virtual void showPopup() override;

    CustomComboBoxPopup *popup() const;
    void setPopup(CustomComboBoxPopup *popup);

private:

    CustomComboBoxPopup *m_popup = nullptr;

};
#endif // CUSTOMCOMBOBOX_H
