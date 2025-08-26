#ifndef DAPDNSCONTROLLER_H
#define DAPDNSCONTROLLER_H

// Qt includes first
#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QHostAddress>
#include <QSet>
#include <QMutex>
#include <QMap>
#include <QTimer>
#include <QThread>
#include <QRegularExpression>

#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <jni.h>
#endif

// Platform specific includes
#ifdef Q_OS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <windows.h>
#include <windns.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "dnsapi.lib")
#pragma comment(lib, "ws2_32.lib")

// Define Windows-specific constants if not defined
#ifndef GAA_FLAG_INCLUDE_PREFIX
#define GAA_FLAG_INCLUDE_PREFIX 0x0010
#endif

// Declare Windows API functions
extern "C" {
    DWORD WINAPI GetAdaptersAddresses(
        ULONG Family,
        DWORD Flags,
        PVOID Reserved,
        PIP_ADAPTER_ADDRESSES AdapterAddresses,
        PULONG SizePointer
    );

    DWORD WINAPI DnsFlushResolverCache();
    DWORD WINAPI DnsRegisterAdapterName(PCWSTR Adapter, PVOID Reserved);
}
#endif

#ifdef DAP_OS_LINUX
#include <resolv.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#ifdef Q_OS_MACOS
#include <SystemConfiguration/SystemConfiguration.h>
#endif

#ifdef Q_OS_IOS
#include <NetworkExtension/NetworkExtension.h>
#endif

/**
 * @brief The DapDNSController class
 * 
 * This class provides a unified interface for managing DNS settings across different platforms.
 * It handles setting DNS servers, restoring default DNS settings, and managing DNS cache.
 * Includes DNS monitoring system to automatically restore VPN DNS settings if manually changed.
 */
class DapDNSController : public QObject
{
    Q_OBJECT
public:
    explicit DapDNSController(QObject *parent = nullptr);
    ~DapDNSController();

    // Main methods for DNS management
    bool setDNSServers(const QStringList &dnsServers);
    bool restoreDefaultDNS();
    QStringList getCurrentDNSServers() const;
    bool isDNSSet() const;

    // New methods for DNS cache and registration management
    bool flushDNSCache();
    bool registerDNS();

    // Emergency DNS restoration methods
    bool hasVPNDNSMarkers() const;
    bool emergencyDNSRestoration();
    bool detectOrphanedVPNDNS() const;

    // Thread-safe accessors
    QString getIfaceName() const;
    void setIfaceName(const QString &name);
    QStringList getOriginalDNSServers() const;
    void setOriginalDNSServers(const QStringList &servers);

    // New interface-specific methods
    bool setDNSServersForInterface(ulong ifIndex, const QStringList &dnsServers);
    bool setDNSServerForInterface(ulong ifIndex, const QString &dnsServer);
    bool restoreDNSForInterface(ulong ifIndex);
    QStringList getCurrentDNSServersForInterface(ulong ifIndex);
    void setTargetInterfaceIndex(ulong ifIndex);
    ulong getTargetInterfaceIndex() const;

    // Helper methods for DNS management
    QStringList getCurrentDNSIndexes(const QString &iface);
    bool resetInterfaceDNS(const QString &iface, bool useDHCP = false);

    // DNS Monitoring System
    void startDNSMonitoring(int intervalMs = 5000);
    void stopDNSMonitoring();
    bool isDNSMonitoringActive() const;
    void setVPNConnectionState(bool connected);
    bool isVPNConnected() const;
    void setExpectedDNSServers(const QStringList &servers);
    QStringList getExpectedDNSServers() const;

    // Linux-specific DNS management methods
    bool setDNSServersLinux(const QStringList &dnsServers);
    bool restoreDefaultDNSLinux();
    QStringList getCurrentDNSServersLinux() const;
    bool trySetDNSWithNetworkManager(const QStringList &dnsServers);
    bool trySetDNSWithNmcli(const QStringList &dnsServers);
    QString getActiveNetworkInterface() const;

signals:
    // Signals for change notifications
    void dnsServersChanged(const QStringList &servers);
    void dnsRestored();
    void errorOccurred(const QString &error);
    
    // DNS Monitoring signals
    void dnsManuallyChanged(const QStringList &detected, const QStringList &expected);
    void dnsAutoRestored(const QStringList &restored);
    void dnsMonitoringStarted();
    void dnsMonitoringStopped();

protected:
    // Helper methods
    bool isValidIPAddress(const QString &ipAddress) const;
    int exec_silent(const QString &cmd);
    bool runNetshCommand(const QString &program, const QStringList &args, QString *output = nullptr, int timeout = 3000);

private slots:
    // DNS Monitoring slots
    void checkDNSIntegrity();
    void onMonitoringTimerTimeout();

private:
    // Mutex for thread safety
    mutable QMutex m_mutex;

