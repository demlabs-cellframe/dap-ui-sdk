#include "CustomButtonComboBox.h"

#include <QVBoxLayout>
#include <QDebug>

const QString CustomButtonComboBox::BUTTON_NAME_SUFFIX = "_control";

CustomButtonComboBox::CustomButtonComboBox(QWidget *a_parent)
    : CustomComboBox(a_parent)
{
    this->setLayout(new QVBoxLayout(this));
    this->layout()->setContentsMargins(0, 0 ,0, 0);
}

void CustomButtonComboBox::setButtonControll(CustomButtonAbstract *a_button)
{
    if (m_button)
        delete m_button;

    a_button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    qDebug() << this->objectName() + BUTTON_NAME_SUFFIX;
    a_button->setObjectName(this->objectName() + BUTTON_NAME_SUFFIX);

    this->layout()->addWidget(a_button);

    this->m_button = a_button;

    connect(a_button, &CustomButtonAbstract::clicked, [this]{
        this->showPopup();
    });

    connect(this, &QComboBox::currentTextChanged, [this](const QString& a_text){

        if(m_showTextPolicy == TextPolicy::showAlways)
            m_button->setText(a_text);
    });
}

void CustomButtonComboBox::setObjectName(const QString &a_name)
{
    if (a_name.isEmpty())
        return;

    this->CustomComboBox::setObjectName(a_name);
    if(m_button)
        m_button->setObjectName(a_name + BUTTON_NAME_SUFFIX);
   // QComboBox::setObjectName(a_name);
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
        m_button->setText(text);
}

void CustomButtonComboBox::setCaption(const QString &a_text)
{
        if (m_button)
            m_button->setText(a_text);
}

void CustomButtonComboBox::setCaptionPolicy(TextPolicy a_show)
{
    m_showTextPolicy = a_show;
}
