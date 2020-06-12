#include "CustomPopupComboBox.h"
#include <QDebug>
#include "CustomComboBoxPopup.h"

/** @brief constructor
 *  @param a_parent object parent
 */
CustomPopupComboBox::CustomPopupComboBox(QWidget *parent)
    : QComboBox (parent)
{
}

void CustomPopupComboBox::showPopup()
{
    if (m_popup)
    {
        if (m_popup->windowType() == Qt::ToolTip)
        {
            QMainWindow* mainWindow = Utils::findParent<QMainWindow*>(this);
            QPoint popupPosition = this->mapTo(mainWindow, QPoint(-1, this->height()));

            popup()->move(popupPosition);

            qDebug() << "CustomPopupComboBox::showPopup" << this->width() << " " << this->popup()->maximumWidth() << this->popup()->width();
            if (this->popup()->sizePolicy().horizontalPolicy() != QSizePolicy::Fixed)
            {
                this->popup()->setMaximumWidth(this->width());
                this->popup()->resize(this->width(), popup()->height());
            }

            qDebug() << "CustomPopupComboBox::showPopup" << this->width() << " " << this->popup()->maximumWidth() << this->popup()->width();

        }
        m_popup->show();
    }

    else
        this->QComboBox::showPopup();
}

CustomComboBoxPopup *CustomPopupComboBox::popup() const
{
    return m_popup;
}

void CustomPopupComboBox::setPopup(CustomComboBoxPopup *popup)
{
    m_popup = popup;
    popup->setModel(this->model());
    popup->setCurrentIndex(this->currentIndex());
    connect(this, SIGNAL(currentIndexChanged(int)), popup, SLOT(setCurrentIndex(int)));

    connect(popup, SIGNAL(activated(int))           , this, SIGNAL(activated(int))           );
    connect(popup, SIGNAL(activated(const QString&)), this, SIGNAL(activated(const QString&)));
    connect(popup, SIGNAL(activated(int))           , this, SLOT  (setCurrentIndex(int))     );
}

QAbstractItemModel *CustomPopupComboBox::model() const
{
    return QComboBox::model();
}

void CustomPopupComboBox::setModel(QAbstractItemModel *a_model)
{
    if (this->popup())
        this->popup()->setModel(a_model);

    QComboBox::setModel(a_model);
}

void CustomPopupComboBox::setCaption(const QString &a_text)
{
    if (m_caption == a_text)
        return;
    m_caption = a_text;

    if (this->popup())
        this->popup()->setCaption(a_text);
}

QString CustomPopupComboBox::caption() const
{
    return m_caption;
}
