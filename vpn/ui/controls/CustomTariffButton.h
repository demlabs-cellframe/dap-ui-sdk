/*********************************************************************************************
**
** This class is designed so that when you click the button, you will be redirected to the page
** with the required fare. To specify a link, you need to create a
** string(dynamic property) - URLTariff in the designer and specify the required link.
** The appearance is configured as on a normal button.
**
**
**********************************************************************************************/
#ifndef CUSTOMTARIFFBUTTON_H
#define CUSTOMTARIFFBUTTON_H

#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>
#include <QVariant>

class CustomTariffButton: public QPushButton
{
    Q_OBJECT
public:
    CustomTariffButton(QWidget *a_parent);

};

#endif // CUSTOMTARIFFBUTTON_H
