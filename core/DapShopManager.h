#ifndef DAPSHOPMANAGER_H
#define DAPSHOPMANAGER_H

#include <QObject>
#include <QString>
#include <QMap>

#include <QtPurchasing>

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
    // This signal is emitted when the App Store is initializing and after the purchase is complete.
    void productStateChanged(Products product, ProductState state);
    void errorMessage(const QString& meg);

private slots:
    void handleError(QInAppProduct::ProductType productType, const QString &identifier);
    void handleTransaction(QInAppTransaction*);
    void markProductAvailable(QInAppProduct*);

private:
    explicit DapShopManager(QObject *parent = nullptr);
    void setupConnections();
    void changeProductState(const QString &productId, ProductState state);

    QInAppStore *m_store;
    QMap<int, ProductState> m_products;
    QString m_log;
};

#endif // DAPSHOPMANAGER_H
