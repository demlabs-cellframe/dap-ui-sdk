#include "DapDNSController.h"
#include <QDebug>
#include <QProcess>
#include <QHostAddress>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

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
    return !address.isNull() && (address.protocol() == QAbstractSocket::IPv4Protocol || 
                                address.protocol() == QAbstractSocket::IPv6Protocol);
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
namespace {
    // Helper function to get adapter addresses with proper error handling
    std::pair<PIP_ADAPTER_ADDRESSES, DWORD> getAdapterAddresses(ULONG flags) {
        ULONG outBufLen = 0;
        PIP_ADAPTER_ADDRESSES pAddresses = nullptr;
        
        // Get required buffer size
        DWORD dwRetVal = GetAdaptersAddresses(AF_UNSPEC, flags, nullptr, nullptr, &outBufLen);
        if (dwRetVal != ERROR_BUFFER_OVERFLOW) {
            qWarning() << "Failed to get adapter addresses buffer size, error:" << dwRetVal;
            return {nullptr, dwRetVal};
        }

        // Allocate memory
        pAddresses = (PIP_ADAPTER_ADDRESSES)malloc(outBufLen);
        if (!pAddresses) {
            qWarning() << "Failed to allocate" << outBufLen << "bytes for adapter addresses";
            return {nullptr, ERROR_NOT_ENOUGH_MEMORY};
        }

        // Get actual data
        dwRetVal = GetAdaptersAddresses(AF_UNSPEC, flags, nullptr, pAddresses, &outBufLen);
        if (dwRetVal != NO_ERROR) {
            free(pAddresses);
            pAddresses = nullptr;
            qWarning() << "GetAdaptersAddresses failed with error:" << dwRetVal;
            return {nullptr, dwRetVal};
        }

        return {pAddresses, NO_ERROR};
    }
}

bool DapDNSController::getInterfaceName()
{
    bool found = false;
    PIP_ADAPTER_ADDRESSES pAddresses = nullptr;

    // First try to get the best interface using GetBestInterface
    DWORD bestIfIndex = 0;
    struct sockaddr_in destAddr;
    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_addr.s_addr = inet_addr("8.8.8.8"); // Google DNS as a reference point

    if (destAddr.sin_addr.s_addr == INADDR_NONE) {
        qWarning() << "inet_addr failed for 8.8.8.8";
        emit errorOccurred("Failed to resolve reference IP address");
        return false;
    }

    DWORD bestIfResult = GetBestInterfaceEx((struct sockaddr*)&destAddr, &bestIfIndex);
    if (bestIfResult != NO_ERROR) {
        qWarning() << "GetBestInterfaceEx failed with code:" << GetLastError();
        emit errorOccurred("Failed to determine best network interface");
    } else {
        // Try to find the best interface first
        auto result = getAdapterAddresses(GAA_FLAG_INCLUDE_PREFIX | GAA_FLAG_INCLUDE_GATEWAYS);
        pAddresses = result.first;
        DWORD error = result.second;

        if (error == NO_ERROR && pAddresses) {
            PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses;
            while (pCurrAddresses && !found) {
                if (pCurrAddresses->IfIndex == bestIfIndex || 
                    pCurrAddresses->Ipv6IfIndex == bestIfIndex) {
                    
                    // Check if adapter is up and has gateway
                    if (pCurrAddresses->OperStatus == IfOperStatusUp && 
                        pCurrAddresses->FirstGatewayAddress != nullptr) {
                        
                        // Check for IPv4 or IPv6 connectivity
                        PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pCurrAddresses->FirstUnicastAddress;
                        while (pUnicast && !found) {
                            if (pUnicast->Address.lpSockaddr->sa_family == AF_INET ||
                                pUnicast->Address.lpSockaddr->sa_family == AF_INET6) {
                                
                                m_interfaceName = QString::fromWCharArray(pCurrAddresses->FriendlyName);
                                qDebug() << "Found best interface:" 
                                        << QString::fromWCharArray(pCurrAddresses->FriendlyName)
                                        << "(" << QString::fromUtf16((const ushort*)pCurrAddresses->Description) << ")";
                                found = true;
                                break;
                            }
                            pUnicast = pUnicast->Next;
                        }
                    }
                }
                pCurrAddresses = pCurrAddresses->Next;
            }
            free(pAddresses);
            pAddresses = nullptr;
        }
    }

    // If best interface method failed, try fallback method
    if (!found) {
        auto result = getAdapterAddresses(GAA_FLAG_INCLUDE_PREFIX | GAA_FLAG_INCLUDE_GATEWAYS);
        pAddresses = result.first;
        DWORD error = result.second;

        if (error == NO_ERROR && pAddresses) {
            PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses;
            while (pCurrAddresses && !found) {
                // Check if adapter is up, not loopback, and has gateway
                if (pCurrAddresses->OperStatus == IfOperStatusUp &&
                    pCurrAddresses->IfType != IF_TYPE_SOFTWARE_LOOPBACK &&
                    pCurrAddresses->FirstGatewayAddress != nullptr) {
                    
                    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pCurrAddresses->FirstUnicastAddress;
                    while (pUnicast && !found) {
                        if (pUnicast->Address.lpSockaddr->sa_family == AF_INET ||
                            pUnicast->Address.lpSockaddr->sa_family == AF_INET6) {
                            
                            m_interfaceName = QString::fromWCharArray(pCurrAddresses->FriendlyName);
                            qDebug() << "Found fallback interface:" 
                                    << QString::fromWCharArray(pCurrAddresses->FriendlyName)
                                    << "(" << QString::fromUtf16((const ushort*)pCurrAddresses->Description) << ")";
                            found = true;
                            break;
                        }
                        pUnicast = pUnicast->Next;
                    }
                }
                pCurrAddresses = pCurrAddresses->Next;
            }
            free(pAddresses);
            pAddresses = nullptr;
        }
    }

    if (!found) {
        qWarning() << "No suitable network interface found";
        emit errorOccurred("No suitable network interface found");
        m_interfaceName.clear();
        return false;
    }

    return true;
}

