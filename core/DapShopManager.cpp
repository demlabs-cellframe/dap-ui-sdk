#include "DapShopManager.h"
#include <QString>
#include <QDebug>

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#endif

// Plan identifiers (SKU)
const QString DapShopManager::PLAN_1M = QStringLiteral("plan_1m");
const QString DapShopManager::PLAN_6M = QStringLiteral("plan_6m");
const QString DapShopManager::PLAN_12M = QStringLiteral("plan_12m");

// The product ID
const QStringList DapShopManager::m_productNames = {
    "Undefined",
    DapShopManager::PLAN_1M,
    DapShopManager::PLAN_6M,
    DapShopManager::PLAN_12M
};

void DapShopManager::setupConnections()
{
}

DapShopManager::DapShopManager(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_ANDROID
    qDebug()<<"[IN-APP STORE] DapShopManager";
    for (int i = PRODUCT_UNDEFINED_KEY + 1; i < PRODUCT_COUNT; i++) {
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
        index = 1;
        break;
    case PRODUCT_6MONTHS_KEY:
        index = 2;
        break;
    case PRODUCT_YEAR_KEY:
        index = 3;
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
#ifdef BUILD_VAR_GOOGLE
    /*---- Purchase BUILD_VAR_GOOGLE----*/
    qInfo() << "Payment response" << m_productNames[index] << index;
    jint res = QtAndroid::androidActivity()
            .callMethod<jint>("launchBilling"
            ,"(Ljava/lang/String;)I"
            ,QAndroidJniObject::fromString(m_productNames[index]).object<jstring>());
//    jint res = m_store.callMethod<jint>("launchBilling", "(Ljava/lang/String;)I",
//                             QAndroidJniObject::fromString(m_productNames[index]).object<jstring>());
    qInfo() << "Payment response code: " << res;
#else
    QString url = DAP_LICENCE_URL;
    QDesktopServices::openUrl(QUrl(url));
#endif
#endif

}

DapShopManager::ProductState DapShopManager::getProdustState(DapShopManager::Products product) const
{
    ProductState state = STATE_UNAVAILABLE;
    if (product > PRODUCT_UNDEFINED_KEY && product < PRODUCT_COUNT)
        state = m_products[product];
    return state;
}

void DapShopManager::purchaseVerified(const QString &key)
{
    // Acknowledge (purchase confirmation) is done on the server
    // here we do Consume (purchase usage) to be able to sell this product again
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

//- static functions - for calls from java -

#ifdef Q_OS_ANDROID
void DapShopManager::reportError(JNIEnv *env, jobject thiz, jstring error)
{
    Q_UNUSED(thiz)
    const char* ptr = env->GetStringUTFChars(error, 0);
    QString str = QString(ptr);
    QString s = QString("[IN-APP STORE] Error: ").append(str);
    qDebug() << s;
    DapShopManager::instance()->reportErrorMessage(str);
    env->ReleaseStringUTFChars(error, ptr);
}

void DapShopManager::reportPurchase(JNIEnv *env, jobject thiz, jstring sku, jstring token)
{
    Q_UNUSED(thiz)
    const char* ptr1 = env->GetStringUTFChars(sku, 0);
    QString strSku = ptr1;
    const char* ptr2 = env->GetStringUTFChars(token, 0);
    QString strToken = ptr2;
    DapShopManager::instance()->requestPurchase(strSku, strToken);
    env->ReleaseStringUTFChars(sku, ptr1);
    env->ReleaseStringUTFChars(token, ptr2);
}

#endif
