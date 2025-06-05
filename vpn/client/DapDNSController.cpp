#include "DapDNSController.h"
#include <QDebug>
#include <QProcess>
#include <QHostAddress>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QRegularExpression>
#include <QMutexLocker>
#include <QCoreApplication>

#ifdef Q_OS_WINDOWS
#include <windows.h>
#include <iphlpapi.h>
#include <windns.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <comdef.h>
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
    
#ifdef Q_OS_ANDROID
    // Initialize Android activity
    m_activity = QAndroidJniObject::callStaticObjectMethod(
        "org/qtproject/qt5/android/QtNative",
        "activity",
        "()Landroid/app/Activity;"
    );
    if (!m_activity.isValid()) {
        qWarning() << "Failed to get Android activity";
    }
#endif

#ifdef Q_OS_MACOS
    // Initialize store reference
    m_store = SCDynamicStoreCreate(nullptr, 
                                  CFSTR("DapDNSController"),
                                  nullptr, 
                                  nullptr);
    if (!m_store) {
        qWarning() << "Failed to create SCDynamicStore";
    }
#endif
}

DapDNSController::~DapDNSController()
{
    if (m_isDNSSet) {
        restoreDefaultDNS();
    }

#ifdef Q_OS_WINDOWS
    if (m_originalDNSConfig) {
        free(m_originalDNSConfig);
        m_originalDNSConfig = nullptr;
    }
#endif

#ifdef Q_OS_MACOS
    if (m_store) {
        CFRelease(m_store);
        m_store = nullptr;
    }
#endif
}

// Update getter/setter implementations
QString DapDNSController::getIfaceName() const
{
    QMutexLocker locker(&m_mutex);
    return m_ifaceName;
}

void DapDNSController::setIfaceName(const QString &name)
{
    QMutexLocker locker(&m_mutex);
    m_ifaceName = name;
}

// Add thread-safe accessor for original DNS servers
QStringList DapDNSController::getOriginalDNSServers() const
{
    QMutexLocker locker(&m_mutex);
    return m_originalDNSServers;
}

// Add thread-safe setter for original DNS servers
void DapDNSController::setOriginalDNSServers(const QStringList &servers)
{
    QMutexLocker locker(&m_mutex);
    m_originalDNSServers = servers;
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
        QMutexLocker locker(&m_mutex);
        m_isDNSSet = true;
        emit dnsServersChanged(dnsServers);
    } else {
        emit errorOccurred("Failed to set DNS servers");
    }
    return result;
}

