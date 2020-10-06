#ifndef DAPSHOPMANAGER_H
#define DAPSHOPMANAGER_H

#include <QObject>
#include <QString>

#if !defined Q_OS_WIN
#include <QtPurchasing>
#else
#include "microsoftshop.h"
#endif


class DapShopManager : public QObject
{
    Q_OBJECT

    QInAppStore *m_store;
    void setupConnections();
public:
    enum Products { PRODUCT_1MONTH_KEY, PRODUCT_6MONTHS_KEY, PRODUCT_YEAR_KEY};
    static const QStringList m_productNames;

    explicit DapShopManager(QObject *parent = nullptr);

    void doPurchase(Products product);
    //void restorePurchases(); not relevant... or?..

signals:
    void productPurchased(Products product);
    //void error(const QString& errorMessage); are we need it?

public slots:
    void handleErrorGracefully(QInAppProduct*);
    void handleTransaction(QInAppTransaction*);
    void markProductAvailable(QInAppProduct*);

};

#endif // DAPSHOPMANAGER_H
