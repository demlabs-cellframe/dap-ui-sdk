#include "DapDNSController.h"
#include <QDebug>
#include <QProcess>
#include <QHostAddress>
#include <QFile>
#include <QTextStream>

#ifdef Q_OS_WINDOWS
#include <windows.h>
#include <iphlpapi.h>
#include <windns.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "dnsapi.lib")
#pragma comment(lib, "ws2_32.lib")
#endif

DapDNSController::DapDNSController(QObject *parent)
    : QObject(parent)
    , m_isDNSSet(false)
#ifdef Q_OS_WINDOWS
    , m_originalDNSConfig(nullptr)
#endif
#ifdef Q_OS_MACOS
    , m_store(nullptr)
#endif
{
    // Save current DNS settings
    m_originalDNSServers = getCurrentDNSServers();
}

DapDNSController::~DapDNSController()
{
    if (m_isDNSSet) {
        restoreDefaultDNS();
    }
}

bool DapDNSController::setDNSServers(const QStringList &dnsServers)
{
    if (dnsServers.isEmpty()) {
        qWarning() << "Empty DNS servers list provided";
        emit errorOccurred("Empty DNS servers list provided");
        return false;
    }

    // Validate IP addresses
    for (const QString &dnsServer : dnsServers) {
        if (!isValidIPAddress(dnsServer)) {
            qWarning() << "Invalid IP address provided:" << dnsServer;
            emit errorOccurred(QString("Invalid IP address provided: %1").arg(dnsServer));
            return false;
        }
    }

    bool result = false;
#ifdef Q_OS_WINDOWS
    result = setDNSServersWindows(dnsServers);
#elif defined(Q_OS_LINUX)
    result = setDNSServersLinux(dnsServers);
#elif defined(Q_OS_MACOS)
    result = setDNSServersMacOS(dnsServers);
#elif defined(Q_OS_ANDROID)
    result = setDNSServersAndroid(dnsServers);
#elif defined(Q_OS_IOS)
    result = setDNSServersIOS(dnsServers);
#endif

    if (result) {
        m_isDNSSet = true;
        emit dnsServersChanged(dnsServers);
    } else {
        emit errorOccurred("Failed to set DNS servers");
    }
    return result;
}

bool DapDNSController::restoreDefaultDNS()
{
    if (m_originalDNSServers.isEmpty()) {
        qWarning() << "No original DNS servers to restore";
        emit errorOccurred("No original DNS servers to restore");
        return false;
    }

    bool result = false;
#ifdef Q_OS_WINDOWS
    result = restoreDefaultDNSWindows();
#elif defined(Q_OS_LINUX)
    result = restoreDefaultDNSLinux();
#elif defined(Q_OS_MACOS)
    result = restoreDefaultDNSMacOS();
#elif defined(Q_OS_ANDROID)
    result = restoreDefaultDNSAndroid();
#elif defined(Q_OS_IOS)
    result = restoreDefaultDNSIOS();
#endif

    if (result) {
        m_isDNSSet = false;
        emit dnsRestored();
    } else {
        emit errorOccurred("Failed to restore original DNS servers");
    }
    return result;
}

QStringList DapDNSController::getCurrentDNSServers()
{
    QStringList servers;
#ifdef Q_OS_WINDOWS
    servers = getCurrentDNSServersWindows();
#elif defined(Q_OS_LINUX)
    servers = getCurrentDNSServersLinux();
#elif defined(Q_OS_MACOS)
    servers = getCurrentDNSServersMacOS();
#elif defined(Q_OS_ANDROID)
    servers = getCurrentDNSServersAndroid();
#elif defined(Q_OS_IOS)
    servers = getCurrentDNSServersIOS();
#else
    servers = QStringList();
#endif

    if (servers.isEmpty()) {
        qWarning() << "Failed to get current DNS servers";
        emit errorOccurred("Failed to get current DNS servers");
    }
    return servers;
}

