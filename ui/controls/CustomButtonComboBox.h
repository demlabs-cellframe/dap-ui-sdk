#ifndef CUSTOMBUTTONCOMBOBOX_H
#define CUSTOMBUTTONCOMBOBOX_H

#include <QPushButton>
#include <QLabel>

#include "CustomPopupComboBox.h"
#include "CustomPlacementButton.h"
#include "CustomButtonAbstract.h"


class CustomButtonComboBox : public CustomPopupComboBox
{
public:

    enum class CaptionPolicy{ShowWhenUnselected, ShowAlways};

    CustomButtonComboBox(QWidget *a_parent = Q_NULLPTR);

    void setObjectName(const QString &a_name);

    void setButtonControll(CustomButtonAbstract* a_button);

    void setInscriptionComboBox(QLabel *a_label);
    void setInscriptionText(QString a_text);

    virtual CustomButtonAbstract *buttonControll() const;

    virtual QLabel *buttonInscription() const;

    virtual void setCaption(const QString &a_caption) override;

    void setCaptionPolicy(CaptionPolicy a_policy = CaptionPolicy::ShowWhenUnselected);

protected:

    virtual void paintEvent(QPaintEvent *e) override;

private:
    void updateButtonText();

    CaptionPolicy m_captionPolicy = CaptionPolicy::ShowWhenUnselected;
    CustomButtonAbstract* m_button = nullptr;
    QLabel* m_lable = nullptr;

    static const QString BUTTON_NAME_SUFFIX;
    static const QString LABEL_NAME_SUFFIX;
};

#endif // CUSTOMBUTTONCOMBOBOX_H
