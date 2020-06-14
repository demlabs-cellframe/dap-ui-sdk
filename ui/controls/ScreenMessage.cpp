#include "ScreenMessage.h"

ScreenMessage::ScreenMessage(QMainWindow *parent)
    :CustomPopup(parent)
{
create<Ui::ScreenMessage>();
this->setObjectName("stwMessage");
this->setWindowType(Qt::ToolTip);
this->setSizePolicy(QSizePolicy::Preferred, this->sizePolicy().horizontalPolicy());
this->hide();
}

void ScreenMessage::setCaption(const QString& a_caption)
{

}

void ScreenMessage::initVariantUi(QWidget * a_widget)
{

}

void ScreenMessage::showPopup()
{

        if (m_popup->windowType() == Qt::ToolTip)
        {
            QMainWindow* mainWindow = Utils::findParent<QMainWindow*>(this);
            QPoint popupPosition = this->mapTo(mainWindow, QPoint(-1, this->height()));

            move(popupPosition);

            qDebug() << "CustomPopupComboBox::showPopup" << this->width() << " " << this->maximumWidth() << this->width();
            if (this->sizePolicy().horizontalPolicy() != QSizePolicy::Fixed)
            {
                this->setMaximumWidth(this->width());
                this->resize(this->width(), height());
            }

            qDebug() << "CustomPopupComboBox::showPopup" << this->width() << " " << this->maximumWidth() << this->width();

        }
        m_popup->show();

}
