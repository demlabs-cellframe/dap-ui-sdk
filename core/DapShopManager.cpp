#include "DapShopManager.h"
#include <QString>

const QString androidPublicKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEArU3LGlb29Qc715q8nC0JpNdxWHW8xVXG5qw4iiBWomw9GESeM/dEegOHFa66Hl+iGe5/zkfc/Wgchg8Ta7+fBxOvmKxSnuaAMzaTlzxUGqKYDAhCJAlUkFK578XNGNAr2uia4DcGQfAQYAjeYfZl0kS81uqXAAGlJCWUMUO4pSAJF27L12xLzCA6rIjBzFbhrJXm0P4htCdssAQDl1kv1piuhRJUWW+FCxVyjV/oJl0N/Td9wpPQn284vxWFaxU6WaHKwU8CPIGA4Ze50MjiPfCbQsiiTLUC7BxKGiIH8m4Dglig2Fgr2q9UjWGpn5qwgCfTiVVhayfCMY/tUDOdXQIDAQAB";

// The product ID
const QStringList DapShopManager::m_productNames = {
    QStringLiteral("android.demlabs.kelvpn_1"),
    QStringLiteral("android.demlabs.kelvpn_6"),
    QStringLiteral("android.demlabs.kelvpn_12")
};

void DapShopManager::setupConnections()
{
    connect(m_store, &QInAppStore::productRegistered,
            this, &DapShopManager::markProductAvailable);
    connect(m_store, &QInAppStore::productUnknown,
            this, &DapShopManager::handleError);
    connect(m_store, &QInAppStore::transactionReady,
            this, &DapShopManager::handleTransaction);
}

DapShopManager::DapShopManager(QObject *parent) : QObject(parent)
{
    qDebug()<<"[IN-APP STORE] DapShopManager";
    for (int i = PRODUCT_UNDEFINED + 1; i < PRODUCT_COUNT; i++) {
        m_products[i] = STATE_UNAVAILABLE;
    }

    m_store = new QInAppStore(parent);
    m_store->setPlatformProperty(QStringLiteral("AndroidPublicKey"), androidPublicKey);
    setupConnections();

    m_log = QString("Setup connection to playmarket <br>");

    for (auto name: m_productNames) {
        m_store->registerProduct(QInAppProduct::Consumable, name);
    }
}

DapShopManager *DapShopManager::instance()
{
    static DapShopManager s_instance;
    return &s_instance;
}

void DapShopManager::doPurchase(DapShopManager::Products product)
{
    QInAppProduct *inAppProduct = nullptr;
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
    inAppProduct = m_store->registeredProduct(m_productNames[index]);
    if (inAppProduct) {
        inAppProduct->purchase();
    }
    else {
        m_log = m_productNames[index];
        m_log.append(" purchase failed by unknown reasen");
        emit errorMessage(m_log);
    }
}

DapShopManager::ProductState DapShopManager::getProdustState(DapShopManager::Products product) const
{
    ProductState state = STATE_UNAVAILABLE;
    if (product > PRODUCT_UNDEFINED && product < PRODUCT_COUNT)
        state = m_products[product];
    return state;
}

void DapShopManager::changeProductState(const QString &productId, DapShopManager::ProductState state)
{
    int i = 1;
    for (auto name: m_productNames) {
        if (name == productId) {
            m_products[i] = state;
            emit productStateChanged(Products(i), state);
            m_log.append(name);
            break;
        }
        ++i;
    }
}

void DapShopManager::handleError(QInAppProduct::ProductType productType, const QString &identifier)
{
    Q_UNUSED(productType)
    qDebug() << "[IN-APP STORE] Error: product unknown: " << identifier;
    changeProductState(identifier, STATE_UNAVAILABLE);
    m_log.append(" unavailable in store <br>");
    emit errorMessage(m_log);
}

void DapShopManager::handleTransaction(QInAppTransaction *transaction)
{
    if (transaction == nullptr)
        return;

    if (transaction->status() == QInAppTransaction::PurchaseApproved) {
        auto product = transaction->product();
        changeProductState(product->identifier(), STATE_PURCHASED);
        qDebug() << "[IN-APP STORE] product purchased: " << product->identifier();
        m_log = product->identifier().append(" purchased");
    }
    else if (transaction->status() == QInAppTransaction::PurchaseFailed) {
        qDebug() << "[IN-APP STORE] Transaction failed: " << transaction->failureReason() << transaction->errorString();
        m_log.append("<br>").append(transaction->errorString());
    }
    transaction->finalize();
    emit errorMessage(m_log);
}

void DapShopManager::markProductAvailable(QInAppProduct *product)
{
    if (product == nullptr)
        return;

    changeProductState(product->identifier(), STATE_AVAILABLE);
    m_log.append(" available in store <br>");
    emit errorMessage(m_log);
    qDebug() << "[IN-APP STORE] product available: " << product->identifier();
}
