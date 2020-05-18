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
        m_popup->show();

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
    connect(popup, SIGNAL(itemSelected(int)), this, SLOT(setCurrentIndex(int)));
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
    if (this->popup())
        this->popup()->setCaption(a_text);
}

QString CustomPopupComboBox::caption() const
{
    return m_caption;
}