bool DapDNSController::runNetshCommand(const QString &cmd, QString *output, int timeout)
{
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    
    process.start("cmd.exe", QStringList() << "/c" << cmd);
    
    if (!process.waitForStarted(timeout)) {
        qWarning() << "Failed to start command:" << cmd << "Error:" << process.errorString();
        emit errorOccurred(QString("Failed to start command: %1").arg(process.errorString()));
        return false;
    }

    if (!process.waitForFinished(timeout)) {
        qWarning() << "Command timed out after" << timeout << "ms:" << cmd;
        emit errorOccurred(QString("Command timed out after %1 ms").arg(timeout));
        process.kill();
        return false;
    }

    QByteArray processOutput = process.readAll();
    QString outputStr = QString::fromLocal8Bit(processOutput);

    if (output) {
        *output = outputStr;
    }

    // Check exit code first
    int exitCode = process.exitCode();
    if (exitCode != 0) {
        // Check for specific error conditions
        if (exitCode == ERROR_ELEVATION_REQUIRED || 
            outputStr.contains("The requested operation requires elevation") ||
            outputStr.contains("требует повышения прав")) {
            qWarning() << "Command requires elevation:" << cmd;
            emit errorOccurred("The operation requires administrator privileges");
            return false;
        }

        qWarning() << "Command failed with exit code" << exitCode << ":" << cmd
                  << "Output:" << outputStr;
        emit errorOccurred(QString("Command failed with exit code %1: %2")
                         .arg(exitCode)
                         .arg(outputStr.trimmed()));
        return false;
    }

    // Check for common error patterns even if exit code is 0
    if (outputStr.contains("The system cannot find the file specified") ||
        outputStr.contains("Не удается найти указанный файл") ||
        outputStr.contains("The command failed to complete successfully") ||
        outputStr.contains("Не удалось выполнить команду")) {
        qWarning() << "Command failed:" << cmd << "Output:" << outputStr;
        emit errorOccurred(QString("Command failed: %1").arg(outputStr.trimmed()));
        return false;
    }

    return true;
}

