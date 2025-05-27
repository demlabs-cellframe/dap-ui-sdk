#ifndef DAPDNSCONTROLLER_H
#define DAPDNSCONTROLLER_H

#include <QObject>
#include <QString>
#include <QStringList>

#ifdef Q_OS_WINDOWS
#include <windows.h>
#include <iphlpapi.h>
#include <windns.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "dnsapi.lib")
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

#ifdef Q_OS_ANDROID
#include <jni.h>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
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
    bool isDNSSet();

    // New methods for DNS cache and registration management
    bool flushDNSCache();
    bool registerDNS();

signals:
    // Signals for change notifications
    void dnsServersChanged(const QStringList &servers);
    void dnsRestored();
    void errorOccurred(const QString &error);

private:
    // Store original DNS settings
    QStringList m_originalDNSServers;
    bool m_isDNSSet;

    // Helper methods
    bool isValidIPAddress(const QString &ipAddress);
    int exec_silent(const QString &cmd);

#ifdef Q_OS_WINDOWS
    IP_ADAPTER_DNS_SERVER_ADDRESS_XP* m_originalDNSConfig;
    bool setDNSServersWindows(const QStringList &dnsServers);
    bool restoreDefaultDNSWindows();
    QStringList getCurrentDNSServersWindows();
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