bool DapDNSController::restoreDefaultDNS()
{
    QStringList originalServers = getOriginalDNSServers();
    if (originalServers.isEmpty()) {
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
        QMutexLocker locker(&m_mutex);
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
#endif

    if (servers.isEmpty()) {
        qWarning() << "Failed to get current DNS servers";
        emit errorOccurred("Failed to get current DNS servers");
    }
    return servers;
}

bool DapDNSController::isDNSSet() const
{
    QMutexLocker locker(&m_mutex);
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
    process.setProcessChannelMode(QProcess::MergedChannels);
    QStringList args = QProcess::splitCommand(cmd);
    QString program = args.takeFirst();
    process.start(program, args);
    process.waitForFinished();
    return process.exitCode();
}

bool DapDNSController::runNetshCommand(const QString &cmd, QString *output, int timeout)
{
    qInfo() << "[DNS] Executing netsh command:" << cmd;
#ifdef Q_OS_WINDOWS
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    
    process.start("cmd.exe", QStringList() << "/c" << cmd);
    
    if (!process.waitForStarted(timeout)) {
        qWarning() << "[DNS] Failed to start command:" << cmd;
        qWarning() << "[DNS] Process error:" << process.errorString();
        qWarning() << "[DNS] Process state:" << process.state();
        emit errorOccurred(QString("Failed to start command: %1").arg(process.errorString()));
        return false;
    }

    if (!process.waitForFinished(timeout)) {
        qWarning() << "[DNS] Command timed out after" << timeout << "ms:" << cmd;
        qWarning() << "[DNS] Process state before kill:" << process.state();
        emit errorOccurred(QString("Command timed out after %1 ms").arg(timeout));
        process.kill();
        return false;
    }

    QByteArray processOutput = process.readAll();
    QString outputStr = QString::fromLocal8Bit(processOutput);

    if (output) {
        *output = outputStr;
    }

    qInfo() << "[DNS] Command output:" << outputStr.trimmed();
    
    int exitCode = process.exitCode();
    qInfo() << "[DNS] Command exit code:" << exitCode;
    
    if (exitCode != 0) {
        // Check for specific error conditions
        if (exitCode == ERROR_ELEVATION_REQUIRED || 
            outputStr.contains("The requested operation requires elevation") ||
            outputStr.contains("requires elevated privileges")) {
            qWarning() << "[DNS] Command requires elevation:" << cmd;
            emit errorOccurred("The operation requires administrator privileges");
            return false;
        }

        qWarning() << "[DNS] Command failed with exit code" << exitCode << ":" << cmd
                  << "Output:" << outputStr;
        qWarning() << "[DNS] Error output:" << process.readAllStandardError();
        emit errorOccurred(QString("Command failed with exit code %1: %2")
                         .arg(exitCode)
                         .arg(outputStr.trimmed()));
        return false;
    }

    // Check for common error patterns even if exit code is 0
    if (outputStr.contains("The system cannot find the file specified") ||
        outputStr.contains("File not found") ||
        outputStr.contains("The command failed to complete successfully") ||
        outputStr.contains("Command execution failed")) {
        qWarning() << "[DNS] Command failed:" << cmd << "Output:" << outputStr;
        emit errorOccurred(QString("Command failed: %1").arg(outputStr.trimmed()));
        return false;
    }

    return true;
#else
    // On non-Windows platforms, use exec_silent
    int exitCode = exec_silent(cmd);
    if (output) {
        *output = QString();  // Clear output on non-Windows platforms
    }
    return exitCode == 0;
#endif
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

bool DapDNSController::getIfaceName()
{
    qInfo() << "[DNS] Starting interface name detection...";
    bool found = false;
    PIP_ADAPTER_ADDRESSES pAddresses = nullptr;

    // First try to get the best interface using GetBestInterface
    DWORD bestIfIndex = 0;
    struct sockaddr_in destAddr;
    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_addr.s_addr = inet_addr("8.8.8.8"); // Google DNS as a reference point

    qInfo() << "[DNS] Attempting to find best interface using 8.8.8.8 as reference";

    if (destAddr.sin_addr.s_addr == INADDR_NONE) {
        qWarning() << "[DNS] inet_addr failed for 8.8.8.8";
        emit errorOccurred("Failed to resolve reference IP address");
        return false;
    }

    DWORD bestIfResult = GetBestInterfaceEx((struct sockaddr*)&destAddr, &bestIfIndex);
    if (bestIfResult != NO_ERROR) {
        qWarning() << "[DNS] GetBestInterfaceEx failed with code:" << GetLastError();
        qWarning() << "[DNS] System error message:" << QString::fromWCharArray(_com_error(GetLastError()).ErrorMessage());
        emit errorOccurred("Failed to determine best network interface");
    } else {
        qInfo() << "[DNS] Best interface index found:" << bestIfIndex;
    }

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
                            
                            m_ifaceName = QString::fromWCharArray(pCurrAddresses->FriendlyName);
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
                            
                            m_ifaceName = QString::fromWCharArray(pCurrAddresses->FriendlyName);
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
        qWarning() << "[DNS] No suitable network interface found after all attempts";
        emit errorOccurred("No suitable network interface found");
        m_ifaceName.clear();
        return false;
    }

    qInfo() << "[DNS] Successfully found network interface:" << m_ifaceName;
    return true;
}

bool DapDNSController::verifyIfaceStatus()
{
    if (m_ifaceName.isEmpty()) {
        qWarning() << "Interface name is empty";
        emit errorOccurred("Interface name is empty");
        return false;
    }

    QString output;
    QString cmd = QString("netsh interface show interface \"%1\"").arg(m_ifaceName);
    
    if (!runNetshCommand(cmd, &output)) {
        return false;
    }

    if (!output.contains("Connect state: Connected")) {
        qWarning() << "Interface is not connected:" << m_ifaceName;
        emit errorOccurred(QString("Interface %1 is not in connected state").arg(m_ifaceName));
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
    qInfo() << "[DNS] Starting DNS server configuration...";
    qInfo() << "[DNS] Attempting to set DNS servers:" << dnsServers;

    // Update original DNS servers if not set
    updateOriginalDNSServers();
    qInfo() << "[DNS] Original DNS servers:" << m_originalDNSServers;

    // Validate input parameters
    if (dnsServers.isEmpty()) {
        qWarning() << "[DNS] Empty DNS servers list provided";
        emit errorOccurred("Empty DNS servers list provided");
        return false;
    }

    // Validate each DNS server address
    for (const QString &dns : dnsServers) {
        QHostAddress addr(dns);
        if (addr.isNull() || (addr.protocol() != QAbstractSocket::IPv4Protocol && 
                             addr.protocol() != QAbstractSocket::IPv6Protocol)) {
            qWarning() << "[DNS] Invalid DNS server address:" << dns;
            emit errorOccurred(QString("Invalid DNS server address: %1").arg(dns));
            return false;
        }
        qInfo() << "[DNS] Validated DNS server:" << dns;
    }

    // Check administrator privileges
    if (!isRunAsAdmin()) {
        qWarning() << "[DNS] Application is not running with administrator privileges";
        qWarning() << "[DNS] Current process ID:" << QCoreApplication::applicationPid();
        emit errorOccurred("DNS settings cannot be changed without administrator privileges");
        return false;
    }
    qInfo() << "[DNS] Administrator privileges confirmed";

    // Get interface name if not already set
    if (m_ifaceName.isEmpty()) {
        qInfo() << "[DNS] Interface name not set, attempting to get it...";
        if (!getIfaceName()) {
            qWarning() << "[DNS] Failed to get interface name";
            emit errorOccurred("Failed to get interface name");
            return false;
        }
    }
    qInfo() << "[DNS] Using network interface:" << m_ifaceName;

    // Verify interface is connected and operational
    qInfo() << "[DNS] Verifying interface status...";
    if (!verifyIfaceStatus()) {
        qWarning() << "[DNS] Interface verification failed for:" << m_ifaceName;
        return false;
    }
    qInfo() << "[DNS] Interface status verified successfully";

    // Reset current DNS settings
    QString resetCmd = QString("netsh interface ip set dns name=\"%1\" source=none")
        .arg(m_ifaceName);
    
    qInfo() << "[DNS] Executing reset command:" << resetCmd;
    if (!runNetshCommand(resetCmd)) {
        qWarning() << "[DNS] Failed to reset DNS settings";
        emit errorOccurred("Failed to reset DNS settings");
        return false;
    }
    qInfo() << "[DNS] Successfully reset DNS settings";

    // Set primary DNS server
    QString cmd = QString("netsh interface ip set dns name=\"%1\" static %2 primary")
        .arg(m_ifaceName)
        .arg(dnsServers.first());

    qInfo() << "[DNS] Setting primary DNS server with command:" << cmd;
    if (!runNetshCommand(cmd)) {
        qWarning() << "[DNS] Failed to set primary DNS server:" << dnsServers.first();
        emit errorOccurred(QString("Failed to set primary DNS server %1").arg(dnsServers.first()));
        return false;
    }
    qInfo() << "[DNS] Successfully set primary DNS server:" << dnsServers.first();

    // Add additional DNS servers if any
    for (int i = 1; i < dnsServers.size(); ++i) {
        cmd = QString("netsh interface ip add dns name=\"%1\" addr=%2 index=%3")
            .arg(m_ifaceName)
            .arg(dnsServers[i])
            .arg(i + 1);

        qInfo() << "[DNS] Adding secondary DNS server with command:" << cmd;
        if (!runNetshCommand(cmd)) {
            qWarning() << "[DNS] Failed to add DNS server:" << dnsServers[i] << "with index" << (i + 1);
            emit errorOccurred(QString("Failed to add DNS server %1").arg(dnsServers[i]));
            return false;
        }
        qInfo() << "[DNS] Successfully added secondary DNS server:" << dnsServers[i];
    }

    qInfo() << "[DNS] DNS configuration completed successfully";
    return true;
}

bool DapDNSController::restoreDefaultDNSWindows()
{
    qInfo() << "[DNS] Starting DNS restoration process...";

    // Check administrator privileges
    if (!isRunAsAdmin()) {
        qWarning() << "[DNS] Administrator privileges required for DNS restoration";
        qWarning() << "[DNS] Current process ID:" << QCoreApplication::applicationPid();
        emit errorOccurred("DNS settings cannot be restored without administrator privileges");
        return false;
    }
    qInfo() << "[DNS] Administrator privileges confirmed for restoration";

    // Check for saved DNS servers
    if (m_originalDNSServers.isEmpty()) {
        qWarning() << "[DNS] No original DNS servers found to restore";
        emit errorOccurred("No original DNS servers to restore");
        return false;
    }
    qInfo() << "[DNS] Original DNS servers to restore:" << m_originalDNSServers;

    // Validate saved DNS addresses
    for (const QString &dns : m_originalDNSServers) {
        QHostAddress addr(dns);
        if (addr.isNull() || (addr.protocol() != QAbstractSocket::IPv4Protocol && 
                             addr.protocol() != QAbstractSocket::IPv6Protocol)) {
            qWarning() << "[DNS] Invalid saved DNS server address:" << dns;
            emit errorOccurred(QString("Invalid saved DNS server address: %1").arg(dns));
            return false;
        }
        qInfo() << "[DNS] Validated original DNS server:" << dns;
    }

    // Check and get interface name if not set
    if (m_ifaceName.isEmpty()) {
        qInfo() << "[DNS] Interface name not set for restoration, attempting to get it...";
        if (!getIfaceName()) {
            qWarning() << "[DNS] Failed to get interface name for restoration";
            emit errorOccurred("Failed to get interface name");
            return false;
        }
    }
    qInfo() << "[DNS] Using network interface for restoration:" << m_ifaceName;

    // Verify interface is connected and operational
    qInfo() << "[DNS] Verifying interface status for restoration...";
    if (!verifyIfaceStatus()) {
        qWarning() << "[DNS] Interface verification failed during restoration for:" << m_ifaceName;
        return false;
    }
    qInfo() << "[DNS] Interface status verified successfully for restoration";

    // Reset current DNS settings
    QString resetCmd = QString("netsh interface ip set dns name=\"%1\" source=none")
        .arg(m_ifaceName);
    
    qInfo() << "[DNS] Executing reset command for restoration:" << resetCmd;
    if (!runNetshCommand(resetCmd)) {
        qWarning() << "[DNS] Failed to reset DNS settings during restoration";
        emit errorOccurred("Failed to reset DNS settings");
        return false;
    }
    qInfo() << "[DNS] Successfully reset DNS settings for restoration";

    // Restore DNS servers
    QString cmd = QString("netsh interface ip set dns name=\"%1\" static %2 primary")
        .arg(m_ifaceName)
        .arg(m_originalDNSServers.first());

    qInfo() << "[DNS] Restoring primary DNS server with command:" << cmd;
    if (!runNetshCommand(cmd)) {
        qWarning() << "[DNS] Failed to restore primary DNS server:" << m_originalDNSServers.first();
        emit errorOccurred(QString("Failed to restore primary DNS server %1").arg(m_originalDNSServers.first()));
        return false;
    }
    qInfo() << "[DNS] Successfully restored primary DNS server:" << m_originalDNSServers.first();

    // Add remaining DNS servers
    for (int i = 1; i < m_originalDNSServers.size(); ++i) {
        cmd = QString("netsh interface ip add dns name=\"%1\" addr=%2 index=%3")
            .arg(m_ifaceName)
            .arg(m_originalDNSServers[i])
            .arg(i + 1);

        qInfo() << "[DNS] Restoring secondary DNS server with command:" << cmd;
        if (!runNetshCommand(cmd)) {
            qWarning() << "[DNS] Failed to restore DNS server:" << m_originalDNSServers[i] << "with index" << (i + 1);
            emit errorOccurred(QString("Failed to restore DNS server %1").arg(m_originalDNSServers[i]));
            return false;
        }
        qInfo() << "[DNS] Successfully restored secondary DNS server:" << m_originalDNSServers[i];
    }

    qInfo() << "[DNS] DNS restoration completed successfully";
    return true;
}

bool DapDNSController::restoreDNSFromList(const QString &iface, const QStringList &dnsList)
{
    if (iface.isEmpty()) {
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
        .arg(iface);
    
    if (!runNetshCommand(resetCmd)) {
        qWarning() << "Failed to reset DNS settings for interface" << iface;
        emit errorOccurred(QString("Failed to reset DNS settings for interface %1").arg(iface));
        return false;
    }

    // Set primary DNS server
    QString cmd = QString("netsh interface ip set dns name=\"%1\" static %2 primary")
        .arg(iface)
        .arg(dnsList.first());

    if (!runNetshCommand(cmd)) {
        qWarning() << "Failed to set primary DNS server" << dnsList.first() << "for interface" << iface;
        emit errorOccurred(QString("Failed to set primary DNS server %1").arg(dnsList.first()));
        return false;
    }

    // Add remaining DNS servers
    for (int i = 1; i < dnsList.size(); ++i) {
        cmd = QString("netsh interface ip add dns name=\"%1\" addr=%2 index=%3")
            .arg(iface)
            .arg(dnsList[i])
            .arg(i + 1);

        if (!runNetshCommand(cmd)) {
            qWarning() << "Failed to add DNS server" << dnsList[i] << "with index" << (i + 1) 
                      << "for interface" << iface;
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

/**
 * Get list of DNS server indexes for the specified network interface
 * Expected netsh output format (examples):
 * English:
 *   Configuration for interface "Ethernet"
 *   DNS servers configured through DHCP:  8.8.8.8 (1)
 *                                        8.8.4.4 (2)
 *   Statically Configured DNS Servers:    1.1.1.1 (1)
 *                                        1.0.0.1 (2)
 * 
 * @param iface Network interface name
 * @return List of DNS server indexes found in the output
 * @note Returns empty list if no DNS servers found or command failed
 */
QStringList DapDNSController::getCurrentDNSIndexes(const QString &iface)
{
    QStringList indexes;
    QString output;
    QString cmd = QString("netsh interface ip show dns \"%1\"").arg(iface);
    qDebug() << "Executing command:" << cmd;
    
    if (!runNetshCommand(cmd, &output)) {
        qWarning() << "getCurrentDNSIndexes: Failed to get DNS configuration for interface" << iface;
        return indexes;
    }

    if (output.isEmpty()) {
        qDebug() << "getCurrentDNSIndexes: Empty output from netsh command";
        return indexes;
    }

    // Modern regex pattern that matches both IP and index in one go
    // Matches: IP address followed by index in parentheses, with optional whitespace
    // Example: "8.8.8.8 (1)" or "8.8.8.8(1)" or "   8.8.8.8 (1)"
    static const QRegularExpression dnsPattern(
        R"((\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\s*\((\d+)\))",
        QRegularExpression::MultilineOption
    );

    QRegularExpressionMatchIterator matchIterator = dnsPattern.globalMatch(output);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        QString ip = match.captured(1);
        QString index = match.captured(2);
        
        // Validate IP address format
        if (QHostAddress(ip).isNull()) {
            qWarning() << "getCurrentDNSIndexes: Invalid IP address found:" << ip;
            continue;
        }

        if (!indexes.contains(index)) {
            indexes.append(index);
            qDebug() << "getCurrentDNSIndexes: Found DNS server" << ip << "with index" << index;
        }
    }

    if (indexes.isEmpty()) {
        qDebug() << "getCurrentDNSIndexes: No DNS servers found in output:" << output;
    }

    return indexes;
}

/**
 * Reset DNS settings for the specified network interface
 * 
 * @param iface Network interface name
 * @param useDHCP If true, sets DNS to DHCP mode; if false, removes all DNS servers
 * @return true if operation succeeded, false otherwise
 */
bool DapDNSController::resetInterfaceDNS(const QString &iface, bool useDHCP)
{
    // Validate input
    if (iface.isEmpty()) {
        qWarning() << "resetInterfaceDNS: Empty interface name provided";
        emit errorOccurred("Empty interface name provided");
        return false;
    }

    // Prepare command
    QString cmd = QString("netsh interface ip set dns name=\"%1\" source=%2")
        .arg(iface)
        .arg(useDHCP ? "dhcp" : "none");

    qDebug() << "resetInterfaceDNS: Executing command:" << cmd;

    // Execute command and capture output
    QString output;
    if (!runNetshCommand(cmd, &output)) {
        qWarning() << "resetInterfaceDNS: Command failed for interface" << iface;
        qWarning() << "resetInterfaceDNS: Command output:" << output;
        
        QString errorMsg = QString("Failed to reset DNS settings for interface %1: %2")
            .arg(iface)
            .arg(output.trimmed());
        emit errorOccurred(errorMsg);
        return false;
    }

    qDebug() << "resetInterfaceDNS: Successfully reset DNS settings for interface" 
             << iface << "to" << (useDHCP ? "DHCP" : "none");
    return true;
} 
