#ifndef DAPTUNLINUX_H
#define DAPTUNLINUX_H

#include "DapTunUnixAbstract.h"
#include "DapDNSController.h"

class DapTunLinux : public DapTunUnixAbstract
{
    Q_OBJECT
public:
    explicit DapTunLinux(QObject *parent = nullptr);
    ~DapTunLinux();
protected:
    void tunDeviceCreate() override;
    void tunDeviceDestroy() override;
    void onWorkerStarted() override;
    void resetNetworkSettingsToDefault();

    // Getting currently using connection interface name from nmcli command-line tool
    // and save to m_lastUsedConnectionName and m_lastUsedConnectionDevice
    void saveCurrentConnectionInterfaceData();

private:
    QString nmcliVersion;
    static int nmcliVersionMajor;
    static int nmcliVersionMinor;
    static int nmcliVersionBuild;

    // Connection witch used before creating DiveVPN Interface
    QString m_lastUsedConnectionName;
    QString m_lastUsedConnectionDevice;

    // operates m_lastUsedConnectionDevice
    void disableIPV6();
    void enableIPV6();

    QString runBashCmd(const QString& cmd);

    bool connectionExists(const QString &connName);

    // if metric is 0 upping to 15
    void checkDefaultGetaweyMetric();

    DapDNSController *m_dnsController;
};

#endif // DAPTUNLINUX_H
