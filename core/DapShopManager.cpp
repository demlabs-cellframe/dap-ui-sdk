#include "DapShopManager.h"
#include <QString>

//TODO: string ID here not a real strig ID
const QStringList DapShopManager::m_productNames = {
    QStringLiteral("Key_for_1_month"),
    QStringLiteral("Key_for_6_month"),
    QStringLiteral("Key_for_year")
};

void DapShopManager::setupConnections()
{
    connect(m_store, SIGNAL(productRegistered(QInAppProduct*)),
            this, SLOT(markProductAvailable(QInAppProduct*)));
    connect(m_store, SIGNAL(productUnknown(QInAppProduct*)),
            this, SLOT(handleErrorGracefully(QInAppProduct*)));
    connect(m_store, SIGNAL(transactionReady(QInAppTransaction*)),
            this, SLOT(handleTransaction(QInAppTransaction*)));
}

DapShopManager::DapShopManager(QObject *parent) : QObject(parent)
{
    m_store = new QInAppStore(parent);
    //TODO: setup Android Public Key here:
    //m_store->setPlatformProperty(QStringLiteral("AndroidPublicKey"), QStringLiteral(""));
    setupConnections();

    for (auto name: m_productNames) {
        m_store->registerProduct(QInAppProduct::Consumable, name);
    }
}

void DapShopManager::doPurchase(DapShopManager::Products product)
{
    QInAppProduct *inAppProduct = nullptr;
    switch (product)
    {
    case PRODUCT_1MONTH_KEY:
        inAppProduct = m_store->registeredProduct(m_productNames[0]);
        break;
    case PRODUCT_6MONTHS_KEY:
        inAppProduct = m_store->registeredProduct(m_productNames[1]);
        break;
    case PRODUCT_YEAR_KEY:
        inAppProduct = m_store->registeredProduct(m_productNames[2]);
        break;
    }
    if (inAppProduct)
        inAppProduct->purchase();
}

void DapShopManager::handleErrorGracefully(QInAppProduct *product)
{
    qDebug() << "[IN-APP STORE] Error: product " << product->identifier() << product;
    //TODO: set disable product in screen
}

void DapShopManager::handleTransaction(QInAppTransaction *transaction)
{
    if (transaction->status() == QInAppTransaction::PurchaseApproved) {
        //TODO: realize something like this:
        //  if (!hasAlreadyStoredTransaction(transaction->orderId())) {
        //      if (!addKeyToPersistentStorage(transaction->orderId()))
        //          popupErrorDialog(tr("Unable to write to persistent storage. Please make sure there is sufficient space and restart."));
        //      else
                    transaction->finalize();
        //  }
    } else if (transaction->status() == QInAppTransaction::PurchaseFailed) {
        qDebug() << "[IN-APP STORE] Error: " << "Purchase is failed.";
        transaction->finalize();
    }
}

void DapShopManager::markProductAvailable(QInAppProduct *)
{
    //TODO: set enable product in screen
}
