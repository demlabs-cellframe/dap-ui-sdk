#ifndef DAPDEVELOPERMODEMANAGER_H
#define DAPDEVELOPERMODEMANAGER_H

#include <QObject>
#include <QString>

class DapDeveloperModeManager : public QObject
{
    Q_OBJECT

public:
    explicit DapDeveloperModeManager(QObject *parent = nullptr);
    
    static DapDeveloperModeManager* instance();
    
    Q_INVOKABLE bool isDeveloperModeEnabled() const;
    Q_INVOKABLE bool shouldEnableNetworkSelection() const;
    Q_INVOKABLE QString getDefaultNetwork() const;
    Q_INVOKABLE QString getNetworkForCurrentBrand() const;

#ifdef DEVELOPER_MODE
    Q_INVOKABLE QString getDebugInfo() const;
#endif

private:
    static DapDeveloperModeManager* s_instance;
};

#endif // DAPDEVELOPERMODEMANAGER_H 