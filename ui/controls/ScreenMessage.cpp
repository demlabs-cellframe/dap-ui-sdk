#include "ScreenMessage.h"

ScreenMessage::ScreenMessage(QWidget *parent)
    :CustomPopup(parent)
{
create<Ui::ScreenMessage>();

this->hide();
}

void ScreenMessage::setCaption(const QString& a_caption)
{

}

void ScreenMessage::initVariantUi(QWidget * a_widget)
{
    QPushButton     *btnCencel    = a_widget->findChild<QPushButton*>("btnCencel");           Q_ASSERT(btnCencel);
    QPushButton     *btnDelete    = a_widget->findChild<QPushButton*>("btnDelete");           Q_ASSERT(btnDelete);

    connect(btnCencel,&QPushButton::clicked,this,&ScreenMessage::hide);

    connect(btnDelete,&QPushButton::clicked,[=]{
        emit this->deleteSerialKey();
        this->hide();
    });
}

void ScreenMessage::showPopup()
{

//        if (m_popup->windowType() == Qt::ToolTip)
//        {
//            QMainWindow* mainWindow = Utils::findParent<QMainWindow*>(this);
//            QPoint popupPosition = this->mapTo(mainWindow, QPoint(-1, this->height()));

//            move(popupPosition);

//            qDebug() << "CustomPopupComboBox::showPopup" << this->width() << " " << this->maximumWidth() << this->width();
//            if (this->sizePolicy().horizontalPolicy() != QSizePolicy::Fixed)
//            {
//                this->setMaximumWidth(this->width());
//                this->resize(this->width(), height());
//            }

//            qDebug() << "CustomPopupComboBox::showPopup" << this->width() << " " << this->maximumWidth() << this->width();

//        }
        m_popup->show();

}
