#include "CustomButtonComboBox.h"

#include <QVBoxLayout>
#include <QDebug>

#include "CustomComboBoxPopup.h"

const QString CustomButtonComboBox::BUTTON_NAME_SUFFIX = "_control";

CustomButtonComboBox::CustomButtonComboBox(QWidget *a_parent)
    : CustomPopupComboBox(a_parent)
{
    this->setLayout(new QVBoxLayout(this));
    this->layout()->setContentsMargins(0, 0 ,0, 0);
}

void CustomButtonComboBox::setButtonControll(CustomButtonAbstract *a_button)
{
    if (m_button)
        delete m_button;

    a_button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    a_button->setObjectName(this->objectName() + BUTTON_NAME_SUFFIX);

    this->layout()->addWidget(a_button);

    this->m_button = a_button;

    connect(a_button, &CustomButtonAbstract::clicked, [this] {
        this->showPopup();
    });

    connect(this, &QComboBox::currentTextChanged, [this](const QString& a_text) {
        this->setCurrentText(a_text);
    });
}

void CustomButtonComboBox::setObjectName(const QString &a_name)
{
    if (a_name.isEmpty())
        return;

    CustomPopupComboBox::setObjectName(a_name);
    if(m_button)
        m_button->setObjectName(a_name + BUTTON_NAME_SUFFIX);
}

void CustomButtonComboBox::paintEvent(QPaintEvent *e)
{
    if (!m_button)
        this->CustomPopupComboBox::paintEvent(e);
}

CustomButtonAbstract *CustomButtonComboBox::buttonControll() const
{
    return m_button;
}

void CustomButtonComboBox::setCurrentText(const QString &a_text)
{
    if (m_button)
    {
        if ((m_captionPolicy == CaptionPolicy::ShowWhenUnselected && !a_text.isEmpty()) || this->caption().isEmpty())
            m_button->setText(a_text);
        else if (a_text == "")
            m_button->setText(this->caption());
    }
}

void CustomButtonComboBox::setCaption(const QString &a_caption)
{
    if (a_caption.isEmpty())
        m_button->setText(this->currentText());

    else if (m_button &&  ((m_captionPolicy == CaptionPolicy::ShowWhenUnselected && currentText()=="") ||  m_captionPolicy == CaptionPolicy::ShowAlways))
        m_button->setText(a_caption);

    this->CustomPopupComboBox::setCaption(a_caption);
}
//Why is that?
void CustomButtonComboBox::setCaptionPolicy(CaptionPolicy a_policy)
{
    m_captionPolicy = a_policy;

//especially why is that?
   // this->setCaption(this->caption());
}