bool DapDNSController::isDNSSet()
{
    return m_isDNSSet;
}

// Helper function to validate IP address
bool DapDNSController::isValidIPAddress(const QString &ipAddress)
{
    QHostAddress address(ipAddress);
    return !address.isNull() && address.protocol() == QAbstractSocket::IPv4Protocol;
}

// Helper function to execute commands silently
int DapDNSController::exec_silent(const QString &cmd)
{
    QProcess process;
    QStringList args = QProcess::splitCommand(cmd);
    QString program = args.takeFirst();
    process.start(program, args);
    process.waitForFinished();
    return process.exitCode();
}

bool DapDNSController::flushDNSCache()
{
    bool result = false;
#ifdef Q_OS_WINDOWS
    // First try using Windows API
    DWORD dwRetVal = DnsFlushResolverCache();
    if (dwRetVal == ERROR_SUCCESS) {
        result = true;
    } else {
        // If API fails, try command line
        result = exec_silent("ipconfig /flushdns") == 0;
    }
#elif defined(Q_OS_MACOS)
    result = exec_silent("sudo killall -HUP mDNSResponder") == 0;
#elif defined(Q_OS_LINUX)
    // Try different commands depending on the distribution
    result = exec_silent("systemd-resolve --flush-caches") == 0;
    if (!result) {
        result = exec_silent("service nscd restart") == 0;
    }
    if (!result) {
        result = exec_silent("service dnsmasq restart") == 0;
    }
#endif

    if (!result) {
        qWarning() << "Failed to flush DNS cache";
        emit errorOccurred("Failed to flush DNS cache");
    }
    return result;
}

bool DapDNSController::registerDNS()
{
    bool result = false;
#ifdef Q_OS_WINDOWS
    result = exec_silent("ipconfig /registerdns") == 0;
#elif defined(Q_OS_MACOS)
    result = exec_silent("sudo killall -HUP mDNSResponder") == 0;
#elif defined(Q_OS_LINUX)
    // Linux usually doesn't require explicit DNS registration
    result = true;
#endif

    if (!result) {
        qWarning() << "Failed to register DNS";
        emit errorOccurred("Failed to register DNS");
    }
    return result;
}

#ifdef Q_OS_WINDOWS
bool DapDNSController::setDNSServersWindows(const QStringList &dnsServers)
{
    PIP_ADAPTER_ADDRESSES pAddresses = nullptr;
    ULONG outBufLen = 0;
    ULONG Iterations = 0;
    DWORD dwRetVal = 0;

    // Get buffer size
    GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, nullptr, pAddresses, &outBufLen);
    pAddresses = (PIP_ADAPTER_ADDRESSES)malloc(outBufLen);

    if (pAddresses == nullptr) {
        qWarning() << "Failed to allocate memory for adapter addresses";
        emit errorOccurred("Failed to allocate memory for adapter addresses");
        return false;
    }

    // Get adapter information
    dwRetVal = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, nullptr, pAddresses, &outBufLen);

    if (dwRetVal == NO_ERROR) {
        PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses;
        while (pCurrAddresses) {
            if (pCurrAddresses->OperStatus == IfOperStatusUp) {
                // Find active network adapter
                PIP_ADAPTER_DNS_SERVER_ADDRESS pDnsServer = pCurrAddresses->FirstDnsServerAddress;
                if (pDnsServer) {
                    // Save original settings
                    m_originalDNSConfig = pDnsServer;

                    // Set new DNS servers
                    for (const QString &dnsServer : dnsServers) {
                        // Convert string to IP address
                        struct in_addr addr;
                        inet_pton(AF_INET, dnsServer.toStdString().c_str(), &addr);

                        // Set DNS server
                        DWORD dwRetVal = DnsSetConfig(
                            DnsConfigDnsServerList,
                            DNS_CONFIG_FLAG_ALLOC,
                            &addr,
                            nullptr
                        );

                        if (dwRetVal != ERROR_SUCCESS) {
                            qWarning() << "Failed to set DNS server:" << dnsServer;
                            emit errorOccurred(QString("Failed to set DNS server: %1").arg(dnsServer));
                            free(pAddresses);
                            return false;
                        }
                    }
                    free(pAddresses);
                    return true;
                }
            }
            pCurrAddresses = pCurrAddresses->Next;
        }
    }

    qWarning() << "No active network adapter found";
    emit errorOccurred("No active network adapter found");
    free(pAddresses);
    return false;
}

