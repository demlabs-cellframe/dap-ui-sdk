#ifndef DAPSHOPMANAGER_H
#define DAPSHOPMANAGER_H

#include <QObject>
#include <QString>
#include <QMap>

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/qandroidjniobject.h>
#include <QtAndroidExtras/qandroidactivityresultreceiver.h>
#endif

class DapShopManager : public QObject
{
    Q_OBJECT

public:
    enum Products { PRODUCT_UNDEFINED_KEY, PRODUCT_1MONTH_KEY, PRODUCT_6MONTHS_KEY, PRODUCT_YEAR_KEY, PRODUCT_COUNT};
    enum ProductState { STATE_UNAVAILABLE, STATE_AVAILABLE, STATE_PURCHASED };
    static const QStringList m_productNames;

    static DapShopManager* instance();
    void doPurchase(Products product);
    ProductState getProdustState(Products product) const;
    void purchaseVerified(const QString& key);

    QString getLastError() {
        return m_lastError;
    }
    QString getLastPurchaseSku() {
        return m_sku;
    }
    QString getLastPurchaseToken() {
        return m_token;
    }

signals:
    //@brief Signal that the product status has changed
    // This signal is emitted after the purchase is complete.
    void productStateChanged(Products product, ProductState state);
    void errorMessage(const QString& meg);
    void requestPurchaseVerify(const QString& productId, const QString& purchaseToken);

private slots:
#ifdef Q_OS_ANDROID
    static void reportError(JNIEnv *env, jobject thiz, jstring error);
    static void reportPurchase(JNIEnv *env, jobject thiz, jstring sku, jstring token);
#endif

private:
    explicit DapShopManager(QObject *parent = nullptr);
    void setupConnections();
    void changeProductState(const QString &productId, ProductState state);
    void requestPurchase(const QString& sku, const QString& token);
    void reportErrorMessage(const QString& msg);

#ifdef Q_OS_ANDROID
    QAndroidJniObject m_store;
#endif
    QString m_token;
    QString m_sku;
    QString m_lastError;

    static const QString PLAN_1M;
    static const QString PLAN_6M;
    static const QString PLAN_12M;

    QMap<int, ProductState> m_products;
};

#endif // DAPSHOPMANAGER_H
