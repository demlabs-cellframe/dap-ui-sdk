#include "CustomButtonComboBox.h"

#include <QVBoxLayout>
#include <QDebug>

const QString CustomButtonComboBox::BUTTON_NAME_SUFFIX = "_control";

CustomButtonComboBox::CustomButtonComboBox(QWidget *a_parent)
    : CustomComboBox(a_parent),
      m_showTextPolicy(CaptionPolicy::showWhenUnselected),
      m_caption("")
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

    connect(a_button, &CustomButtonAbstract::clicked, [this]{
        this->showPopup();
    });

    connect(this, &QComboBox::currentTextChanged, [this](const QString& a_text){

        this->setCurrentText(a_text);
    });
}

void CustomButtonComboBox::setObjectName(const QString &a_name)
{
    if (a_name.isEmpty())
        return;

    CustomComboBox::setObjectName(a_name);
    if(m_button)
        m_button->setObjectName(a_name + BUTTON_NAME_SUFFIX);
}

void CustomButtonComboBox::paintEvent(QPaintEvent *e)
{
    if (!m_button)
        this->CustomComboBox::paintEvent(e);
}

CustomButtonAbstract *CustomButtonComboBox::buttonControll() const
{
    return m_button;
}

void CustomButtonComboBox::setCurrentText(const QString &text)
{
    if (m_button)
    {
        if(m_showTextPolicy == CaptionPolicy::showWhenUnselected)
            m_button->setText(text);
        if(text == "")
            this->setCaption(m_caption);
    }
}

void CustomButtonComboBox::setCaption(const QString &a_text)
{
    if (m_button)
        if(m_showTextPolicy == CaptionPolicy::showAlways)
                m_button->setText(a_text);
    m_caption = a_text;
}

void CustomButtonComboBox::setCaptionPolicy(CaptionPolicy a_policy)
{
    m_showTextPolicy = a_policy;

    setCaption(m_caption);
}
