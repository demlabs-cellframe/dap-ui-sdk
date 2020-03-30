#ifndef CUSTOMBUTTONCOMBOBOX_H
#define CUSTOMBUTTONCOMBOBOX_H

#include <QPushButton>

#include "CustomComboBox.h"
#include "CustomPlacementButton.h"
#include "CustomButtonAbstract.h"

enum class TextPolicy{showWhenUnselected,showAlways};

class CustomButtonComboBox : public CustomComboBox
{
public:
    CustomButtonComboBox(QWidget *a_parent = Q_NULLPTR);

    void setObjectName(const QString &a_name);

    void setButtonControll(CustomButtonAbstract* a_button);

    CustomButtonAbstract *buttonControll() const;

    void setCaption(const QString &a_text);

    void setCaptionPolicy(TextPolicy a_show = TextPolicy::showWhenUnselected);
public slots:

    void setCurrentText(const QString &text);

protected:

    virtual void paintEvent(QPaintEvent *e) override;

private:

    TextPolicy m_showTextPolicy;

    CustomButtonAbstract* m_button = nullptr;

    static const QString BUTTON_NAME_SUFFIX;
};

#endif // CUSTOMBUTTONCOMBOBOX_H
