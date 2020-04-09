#ifndef CUSTOMBUTTONCOMBOBOX_H
#define CUSTOMBUTTONCOMBOBOX_H

#include <QPushButton>

#include "CustomComboBox.h"
#include "CustomPlacementButton.h"
#include "CustomButtonAbstract.h"


class CustomButtonComboBox : public CustomComboBox
{
public:
    enum class CaptionPolicy{ShowWhenUnselected, ShowAlways};

    CustomButtonComboBox(QWidget *a_parent = Q_NULLPTR);

    void setObjectName(const QString &a_name);

    void setButtonControll(CustomButtonAbstract* a_button);

    virtual CustomButtonAbstract *buttonControll() const;

    void setCaption(const QString &a_text);

    void setCaptionPolicy(CaptionPolicy a_policy = CaptionPolicy::ShowWhenUnselected);
public slots:

    void setCurrentText(const QString &text);

protected:

    virtual void paintEvent(QPaintEvent *e) override;

private:

    CaptionPolicy m_captionPolicy = CaptionPolicy::ShowWhenUnselected;
    QString m_caption;
    CustomButtonAbstract* m_button = nullptr;

    static const QString BUTTON_NAME_SUFFIX;
};

#endif // CUSTOMBUTTONCOMBOBOX_H
