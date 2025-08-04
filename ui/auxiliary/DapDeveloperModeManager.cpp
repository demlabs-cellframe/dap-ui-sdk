#include "DapDeveloperModeManager.h"
#include "../../../DapChainVpnQmlApplication/helper/brand.h"
#include <QDebug>

DapDeveloperModeManager::DapDeveloperModeManager(QObject *parent)
    : QObject(parent)
{
}

DapDeveloperModeManager* DapDeveloperModeManager::instance()
{
    static DapDeveloperModeManager instance;
    return &instance;
}

bool DapDeveloperModeManager::isDeveloperModeEnabled() const
{
#ifdef DEVELOPER_MODE
    qDebug() << "DapDeveloperModeManager::isDeveloperModeEnabled - DEVELOPER_MODE macro value:" << DEVELOPER_MODE;
    bool result = (DEVELOPER_MODE == 1);
    qDebug() << "DapDeveloperModeManager::isDeveloperModeEnabled - returning:" << result;
    return result;
#else
    qDebug() << "DapDeveloperModeManager::isDeveloperModeEnabled - DEVELOPER_MODE macro not defined, returning false";
    return false;
#endif
}

bool DapDeveloperModeManager::shouldEnableNetworkSelection() const
{
#ifdef DEVELOPER_MODE
    return DEVELOPER_MODE == 1;
#else
    return false;
#endif
}

QString DapDeveloperModeManager::getDefaultNetwork() const
{
    return getNetworkForCurrentBrand();
}

QString DapDeveloperModeManager::getNetworkForCurrentBrand() const
{
    QString brandName = Brand::name();
    QString networkName;
    
#ifdef BRAND_KELVPN
    networkName = "KelVPN";
#elif BRAND_VENDETA
    networkName = "riemann";
#elif BRAND_RISEVPN
    networkName = "Backbone";
#else
    // Default fallback
    networkName = "KelVPN";
#endif
    
    return networkName;
}

#ifdef DEVELOPER_MODE
QString DapDeveloperModeManager::getDebugInfo() const
{
    QString info = QString("Developer Mode: %1\n").arg(isDeveloperModeEnabled() ? "Enabled" : "Disabled");
    info += QString("DEVELOPER_MODE macro value: %1\n").arg(DEVELOPER_MODE);
    info += QString("Network selection visible: Yes\n");
    info += QString("Network selection enabled: %1\n").arg(shouldEnableNetworkSelection() ? "Yes" : "No");
    info += QString("Current brand: %1\n").arg(Brand::name());
    info += QString("Default network: %1").arg(getNetworkForCurrentBrand());
    
    return info;
}
#endif 