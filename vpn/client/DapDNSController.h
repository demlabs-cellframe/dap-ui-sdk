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

#ifdef Q_OS_LINUX
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
    QStringList getCurrentDNSServers();
    bool isDNSSet() const;

    // New methods for DNS cache and registration management
    bool flushDNSCache();
    bool registerDNS();

    // Thread-safe accessors
    QString getIfaceName() const;
    void setIfaceName(const QString &name);
    QStringList getOriginalDNSServers() const;
    void setOriginalDNSServers(const QStringList &servers);

    // Helper methods for DNS management
    QStringList getCurrentDNSIndexes(const QString &iface);
    bool resetInterfaceDNS(const QString &iface, bool useDHCP = false);

signals:
    // Signals for change notifications
    void dnsServersChanged(const QStringList &servers);
    void dnsRestored();
    void errorOccurred(const QString &error);

protected:
    // Helper methods
    bool isValidIPAddress(const QString &ipAddress);
    int exec_silent(const QString &cmd);
    bool runNetshCommand(const QString &cmd, QString *output = nullptr, int timeout = 5000);

private:
    // Mutex for thread safety
    mutable QMutex m_mutex;

    // Store original DNS settings
    QStringList m_originalDNSServers;
    bool m_isDNSSet;
    QString m_ifaceName;  // Store network interface name for DNS operations

#ifdef Q_OS_WINDOWS
    PIP_ADAPTER_DNS_SERVER_ADDRESS_XP m_originalDNSConfig;
    bool setDNSServersWindows(const QStringList &dnsServers);
    bool restoreDefaultDNSWindows();
    QStringList getCurrentDNSServersWindows();
    bool getIfaceName();
    
    // New helper methods for Windows
    bool isRunAsAdmin();
    bool verifyIfaceStatus();
    bool verifyDNSSettings(const QStringList &expected, const QStringList &current);
    void updateOriginalDNSServers();
    bool restoreDNSFromList(const QString &iface, const QStringList &dnsList);
#endif

#ifdef Q_OS_LINUX
    bool setDNSServersLinux(const QStringList &dnsServers);
    bool restoreDefaultDNSLinux();
    QStringList getCurrentDNSServersLinux();
#endif

#ifdef Q_OS_MACOS
    SCDynamicStoreRef m_store;
    bool setDNSServersMacOS(const QStringList &dnsServers);
    bool restoreDefaultDNSMacOS();
    QStringList getCurrentDNSServersMacOS();
#endif

#ifdef Q_OS_ANDROID
    QAndroidJniObject m_activity;
    bool setDNSServersAndroid(const QStringList &dnsServers);
    bool restoreDefaultDNSAndroid();
    QStringList getCurrentDNSServersAndroid();
#endif

#ifdef Q_OS_IOS
    bool setDNSServersIOS(const QStringList &dnsServers);
    bool restoreDefaultDNSIOS();
    QStringList getCurrentDNSServersIOS();
#endif
};

#endif // DAPDNSCONTROLLER_H 