    // Store original DNS settings
    QStringList m_originalDNSServers;
    bool m_isDNSSet;
    QString m_ifaceName;  // Store network interface name for DNS operations
    
    // New member variables for interface-specific DNS management
    QMap<ulong, QStringList> m_originalInterfaceDNS;  // Store original DNS per interface
    mutable QMap<ulong, QString> m_interfaceNames;    // Cache interface names by index
    ulong m_targetIfIndex;                            // Target interface index
    
    // Thread-safe registry access
    mutable QMutex m_registryMutex;

    // DNS Monitoring System
    QTimer* m_dnsMonitorTimer;
    bool m_vpnConnected;
    QStringList m_expectedDNSServers;
    bool m_dnsMonitoringActive;
    int m_monitoringInterval;
    int m_consecutiveFailures;
    static const int MAX_CONSECUTIVE_FAILURES = 3;

    // DNS Monitoring helper methods
    bool detectDNSChanges();
    bool shouldRestoreDNS() const;
    void logDNSChange(const QStringList &detected, const QStringList &expected);
    void resetFailureCounter();
    void incrementFailureCounter();

#ifdef Q_OS_WINDOWS
    PIP_ADAPTER_DNS_SERVER_ADDRESS_XP m_originalDNSConfig;
    bool setDNSServersWindows(const QStringList &dnsServers);
    bool restoreDefaultDNSWindows();
    QStringList getCurrentDNSServersWindows() const;
    bool getIfaceName();
    
    // New helper methods for Windows
    bool isRunAsAdmin();
    bool verifyIfaceStatus();
    bool verifyDNSSettings(const QStringList &expected, const QStringList &current);
    void updateOriginalDNSServers();
    bool restoreDNSFromList(const QString &iface, const QStringList &dnsList);
    
    // Registry-based DNS management methods
    bool setDNSServersWindowsRegistry(ulong ifIndex, const QStringList &dnsServers);
    bool restoreDNSWindowsRegistry(ulong ifIndex);
    QStringList getCurrentDNSServersWindowsRegistry(ulong ifIndex) const;
    QString getInterfaceRegistryPath(ulong ifIndex) const;
    QString getInterfaceNameByIndex(ulong ifIndex) const;
    bool saveOriginalDNSForInterface(ulong ifIndex);
#endif

#ifdef DAP_OS_LINUX
    // Linux-specific methods are already declared in public section
#endif

#ifdef Q_OS_MACOS
    mutable SCDynamicStoreRef m_store;
    CFRunLoopSourceRef m_runLoopSource;
    CFStringRef m_primaryServiceID;
    bool m_scMonitoringActive;
    
    bool setDNSServersMacOS(const QStringList &dnsServers);
    bool restoreDefaultDNSMacOS();
    QStringList getCurrentDNSServersMacOS() const;
    QStringList getCurrentDNSServersViaNetworksetup() const;
    
    // New methods for proper macOS DNS monitoring
    bool setupSCDynamicStoreMonitoring();
    void teardownSCDynamicStoreMonitoring();
    QString getPrimaryNetworkServiceID() const;
    bool setDNSForService(const QString &serviceID, const QStringList &dnsServers);
    QStringList getDNSForService(const QString &serviceID) const;
    
    // Static callback for SCDynamicStore
    static void dnsChangeCallback(SCDynamicStoreRef store, CFArrayRef changedKeys, void *info);
#endif

#ifdef Q_OS_ANDROID
    mutable QAndroidJniObject m_activity;
    bool setDNSServersAndroid(const QStringList &dnsServers);
    bool restoreDefaultDNSAndroid();
    QStringList getCurrentDNSServersAndroid() const;
#endif

#ifdef Q_OS_IOS
    bool setDNSServersIOS(const QStringList &dnsServers);
    bool restoreDefaultDNSIOS();
    QStringList getCurrentDNSServersIOS() const;
#endif

    // === DNS validation ===
    bool validateDNSServers(const QStringList &dnsServers) const;
    
    // === Common helper methods (cross-platform) ===
    QString escapeInterfaceName(const QString &interfaceName) const;
};

#endif // DAPDNSCONTROLLER_H 