bool DapDNSController::restoreDefaultDNSWindows()
{
    if (m_originalDNSConfig) {
        DWORD dwRetVal = DnsSetConfig(
            DnsConfigDnsServerList,
            DNS_CONFIG_FLAG_ALLOC,
            m_originalDNSConfig,
            nullptr
        );
        return dwRetVal == ERROR_SUCCESS;
    }
    return false;
}

QStringList DapDNSController::getCurrentDNSServersWindows()
{
    QStringList dnsServers;
    PIP_ADAPTER_ADDRESSES pAddresses = nullptr;
    ULONG outBufLen = 0;
    ULONG Iterations = 0;
    DWORD dwRetVal = 0;

    GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, nullptr, pAddresses, &outBufLen);
    pAddresses = (PIP_ADAPTER_ADDRESSES)malloc(outBufLen);

    if (pAddresses == nullptr) {
        return dnsServers;
    }

    dwRetVal = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, nullptr, pAddresses, &outBufLen);

    if (dwRetVal == NO_ERROR) {
        PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses;
        while (pCurrAddresses) {
            if (pCurrAddresses->OperStatus == IfOperStatusUp) {
                PIP_ADAPTER_DNS_SERVER_ADDRESS pDnsServer = pCurrAddresses->FirstDnsServerAddress;
                while (pDnsServer) {
                    char ipAddress[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &(pDnsServer->Address.lpSockaddr->sa_data[2]), ipAddress, INET_ADDRSTRLEN);
                    dnsServers.append(QString(ipAddress));
                    pDnsServer = pDnsServer->Next;
                }
                break;
            }
            pCurrAddresses = pCurrAddresses->Next;
        }
    }

    free(pAddresses);
    return dnsServers;
}
#endif

#ifdef Q_OS_LINUX
bool DapDNSController::setDNSServersLinux(const QStringList &dnsServers)
{
    // First save original resolv.conf
    if (m_originalDNSServers.isEmpty()) {
        QFile resolvConf("/etc/resolv.conf");
        if (resolvConf.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&resolvConf);
            while (!in.atEnd()) {
                QString line = in.readLine();
                if (line.startsWith("nameserver")) {
                    QString dns = line.split(" ").last();
                    if (isValidIPAddress(dns)) {
                        m_originalDNSServers.append(dns);
                    }
                }
            }
            resolvConf.close();
        }
    }

    // Open resolv.conf for writing
    QFile resolvConf("/etc/resolv.conf");
    if (!resolvConf.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open /etc/resolv.conf for writing";
        emit errorOccurred("Failed to open /etc/resolv.conf for writing");
        return false;
    }

    QTextStream out(&resolvConf);
    // Write new DNS servers
    for (const QString &dnsServer : dnsServers) {
        out << "nameserver " << dnsServer << "\n";
    }
    resolvConf.close();

    // Set correct permissions
    if (chmod("/etc/resolv.conf", 0644) != 0) {
        qWarning() << "Failed to set permissions on /etc/resolv.conf";
        emit errorOccurred("Failed to set permissions on /etc/resolv.conf");
        return false;
    }

    return true;
}

