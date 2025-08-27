#include "DapDeveloperModeManager.h"
#include <QtGlobal>

DapDeveloperModeManager* DapDeveloperModeManager::s_instance = nullptr;

DapDeveloperModeManager* DapDeveloperModeManager::instance()
{
	if (!s_instance) {
		s_instance = new DapDeveloperModeManager();
	}
	return s_instance;
}

DapDeveloperModeManager::DapDeveloperModeManager(QObject* parent)
	: QObject(parent)
{
}

bool DapDeveloperModeManager::isDeveloperModeEnabled() const
{
#ifdef DEVELOPER_MODE
	return DEVELOPER_MODE != 0;
#else
	return false;
#endif
}

bool DapDeveloperModeManager::shouldEnableNetworkSelection() const
{
	return isDeveloperModeEnabled();
}

bool DapDeveloperModeManager::shouldShowNetworkSelection() const
{
	return true;
}

QString DapDeveloperModeManager::getDefaultNetwork() const
{
	// Return network name (not token). Map by brand macros defined in config.pri
#ifdef BRAND_KELVPN
	return QStringLiteral("KelVPN");
#elif defined(BRAND_VENDETA)
	return QStringLiteral("riemann");
#elif defined(BRAND_RISEVPN)
	return QStringLiteral("RiseVPN");
#else
	return QStringLiteral("KelVPN");
#endif
}

QString DapDeveloperModeManager::getDebugInfo() const
{
	return QString("developer_mode=%1, default_network=%2")
		.arg(isDeveloperModeEnabled() ? "1" : "0")
		.arg(getDefaultNetwork());
}
