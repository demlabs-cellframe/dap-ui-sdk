#include "CustomComboBox.h"
#include <QDebug>
#include "CustomComboBoxPopup.h"

/** @brief constructor
 *  @param a_parent object parent
 */
CustomComboBox::CustomComboBox(QWidget *parent)
    : QComboBox (parent)
{
}

void CustomComboBox::showPopup()
{
    if (m_popup)
        m_popup->show();

    else
        this->QComboBox::showPopup();
}

CustomComboBoxPopup *CustomComboBox::popup() const
{
    return m_popup;
}

void CustomComboBox::setPopup(CustomComboBoxPopup *popup)
{
    m_popup = popup;
    popup->setModel(this->model());
    popup->setCurrentIndex(this->currentIndex());
    connect(this, SIGNAL(currentIndexChanged(int)), popup, SLOT(setCurrentIndex(int)));
    connect(popup, SIGNAL(itemSelected(int)), this, SLOT(setCurrentIndex(int)));
}
