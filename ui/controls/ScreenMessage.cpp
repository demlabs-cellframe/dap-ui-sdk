#include "ScreenMessage.h"

ScreenMessage::ScreenMessage(QWidget *parent)
    :CustomPopup(parent)
{
create<Ui::ScreenMessage>();

this->hide();
}

void ScreenMessage::setTextMessage(const QString& a_message)
{
    m_lblMessage->setText(a_message);
}

void ScreenMessage::initVariantUi(QWidget * a_widget)
{
                    m_lblMessage    = a_widget->findChild<QLabel*>(LBL_MESSAGE);               Q_ASSERT(m_lblMessage);

    QPushButton     *btnCencel      = a_widget->findChild<QPushButton*>(BTN_CENCEL);           Q_ASSERT(btnCencel);
    QPushButton     *btnDelete      = a_widget->findChild<QPushButton*>(BTN_DELETE);           Q_ASSERT(btnDelete);

    connect(btnCencel,&QPushButton::clicked,this,&ScreenMessage::hide);

    connect(btnDelete,&QPushButton::clicked,[=]{
        emit this->deleteSerialKey();
        this->hide();
    });
}

