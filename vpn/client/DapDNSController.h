#ifndef DAPDNSCONTROLLER_H
#define DAPDNSCONTROLLER_H

#include <QObject>
#include <QString>
#include <QStringList>

#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <jni.h>
#endif

#ifdef Q_OS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iphlpapi.h>
#include <windns.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "dnsapi.lib")
#pragma comment(lib, "ws2_32.lib")

// Define Windows-specific constants if not defined
#ifndef GAA_FLAG_INCLUDE_PREFIX
#define GAA_FLAG_INCLUDE_PREFIX 0x0010
#endif

#ifndef DNS_CONFIG_FLAG_ALLOC
#define DNS_CONFIG_FLAG_ALLOC 0x0001
#endif

#ifndef DnsConfigDnsServerList
#define DnsConfigDnsServerList 0x0003
#endif

// Define DNS_CONFIG_TYPE if not already defined
#ifndef DNS_CONFIG_TYPE
typedef enum _DNS_CONFIG_TYPE {
    DnsConfigPrimaryDomainName_W = 0,
    DnsConfigPrimaryDomainName_A = 1,
    DnsConfigPrimaryDomainName_UTF8 = 2,
    DnsConfigAdapterDomainName_W = 3,
    DnsConfigAdapterDomainName_A = 4,
    DnsConfigAdapterDomainName_UTF8 = 5,
    DnsConfigDnsServerList = 6,
    DnsConfigSearchList = 7,
    DnsConfigAdapterInfo = 8,
    DnsConfigSetHostName = 9,
    DnsConfigHostName = 10,
    DnsConfigFullHostName = 11,
    DnsConfigNameServer = 12
} DNS_CONFIG_TYPE;
#endif

// Forward declarations for Windows types
typedef struct _IP_ADAPTER_UNICAST_ADDRESS {
    union {
        ULONGLONG Alignment;
        struct {
            ULONG Length;
            DWORD Flags;
        };
    };
    struct _IP_ADAPTER_UNICAST_ADDRESS* Next;
    SOCKET_ADDRESS Address;
} IP_ADAPTER_UNICAST_ADDRESS, *PIP_ADAPTER_UNICAST_ADDRESS;

typedef struct _IP_ADAPTER_ANYCAST_ADDRESS {
    union {
        ULONGLONG Alignment;
        struct {
            ULONG Length;
            DWORD Flags;
        };
    };
    struct _IP_ADAPTER_ANYCAST_ADDRESS* Next;
    SOCKET_ADDRESS Address;
} IP_ADAPTER_ANYCAST_ADDRESS, *PIP_ADAPTER_ANYCAST_ADDRESS;

typedef struct _IP_ADAPTER_MULTICAST_ADDRESS {
    union {
        ULONGLONG Alignment;
        struct {
            ULONG Length;
            DWORD Flags;
        };
    };
    struct _IP_ADAPTER_MULTICAST_ADDRESS* Next;
    SOCKET_ADDRESS Address;
} IP_ADAPTER_MULTICAST_ADDRESS, *PIP_ADAPTER_MULTICAST_ADDRESS;

typedef struct _IP_ADAPTER_PREFIX {
    union {
        ULONGLONG Alignment;
        struct {
            ULONG Length;
            DWORD Flags;
        };
    };
    struct _IP_ADAPTER_PREFIX* Next;
    SOCKET_ADDRESS Address;
    ULONG PrefixLength;
} IP_ADAPTER_PREFIX, *PIP_ADAPTER_PREFIX;

// Define structure for Windows XP and above
typedef struct _IP_ADAPTER_DNS_SERVER_ADDRESS_XP {
    union {
        struct sockaddr Address;
        struct sockaddr_in AddressIn;
    };
    struct _IP_ADAPTER_DNS_SERVER_ADDRESS_XP* Next;
} IP_ADAPTER_DNS_SERVER_ADDRESS_XP, *PIP_ADAPTER_DNS_SERVER_ADDRESS_XP;

// Define main adapter addresses structure
typedef struct _IP_ADAPTER_ADDRESSES {
    union {
        ULONGLONG Alignment;
        struct {
            ULONG Length;
            DWORD IfIndex;
        };
    };
    struct _IP_ADAPTER_ADDRESSES* Next;
    PCHAR AdapterName;
    PIP_ADAPTER_UNICAST_ADDRESS FirstUnicastAddress;
    PIP_ADAPTER_ANYCAST_ADDRESS FirstAnycastAddress;
    PIP_ADAPTER_MULTICAST_ADDRESS FirstMulticastAddress;
    PIP_ADAPTER_DNS_SERVER_ADDRESS_XP FirstDnsServerAddress;
    PWCHAR Description;
    PWCHAR FriendlyName;
    BYTE PhysicalAddress[MAX_ADAPTER_ADDRESS_LENGTH];
    DWORD PhysicalAddressLength;
    DWORD Flags;
    DWORD Mtu;
    DWORD IfType;
    IF_OPER_STATUS OperStatus;
    DWORD Ipv6IfIndex;
    DWORD ZoneIndices[16];
    PIP_ADAPTER_PREFIX FirstPrefix;
} IP_ADAPTER_ADDRESSES, *PIP_ADAPTER_ADDRESSES;

// Declare Windows API functions
extern "C" {
    DWORD WINAPI GetAdaptersAddresses(
        ULONG Family,
        DWORD Flags,
        PVOID Reserved,
        PIP_ADAPTER_ADDRESSES AdapterAddresses,
        PULONG SizePointer
    );

    DWORD WINAPI DnsSetConfig(
        DNS_CONFIG_TYPE Config,
        DWORD Flag,
        PVOID pConfigInfo,
        PDWORD pConfigInfoSize
    );
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
    PIP_ADAPTER_DNS_SERVER_ADDRESS_XP m_originalDNSConfig;
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
