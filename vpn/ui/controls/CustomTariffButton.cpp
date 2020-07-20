#include "CustomTariffButton.h"

CustomTariffButton::CustomTariffButton(QWidget *a_parent)
    :QPushButton (a_parent)
{
    connect(this, &QPushButton::clicked, [=]{
           QUrl url(property("URLTariff").toString());
           QDesktopServices::openUrl(url);
    });
}