bool DapDNSController::verifyDNSSettings(const QStringList &expected, const QStringList &current)
{
    if (expected.isEmpty() || current.isEmpty()) {
        qWarning() << "Expected or current DNS list is empty"
                  << "Expected:" << expected 
                  << "Current:" << current;
        emit errorOccurred("DNS settings verification failed: empty DNS list");
        return false;
    }

    // Use QSet for order-independent comparison
    QSet<QString> expectedSet(expected.begin(), expected.end());
    QSet<QString> currentSet(current.begin(), current.end());

    if (expectedSet != currentSet) {
        qWarning() << "DNS settings mismatch."
                  << "Expected:" << expected 
                  << "Got:" << current
                  << "Missing:" << (expectedSet - currentSet).values()
                  << "Extra:" << (currentSet - expectedSet).values();
        emit errorOccurred("DNS settings verification failed: mismatched servers");
        return false;
    }

    return true;
}

bool DapDNSController::setDNSServersWindows(const QStringList &dnsServers)
{
    // Update original DNS servers if not set
    updateOriginalDNSServers();

    // Validate input parameters
    if (dnsServers.isEmpty()) {
        qWarning() << "Empty DNS servers list provided";
        emit errorOccurred("Empty DNS servers list provided");
        return false;
    }

    // Validate each DNS server address
    for (const QString &dns : dnsServers) {
        QHostAddress addr(dns);
        if (addr.isNull() || (addr.protocol() != QAbstractSocket::IPv4Protocol && 
                             addr.protocol() != QAbstractSocket::IPv6Protocol)) {
            qWarning() << "Invalid DNS server address:" << dns;
            emit errorOccurred(QString("Invalid DNS server address: %1").arg(dns));
            return false;
        }
    }

    // Check administrator privileges
    if (!isRunAsAdmin()) {
        qWarning() << "Application is not running with administrator privileges";
        emit errorOccurred("DNS settings cannot be changed without administrator privileges");
        return false;
    }

    // Get interface name if not already set
    if (m_interfaceName.isEmpty() && !getInterfaceName()) {
        qWarning() << "Failed to get interface name";
        emit errorOccurred("Failed to get interface name");
        return false;
    }

    // Verify interface is connected and operational
    if (!verifyInterfaceStatus()) {
        return false;
    }

    // Reset current DNS settings
    QString resetCmd = QString("netsh interface ip set dns name=\"%1\" source=none")
        .arg(m_interfaceName);
    
    if (!runNetshCommand(resetCmd)) {
        qWarning() << "Failed to reset DNS settings";
        emit errorOccurred("Failed to reset DNS settings");
        return false;
    }

    // Set primary DNS server
    QString cmd = QString("netsh interface ip set dns name=\"%1\" static %2 primary")
        .arg(m_interfaceName)
        .arg(dnsServers.first());
    
    if (!runNetshCommand(cmd)) {
        qWarning() << "Failed to set primary DNS server";
        emit errorOccurred("Failed to set primary DNS server");
        return false;
    }

    // Add additional DNS servers
    for (int i = 1; i < dnsServers.size(); ++i) {
        cmd = QString("netsh interface ip add dns name=\"%1\" addr=%2 index=%3")
            .arg(m_interfaceName)
            .arg(dnsServers[i])
            .arg(i + 1);
        
        if (!runNetshCommand(cmd)) {
            qWarning() << "Failed to add DNS server:" << dnsServers[i];
            emit errorOccurred(QString("Failed to add DNS server: %1").arg(dnsServers[i]));
            return false;
        }
    }

    // Verify DNS settings were applied correctly
    QStringList currentDNS = getCurrentDNSServersWindows();
    if (!verifyDNSSettings(dnsServers, currentDNS)) {
        // Try to restore original DNS settings
        if (!m_originalDNSServers.isEmpty()) {
            restoreDNSFromList(m_interfaceName, m_originalDNSServers);
        }
        return false;
    }

    // Clear DNS cache
    if (!flushDNSCache()) {
        qWarning() << "Failed to flush DNS cache";
        // Not considered an error since DNS servers are already set and verified
    }

    // Register DNS settings
    if (!registerDNS()) {
        qWarning() << "Failed to register DNS settings";
        // Not considered a critical error
    }

    emit dnsServersChanged(dnsServers);
    return true;
}

