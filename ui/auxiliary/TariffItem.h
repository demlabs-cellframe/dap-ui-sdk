#ifndef TARIFFITEM_H
#define TARIFFITEM_H

#include <QMetaType>
#include <QString>

struct TariffItem
{
    QString totalPriceText;
    QString monthPriceText;
    QString durationText;
};

Q_DECLARE_METATYPE(TariffItem)

#endif // TARIFFITEM_H
