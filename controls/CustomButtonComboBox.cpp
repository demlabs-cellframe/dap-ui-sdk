#include "CustomButtonComboBox.h"

#include <QVBoxLayout>
#include <QDebug>

const QString CustomButtonComboBox::BUTTON_NAME_SUFFIX = "";

CustomButtonComboBox::CustomButtonComboBox(QWidget *a_parent)
    : CustomComboBox(a_parent)
{
    this->setLayout(new QVBoxLayout(this));
//    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->layout()->setContentsMargins(0, 0 ,0, 0);
}

void CustomButtonComboBox::setButtonControll(QPushButton *a_button)
{
    if (m_button)
        delete m_button;

    a_button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    qDebug() << this->objectName() + BUTTON_NAME_SUFFIX;
    a_button->setObjectName(this->objectName() + BUTTON_NAME_SUFFIX);

    this->layout()->addWidget(a_button);

    this->m_button = a_button;

    connect(a_button, &QPushButton::clicked, [this]{
        this->showPopup();
    });

    connect(this, &QComboBox::currentTextChanged, [this](const QString& a_text){
        m_button->setText(a_text);
    });
}

void CustomButtonComboBox::setObjectName(const QString &a_name)
{
    if (a_name.isEmpty())
        return;

    this->CustomComboBox::setObjectName(a_name);
    m_button->setObjectName(a_name + BUTTON_NAME_SUFFIX);
}

void CustomButtonComboBox::paintEvent(QPaintEvent *e)
{
    if (!m_button)
        this->CustomComboBox::paintEvent(e);
}

QPushButton *CustomButtonComboBox::buttonControll() const
{
    return m_button;
}

void CustomButtonComboBox::setCurrentText(const QString &text)
{
    if (m_button)
        m_button->setText(text);
}