bool DapDNSController::restoreDefaultDNSWindows()
{
    // Check administrator privileges
    if (!isRunAsAdmin()) {
        qWarning() << "Application is not running with administrator privileges";
        emit errorOccurred("DNS settings cannot be restored without administrator privileges");
        return false;
    }

    // Check for saved DNS servers
    if (m_originalDNSServers.isEmpty()) {
        qWarning() << "No original DNS servers to restore";
        emit errorOccurred("No original DNS servers to restore");
        return false;
    }

    // Validate saved DNS addresses
    for (const QString &dns : m_originalDNSServers) {
        QHostAddress addr(dns);
        if (addr.isNull() || (addr.protocol() != QAbstractSocket::IPv4Protocol && 
                             addr.protocol() != QAbstractSocket::IPv6Protocol)) {
            qWarning() << "Invalid saved DNS server address:" << dns;
            emit errorOccurred(QString("Invalid saved DNS server address: %1").arg(dns));
            return false;
        }
    }

    // Check and get interface name if not set
    if (m_interfaceName.isEmpty() && !getInterfaceName()) {
        qWarning() << "Failed to get interface name";
        emit errorOccurred("Failed to get interface name");
        return false;
    }

    // Verify interface is connected and operational
    if (!verifyInterfaceStatus()) {
        return false;
    }

    // Reset current DNS settings
    QString resetCmd = QString("netsh interface ip set dns name=\"%1\" source=none")
        .arg(m_interfaceName);
    
    if (!runNetshCommand(resetCmd)) {
        qWarning() << "Failed to reset DNS settings";
        emit errorOccurred("Failed to reset DNS settings");
        return false;
    }

    // Restore DNS servers
    QString cmd = QString("netsh interface ip set dns name=\"%1\" static %2 primary")
        .arg(m_interfaceName)
        .arg(m_originalDNSServers.first());
    
    if (!runNetshCommand(cmd)) {
        qWarning() << "Failed to restore primary DNS server";
        emit errorOccurred("Failed to restore primary DNS server");
        return false;
    }

    // Add additional DNS servers
    for (int i = 1; i < m_originalDNSServers.size(); ++i) {
        cmd = QString("netsh interface ip add dns name=\"%1\" addr=%2 index=%3")
            .arg(m_interfaceName)
            .arg(m_originalDNSServers[i])
            .arg(i + 1);
        
        if (!runNetshCommand(cmd)) {
            qWarning() << "Failed to restore DNS server:" << m_originalDNSServers[i];
            emit errorOccurred(QString("Failed to restore DNS server: %1").arg(m_originalDNSServers[i]));
            return false;
        }
    }

    // Verify DNS settings were restored correctly
    QStringList currentDNS = getCurrentDNSServersWindows();
    if (!verifyDNSSettings(m_originalDNSServers, currentDNS)) {
        return false;
    }

    // Clear DNS cache
    if (!flushDNSCache()) {
        qWarning() << "Failed to flush DNS cache after restoring DNS servers";
        // Not considered a critical error since DNS servers are already restored and verified
    }

    // Register DNS settings
    if (!registerDNS()) {
        qWarning() << "Failed to register restored DNS settings";
        // Not considered a critical error
    }

    emit dnsRestored();
    return true;
}