bool DapDNSController::restoreDefaultDNSLinux()
{
    if (m_originalDNSServers.isEmpty()) {
        qWarning() << "No original DNS servers to restore";
        emit errorOccurred("No original DNS servers to restore");
        return false;
    }

    QFile resolvConf("/etc/resolv.conf");
    if (!resolvConf.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open /etc/resolv.conf for writing";
        emit errorOccurred("Failed to open /etc/resolv.conf for writing");
        return false;
    }

    QTextStream out(&resolvConf);
    // Restore original DNS servers
    for (const QString &dnsServer : m_originalDNSServers) {
        out << "nameserver " << dnsServer << "\n";
    }
    resolvConf.close();

    // Set correct permissions
    if (chmod("/etc/resolv.conf", 0644) != 0) {
        qWarning() << "Failed to set permissions on /etc/resolv.conf";
        emit errorOccurred("Failed to set permissions on /etc/resolv.conf");
        return false;
    }

    return true;
}

QStringList DapDNSController::getCurrentDNSServersLinux()
{
    QStringList dnsServers;
    QFile resolvConf("/etc/resolv.conf");
    
    if (resolvConf.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&resolvConf);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.startsWith("nameserver")) {
                QString dns = line.split(" ").last();
                if (isValidIPAddress(dns)) {
                    dnsServers.append(dns);
                }
            }
        }
        resolvConf.close();
    }

    if (dnsServers.isEmpty()) {
        qWarning() << "No DNS servers found in /etc/resolv.conf";
        emit errorOccurred("No DNS servers found in /etc/resolv.conf");
    }

    return dnsServers;
}
#endif

