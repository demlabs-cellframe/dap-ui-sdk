#include "DapShopManager.h"
#include <QString>
#include <QDebug>

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#endif

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

    m_sku = m_productNames[index];

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

void DapShopManager::purchaseVerified(const QString &key)
{
    // Acknowledge (подтверждение покупки) делается на сервере
    // тут делаем Consume (использование покупки), чтобы иметь возможность продать этот товар повторно
#ifdef Q_OS_ANDROID
    if (!key.isNull() && !key.isEmpty()) {
        m_store.callMethod<void>("purchaseConsume", "(Ljava/lang/String;)V",
                                 QAndroidJniObject::fromString(key).object<jstring>());
    }
#endif
}

void DapShopManager::requestPurchase(const QString &sku, const QString &token)
{
    if (!sku.isEmpty() && !token.isEmpty()) {
        m_token = token;
        emit DapShopManager::instance()->requestPurchaseVerify(m_sku, m_token);
    }
}

void DapShopManager::reportErrorMessage(const QString &msg)
{
    m_lastError = msg;
    emit DapShopManager::instance()->errorMessage(m_lastError);
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

//- статические функции - для вызовов из java -

void DapShopManager::reportError(JNIEnv *env, jobject thiz, jstring error)
{
    Q_UNUSED(thiz)
    const charr = env->GetStringUTFChars(error, 0);
    QString str = QString(ptr);
    QString s = QString("[IN-APP STORE] Error: ").append(str);
    qDebug() << s;
    DapShopManager::instance()->reportErrorMessage(str);
    env->ReleaseStringUTFChars(error, ptr);
}

void DapShopManager::reportPurchase(JNIEnv *env, jobject thiz, jstring sku, jstring token)
{
    Q_UNUSED(thiz)
    const charr1 = env->GetStringUTFChars(sku, 0);
    QString strSku = ptr1;
    const charr2 = env->GetStringUTFChars(token, 0);
    QString strToken = ptr2;
    DapShopManager::instance()->requestPurchase(strSku, strToken);
    env->ReleaseStringUTFChars(sku, ptr1);
    env->ReleaseStringUTFChars(token, ptr2);
}