bool DapDNSController::restoreDNSFromList(const QString &interface, const QStringList &dnsList)
{
    if (interface.isEmpty()) {
        qWarning() << "Empty interface name provided";
        emit errorOccurred("Empty interface name provided");
        return false;
    }

    if (dnsList.isEmpty()) {
        qWarning() << "Empty DNS servers list provided";
        emit errorOccurred("Empty DNS servers list provided");
        return false;
    }

    // First reset current DNS settings
    QString resetCmd = QString("netsh interface ip set dns name=\"%1\" source=none")
        .arg(interface);
    
    if (!runNetshCommand(resetCmd)) {
        qWarning() << "Failed to reset DNS settings for interface" << interface;
        emit errorOccurred(QString("Failed to reset DNS settings for interface %1").arg(interface));
        return false;
    }

    // Set primary DNS server
    QString cmd = QString("netsh interface ip set dns name=\"%1\" static %2 primary")
        .arg(interface)
        .arg(dnsList.first());

    if (!runNetshCommand(cmd)) {
        qWarning() << "Failed to set primary DNS server" << dnsList.first() << "for interface" << interface;
        emit errorOccurred(QString("Failed to set primary DNS server %1").arg(dnsList.first()));
        return false;
    }

    // Add remaining DNS servers
    for (int i = 1; i < dnsList.size(); ++i) {
        cmd = QString("netsh interface ip add dns name=\"%1\" addr=%2 index=%3")
            .arg(interface)
            .arg(dnsList[i])
            .arg(i + 1);

        if (!runNetshCommand(cmd)) {
            qWarning() << "Failed to add DNS server" << dnsList[i] << "with index" << (i + 1) 
                      << "for interface" << interface;
            emit errorOccurred(QString("Failed to add DNS server %1").arg(dnsList[i]));
            return false;
        }
    }

    return true;
}

QStringList DapDNSController::getCurrentDNSServersWindows()
{
    QStringList dnsServers;
    ULONG outBufLen = 0;
    DWORD dwRetVal = 0;
    PIP_ADAPTER_ADDRESSES pAddresses = nullptr;
    constexpr ULONG INITIAL_BUFFER_SIZE = 15000;

    // Flags to optimize and get only required data
    const ULONG flags = GAA_FLAG_SKIP_ANYCAST | 
                       GAA_FLAG_SKIP_MULTICAST | 
                       GAA_FLAG_SKIP_FRIENDLY_NAME;

    // First attempt with fixed buffer size
    outBufLen = INITIAL_BUFFER_SIZE;
    pAddresses = (PIP_ADAPTER_ADDRESSES)malloc(outBufLen);
    if (!pAddresses) {
        qWarning() << "Failed to allocate memory for adapter addresses";
        return dnsServers;
    }

    // Get adapter information
    dwRetVal = GetAdaptersAddresses(AF_UNSPEC, flags, nullptr, pAddresses, &outBufLen);
    
    // If buffer is too small, reallocate memory
    if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
        free(pAddresses); // Free the initial allocation
        pAddresses = nullptr;
        
        pAddresses = (PIP_ADAPTER_ADDRESSES)malloc(outBufLen);
        if (!pAddresses) {
            qWarning() << "Failed to allocate memory for adapter addresses after size query";
            return dnsServers;
        }
        dwRetVal = GetAdaptersAddresses(AF_UNSPEC, flags, nullptr, pAddresses, &outBufLen);
    }

    if (dwRetVal == NO_ERROR) {
        // Iterate through all adapters
        for (PIP_ADAPTER_ADDRESSES adapter = pAddresses; adapter; adapter = adapter->Next) {
            // Skip inactive adapters
            if (adapter->OperStatus != IfOperStatusUp) {
                continue;
            }

            // Get DNS servers for current adapter
            for (PIP_ADAPTER_DNS_SERVER_ADDRESS dnsAddr = adapter->FirstDnsServerAddress;
                 dnsAddr; 
                 dnsAddr = dnsAddr->Next) 
            {
                if (!dnsAddr->Address.lpSockaddr) {
                    continue;
                }

                char ipStr[INET6_ADDRSTRLEN] = {};
                QString ipAddress;

                switch (dnsAddr->Address.lpSockaddr->sa_family) {
                    case AF_INET: {
                        struct sockaddr_in* ipv4 = (struct sockaddr_in*)dnsAddr->Address.lpSockaddr;
                        if (inet_ntop(AF_INET, &(ipv4->sin_addr), ipStr, INET_ADDRSTRLEN)) {
                            ipAddress = QString::fromLatin1(ipStr);
                        }
                        break;
                    }
                    case AF_INET6: {
                        struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)dnsAddr->Address.lpSockaddr;
                        if (inet_ntop(AF_INET6, &(ipv6->sin6_addr), ipStr, INET6_ADDRSTRLEN)) {
                            ipAddress = QString::fromLatin1(ipStr);
                        }
                        break;
                    }
                    default:
                        continue; // Skip unknown address types
                }

                // Add only unique addresses
                if (!ipAddress.isEmpty() && !dnsServers.contains(ipAddress)) {
                    dnsServers.append(ipAddress);
                    qDebug() << "Found DNS server:" << ipAddress 
                            << "on adapter:" << QString::fromUtf16((const ushort*)adapter->Description);
                }
            }
        }
    } else {
        qWarning() << "GetAdaptersAddresses failed with error:" << dwRetVal;
    }

    if (pAddresses) {
        free(pAddresses);
    }

    if (dnsServers.isEmpty()) {
        qWarning() << "No DNS servers found on any active network adapter";
    }

    return dnsServers;
}