#ifdef Q_OS_MACOS
bool DapDNSController::setDNSServersMacOS(const QStringList &dnsServers)
{
    if (!m_store) {
        m_store = SCDynamicStoreCreate(nullptr, CFSTR("DapDNSController"), nullptr, nullptr);
    }

    if (!m_store) {
        return false;
    }

    // Create DNS servers array
    CFMutableArrayRef dnsArray = CFArrayCreateMutable(nullptr, 0, &kCFTypeArrayCallBacks);
    for (const QString &dnsServer : dnsServers) {
        CFStringRef dnsString = CFStringCreateWithCString(nullptr, dnsServer.toStdString().c_str(), kCFStringEncodingUTF8);
        CFArrayAppendValue(dnsArray, dnsString);
        CFRelease(dnsString);
    }

    // Set DNS servers
    CFStringRef keys[] = { CFSTR("State:/Network/Service/.*/DNS") };
    CFArrayRef keyArray = CFArrayCreate(nullptr, (const void**)keys, 1, &kCFTypeArrayCallBacks);
    
    CFMutableDictionaryRef dnsDict = CFDictionaryCreateMutable(nullptr, 0,
        &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    
    CFDictionarySetValue(dnsDict, CFSTR("ServerAddresses"), dnsArray);
    
    bool result = SCDynamicStoreSetValue(m_store, keys[0], dnsDict);
    
    CFRelease(dnsArray);
    CFRelease(keyArray);
    CFRelease(dnsDict);
    
    return result;
}

bool DapDNSController::restoreDefaultDNSMacOS()
{
    return setDNSServersMacOS(m_originalDNSServers);
}

QStringList DapDNSController::getCurrentDNSServersMacOS()
{
    QStringList dnsServers;
    
    if (!m_store) {
        m_store = SCDynamicStoreCreate(nullptr, CFSTR("DapDNSController"), nullptr, nullptr);
    }

    if (!m_store) {
        return dnsServers;
    }

    CFStringRef keys[] = { CFSTR("State:/Network/Service/.*/DNS") };
    CFArrayRef keyArray = CFArrayCreate(nullptr, (const void**)keys, 1, &kCFTypeArrayCallBacks);
    
    CFDictionaryRef dnsDict = (CFDictionaryRef)SCDynamicStoreCopyValue(m_store, keys[0]);
    
    if (dnsDict) {
        CFArrayRef serverArray = (CFArrayRef)CFDictionaryGetValue(dnsDict, CFSTR("ServerAddresses"));
        if (serverArray) {
            CFIndex count = CFArrayGetCount(serverArray);
            for (CFIndex i = 0; i < count; i++) {
                CFStringRef dnsString = (CFStringRef)CFArrayGetValueAtIndex(serverArray, i);
                char buffer[256];
                CFStringGetCString(dnsString, buffer, sizeof(buffer), kCFStringEncodingUTF8);
                dnsServers.append(QString(buffer));
            }
        }
        CFRelease(dnsDict);
    }
    
    CFRelease(keyArray);
    
    return dnsServers;
}
#endif

#ifdef Q_OS_ANDROID
bool DapDNSController::setDNSServersAndroid(const QStringList &dnsServers)
{
    if (!m_activity.isValid()) {
        return false;
    }

    // Create Java string array
    QAndroidJniEnvironment env;
    jobjectArray dnsArray = env->NewObjectArray(dnsServers.size(), env->FindClass("java/lang/String"), nullptr);
    
    for (int i = 0; i < dnsServers.size(); i++) {
        jstring dnsString = env->NewStringUTF(dnsServers[i].toStdString().c_str());
        env->SetObjectArrayElement(dnsArray, i, dnsString);
        env->DeleteLocalRef(dnsString);
    }

    // Call Java method to set DNS
    bool result = m_activity.callMethod<jboolean>("setDNSServers", "([Ljava/lang/String;)Z", dnsArray);
    
    env->DeleteLocalRef(dnsArray);
    
    return result;
}

bool DapDNSController::restoreDefaultDNSAndroid()
{
    return setDNSServersAndroid(m_originalDNSServers);
}

QStringList DapDNSController::getCurrentDNSServersAndroid()
{
    QStringList dnsServers;
    
    if (!m_activity.isValid()) {
        return dnsServers;
    }

    // Call Java method to get DNS servers
    QAndroidJniObject dnsArray = m_activity.callObjectMethod("getDNSServers", "()[Ljava/lang/String;");
    
    if (dnsArray.isValid()) {
        QAndroidJniEnvironment env;
        jobjectArray array = dnsArray.object<jobjectArray>();
        jsize size = env->GetArrayLength(array);
        
        for (jsize i = 0; i < size; i++) {
            jstring dnsString = (jstring)env->GetObjectArrayElement(array, i);
            const char* dnsChars = env->GetStringUTFChars(dnsString, nullptr);
            dnsServers.append(QString(dnsChars));
            env->ReleaseStringUTFChars(dnsString, dnsChars);
            env->DeleteLocalRef(dnsString);
        }
    }
    
    return dnsServers;
}
#endif

#ifdef Q_OS_IOS
bool DapDNSController::setDNSServersIOS(const QStringList &dnsServers)
{
    // Create DNS servers array
    NSMutableArray *dnsArray = [NSMutableArray arrayWithCapacity:dnsServers.size()];
    for (const QString &dnsServer : dnsServers) {
        [dnsArray addObject:dnsServer.toNSString()];
    }

    // Create DNS configuration
    NEDNSSettings *dnsSettings = [[NEDNSSettings alloc] init];
    dnsSettings.matchDomains = @[@""];
    dnsSettings.matchDomainRules = @[];
    dnsSettings.servers = dnsArray;

    // Set DNS settings
    NSError *error = nil;
    bool result = [dnsSettings applyWithError:&error];
    
    return result;
}

bool DapDNSController::restoreDefaultDNSIOS()
{
    return setDNSServersIOS(m_originalDNSServers);
}

QStringList DapDNSController::getCurrentDNSServersIOS()
{
    QStringList dnsServers;
    
    // Get current DNS settings
    NEDNSSettings *dnsSettings = [NEDNSSettings currentSettings];
    if (dnsSettings) {
        for (NSString *server in dnsSettings.servers) {
            dnsServers.append(QString::fromNSString(server));
        }
    }
    
    return dnsServers;
}
#endif 