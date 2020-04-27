#ifndef CUSTOMBUTTONCOMBOBOX_H
#define CUSTOMBUTTONCOMBOBOX_H

#include <QPushButton>

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

    virtual CustomButtonAbstract *buttonControll() const;

    virtual void setCaption(const QString &a_text) override;

    void setCaptionPolicy(CaptionPolicy a_policy = CaptionPolicy::ShowWhenUnselected);

public slots:

    void setCurrentText(const QString &text);

protected:

    virtual void paintEvent(QPaintEvent *e) override;

private:

    CaptionPolicy m_captionPolicy = CaptionPolicy::ShowWhenUnselected;
    CustomButtonAbstract* m_button = nullptr;

    static const QString BUTTON_NAME_SUFFIX;
};

#endif // CUSTOMBUTTONCOMBOBOX_H