bool DapDNSController::verifyInterfaceStatus()
{
    if (m_interfaceName.isEmpty()) {
        qWarning() << "Interface name is empty";
        emit errorOccurred("Interface name is empty");
        return false;
    }

    QString output;
    QString cmd = QString("netsh interface show interface \"%1\"").arg(m_interfaceName);
    
    if (!runNetshCommand(cmd, &output)) {
        return false;
    }

    if (!output.contains("Connect state: Connected")) {
        qWarning() << "Interface is not connected:" << m_interfaceName;
        emit errorOccurred(QString("Interface %1 is not in connected state").arg(m_interfaceName));
        return false;
    }

    return true;
}

bool DapDNSController::isRunAsAdmin()
{
    HANDLE hToken = nullptr;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        qWarning() << "Failed to open process token:" << GetLastError();
        return false;
    }

    TOKEN_ELEVATION elevation;
    DWORD dwSize;
    if (!GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)) {
        qWarning() << "Failed to get token information:" << GetLastError();
        CloseHandle(hToken);
        return false;
    }

    CloseHandle(hToken);
    return elevation.TokenIsElevated != 0;
}

void DapDNSController::updateOriginalDNSServers()
{
    if (m_originalDNSServers.isEmpty()) {
        m_originalDNSServers = getCurrentDNSServersWindows();
        if (!m_originalDNSServers.isEmpty()) {
            qDebug() << "Cached original DNS servers:" << m_originalDNSServers;
        }
    }
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

    // Validate that we have exactly one DNS server
    if (dnsServers.size() != 1) {
        qWarning() << "Expected exactly one DNS server, got" << dnsServers.size();
        emit errorOccurred("Expected exactly one DNS server");
        return false;
    }

    // Open resolv.conf for writing
    QFile resolvConf("/etc/resolv.conf");
    if (!resolvConf.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open /etc/resolv.conf for writing";
        emit errorOccurred("Failed to open /etc/resolv.conf for writing");
        return false;
    }

    QTextStream out(&resolvConf);
    // Write only the required DNS server
    out << "nameserver " << dnsServers.first() << "\n";
    resolvConf.close();

    // Set correct permissions
    if (chmod("/etc/resolv.conf", 0644) != 0) {
        qWarning() << "Failed to set permissions on /etc/resolv.conf";
        emit errorOccurred("Failed to set permissions on /etc/resolv.conf");
        return false;
    }

    // Flush DNS cache
    flushDNSCache();

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
