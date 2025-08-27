#ifndef DAP_DEVELOPER_MODE_MANAGER_H
#define DAP_DEVELOPER_MODE_MANAGER_H

#include <QObject>
#include <QString>

class DapDeveloperModeManager : public QObject
{
	Q_OBJECT
public:
	static DapDeveloperModeManager* instance();

	Q_INVOKABLE bool isDeveloperModeEnabled() const;
	Q_INVOKABLE bool shouldEnableNetworkSelection() const;
	Q_INVOKABLE bool shouldShowNetworkSelection() const;
    Q_INVOKABLE QString getDefaultNetwork() const;
	Q_INVOKABLE QString getDebugInfo() const;

private:
	explicit DapDeveloperModeManager(QObject* parent = nullptr);
	~DapDeveloperModeManager() override = default;
	Q_DISABLE_COPY(DapDeveloperModeManager)

	static DapDeveloperModeManager* s_instance;
};

#endif // DAP_DEVELOPER_MODE_MANAGER_H
