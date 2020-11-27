#include "DapShopManager.h"
#include <QString>
#include <QDebug>

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#endif

const QString androidPublicKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEArU3LGlb29Qc715q8nC0JpNdxWHW8xVXG5qw4iiBWomw9GESeM/dEegOHFa66Hl+iGe5/zkfc/Wgchg8Ta7+fBxOvmKxSnuaAMzaTlzxUGqKYDAhCJAlUkFK578XNGNAr2uia4DcGQfAQYAjeYfZl0kS81uqXAAGlJCWUMUO4pSAJF27L12xLzCA6rIjBzFbhrJXm0P4htCdssAQDl1kv1piuhRJUWW+FCxVyjV/oJl0N/Td9wpPQn284vxWFaxU6WaHKwU8CPIGA4Ze50MjiPfCbQsiiTLUC7BxKGiIH8m4Dglig2Fgr2q9UjWGpn5qwgCfTiVVhayfCMY/tUDOdXQIDAQAB";

// The product ID
const QStringList DapShopManager::m_productNames = {
    QStringLiteral("android.demlabs.kelvpn_1"),
    QStringLiteral("android.demlabs.kelvpn_6"),
    QStringLiteral("android.demlabs.kelvpn_12")
};

void DapShopManager::setupConnections()
{
}

DapShopManager::DapShopManager(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_ANDROID
    qDebug()<<"[IN-APP STORE] DapShopManager";
    for (int i = PRODUCT_UNDEFINED + 1; i < PRODUCT_COUNT; i++) {
        m_products[i] = STATE_AVAILABLE;
    }

    m_store = QAndroidJniObject("com/demlabs/dapchain/InAppShop",
                                "(Landroid/app/Activity;J)V",
                                QtAndroid::androidActivity().object<jobject>(),
                                reinterpret_cast<jlong>(this));
    if (!m_store.isValid()) {
        qWarning("Cannot initialize IAP backend for Android due to missing dependency: QtInAppPurchase class");
        return;
    }

    //jint i = m_store.callMethod<jint>("test", "(I)I", 3);
    //qDebug()<<"demlab: jni test: "<<i;

    JNINativeMethod methods[] {
        {"reportError", "(Ljava/lang/String;)V", reinterpret_cast<void *>(DapShopManager::reportError)},
        {"reportPurchase", "(Ljava/lang/String;Ljava/lang/String;)V", reinterpret_cast<void *>(DapShopManager::reportPurchase)}
    };
    QAndroidJniEnvironment env;
    jclass objectClass = env->GetObjectClass(m_store.object<jobject>());
    env->RegisterNatives(objectClass, methods, sizeof(methods) / sizeof(methods[0]));
    env->DeleteLocalRef(objectClass);

    m_store.callMethod<void>("initialize", "()V");
#endif
}

DapShopManager *DapShopManager::instance()
{
    static DapShopManager s_instance;
    return &s_instance;
}

void DapShopManager::doPurchase(DapShopManager::Products product)
{
    //QInAppProduct *inAppProduct = nullptr;
    int index = 0;
    switch (product)
    {
    case PRODUCT_1MONTH_KEY:
        index = 0;
        break;
    case PRODUCT_6MONTHS_KEY:
        index = 1;
        break;
    case PRODUCT_YEAR_KEY:
        index = 2;
        break;
    default: // unknown product
        break;
    }

    //inAppProduct = m_store->registeredProduct(m_productNames[index]);
    //if (inAppProduct) {
    //    inAppProduct->purchase();
    //}
    //else {
    //    m_log = m_productNames[index];
    //    m_log.append(" purchase failed by unknown reasen");
    //    emit errorMessage(m_log);
    //}

#ifdef Q_OS_ANDROID
    m_store.callMethod<jint>("launchBilling", "(Ljava/lang/String;)I",
                             QAndroidJniObject::fromString(m_productNames[index]).object<jstring>());
#endif
}

DapShopManager::ProductState DapShopManager::getProdustState(DapShopManager::Products product) const
{
    ProductState state = STATE_UNAVAILABLE;
    if (product > PRODUCT_UNDEFINED && product < PRODUCT_COUNT)
        state = m_products[product];
    return state;
}

void DapShopManager::reportError(const QString &error)
{
    QString s = QString("[IN-APP STORE] Error: ").append(error);
    qDebug() << s;
    emit DapShopManager::instance()->errorMessage(s);
}

void DapShopManager::reportPurchase(QString sku, QString token)
{
    emit DapShopManager::instance()->requestPurchaseVerify(sku, token);
}

void DapShopManager::purchaseVerified(const QString &key)
{
    // Acknowledge (подтверждение покупки) делается на сервере
    //TODO: тут делаем Consume (использование покупки), чтобы иметь возможность продать этот товар повторно
}

void DapShopManager::changeProductState(const QString &productId, DapShopManager::ProductState state)
{
    int i = 1;
    for (auto name: m_productNames) {
        if (name == productId) {
            m_products[i] = state;
            emit productStateChanged(Products(i), state);
            break;
        }
        ++i;
    }
}
