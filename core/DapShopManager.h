#ifndef DAPSHOPMANAGER_H
#define DAPSHOPMANAGER_H

#include <QObject>
#include <QString>
#include <QMap>

#include <QtAndroidExtras/qandroidjniobject.h>
#include <QtAndroidExtras/qandroidactivityresultreceiver.h>

class DapShopManager : public QObject
{
    Q_OBJECT

public:
    enum Products { PRODUCT_UNDEFINED, PRODUCT_1MONTH_KEY, PRODUCT_6MONTHS_KEY, PRODUCT_YEAR_KEY, PRODUCT_COUNT};
    enum ProductState { STATE_UNAVAILABLE, STATE_AVAILABLE, STATE_PURCHASED };
    static const QStringList m_productNames;

    static DapShopManager* instance();
    void doPurchase(Products product);
    ProductState getProdustState(Products product) const;

signals:
    //@brief Signal that the product status has changed
    // This signal is emitted after the purchase is complete.
    void productStateChanged(Products product, ProductState state);
    void errorMessage(const QString& meg);
    void requestPurchaseVerify(const QString& productId, const QString& purchaseToken);

private slots:
    static void reportError(const QString &error);
    static void reportPurchase(QString sku, QString token);

    void purchaseVerified(const QString& key);

private:
    explicit DapShopManager(QObject *parent = nullptr);
    void setupConnections();
    void changeProductState(const QString &productId, ProductState state);

    QAndroidJniObject m_store;
    QMap<int, ProductState> m_products;
};

#endif // DAPSHOPMANAGER_H
