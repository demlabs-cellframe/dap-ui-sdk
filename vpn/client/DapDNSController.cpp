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
#include <QTimer>
#include <QDateTime>

#ifdef Q_OS_WINDOWS
#include <windows.h>
#include <iphlpapi.h>
#include <windns.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <comdef.h>
#include <string>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "dnsapi.lib")
#pragma comment(lib, "ws2_32.lib")
#endif

DapDNSController::DapDNSController(QObject *parent)
    : QObject(parent)
    , m_isDNSSet(false)
    , m_targetIfIndex(0)
    , m_dnsMonitorTimer(nullptr)
    , m_vpnConnected(false)
    , m_dnsMonitoringActive(false)
    , m_monitoringInterval(5000)
    , m_consecutiveFailures(0)
#ifdef Q_OS_WINDOWS
    , m_originalDNSConfig(nullptr)
#endif
#ifdef Q_OS_MACOS
    , m_store(nullptr)
#endif
{
    // Save current DNS settings
    m_originalDNSServers = getCurrentDNSServers();
    
    // Initialize DNS monitoring timer
    m_dnsMonitorTimer = new QTimer(this);
    m_dnsMonitorTimer->setSingleShot(false);
    connect(m_dnsMonitorTimer, &QTimer::timeout, this, &DapDNSController::onMonitoringTimerTimeout);
    
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
    // Stop DNS monitoring before cleanup
    stopDNSMonitoring();
    
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

QStringList DapDNSController::getCurrentDNSServers() const
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
        // Note: Cannot emit errorOccurred from const method, so we just log the warning
    }
    return servers;
}

bool DapDNSController::isDNSSet() const
{
    QMutexLocker locker(&m_mutex);
    return m_isDNSSet;
}

// Helper function to validate IP address
bool DapDNSController::isValidIPAddress(const QString &ipAddress) const
{
    if (ipAddress.isEmpty()) {
        return false;
    }

    QHostAddress address(ipAddress);
    return !address.isNull() && address.protocol() != QAbstractSocket::UnknownNetworkLayerProtocol;
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

bool DapDNSController::runNetshCommand(const QString &program, const QStringList &args, QString *output, int timeout)
{
    Q_UNUSED(timeout)  // Suppress unused parameter warning
    qInfo() << "[DNS] Executing netsh:" << program << args;
#ifdef Q_OS_WINDOWS
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    
    process.start(program, args);
    
    if (!process.waitForStarted(timeout)) {
        qWarning() << "[DNS] Failed to start command:" << program << args;
        qWarning() << "[DNS] Process error:" << process.errorString();
        qWarning() << "[DNS] Process state:" << process.state();
        emit errorOccurred(QString("Failed to start command: %1").arg(process.errorString()));
        return false;
    }

    if (!process.waitForFinished(timeout)) {
        qWarning() << "[DNS] Command timed out after" << timeout << "ms:" << program << args;
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
            qWarning() << "[DNS] Command requires elevation:" << program << args;
            emit errorOccurred("The operation requires administrator privileges");
            return false;
        }

        qWarning() << "[DNS] Command failed with exit code" << exitCode << ":" << program << args
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
        qWarning() << "[DNS] Command failed:" << program << args << "Output:" << outputStr;
        emit errorOccurred(QString("Command failed: %1").arg(outputStr.trimmed()));
        return false;
    }

    return true;
#else
    // On non-Windows platforms, use exec_silent
    QString cmd = program + " " + args.join(" ");
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
    QStringList args;
    args << "interface" << "show" << "interface" << QString("name=%1").arg(m_ifaceName);
    
    if (!runNetshCommand("netsh", args, &output)) {
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

    // Ensure interface name is properly quoted if it contains spaces
    QString safeIface = m_ifaceName.contains(' ') ? QString("\"%1\"").arg(m_ifaceName) : m_ifaceName;

    // Reset current DNS settings
    QStringList resetArgs;
    resetArgs << "interface" << "ip" << "set" << "dns" << QString("name=%1").arg(safeIface) << "source=none";
    
    qInfo() << "[DNS] Executing reset command with args:" << resetArgs;
    if (!runNetshCommand("netsh", resetArgs)) {
        qWarning() << "[DNS] Failed to reset DNS settings";
        emit errorOccurred("Failed to reset DNS settings");
        return false;
    }
    qInfo() << "[DNS] Successfully reset DNS settings";

    // Set primary DNS server
    QStringList setPrimaryArgs;
    setPrimaryArgs << "interface" << "ip" << "set" << "dns" << QString("name=%1").arg(safeIface) << QString("static=%1").arg(dnsServers.first()) << "primary";

    qInfo() << "[DNS] Setting primary DNS server with args:" << setPrimaryArgs;
    if (!runNetshCommand("netsh", setPrimaryArgs)) {
        qWarning() << "[DNS] Failed to set primary DNS server:" << dnsServers.first();
        emit errorOccurred(QString("Failed to set primary DNS server %1").arg(dnsServers.first()));
        return false;
    }
    qInfo() << "[DNS] Successfully set primary DNS server:" << dnsServers.first();

    // Add additional DNS servers if any
    for (int i = 1; i < dnsServers.size(); ++i) {
        QStringList addArgs;
        addArgs << "interface" << "ip" << "add" << "dns" << QString("name=%1").arg(safeIface) << QString("addr=%1").arg(dnsServers[i]) << QString("index=%1").arg(i + 1);

        qInfo() << "[DNS] Adding secondary DNS server with args:" << addArgs;
        if (!runNetshCommand("netsh", addArgs)) {
            qWarning() << "[DNS] Failed to add DNS server:" << dnsServers[i] << "with index" << (i + 1);
            emit errorOccurred(QString("Failed to add DNS server %1").arg(dnsServers[i]));
            return false;
        }
        qInfo() << "[DNS] Successfully added secondary DNS server:" << dnsServers[i];
    }

    // Verify DNS settings were applied correctly
    QStringList appliedDns = getCurrentDNSServersWindows();
    if (!verifyDNSSettings(dnsServers, appliedDns)) {
        qWarning() << "[DNS] DNS verification failed. Expected:" << dnsServers
                  << "but got:" << appliedDns;
        emit errorOccurred("DNS not applied successfully");
        return false;
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

    // Ensure interface name is properly quoted if it contains spaces
    QString safeIface = m_ifaceName.contains(' ') ? QString("\"%1\"").arg(m_ifaceName) : m_ifaceName;

    // Reset current DNS settings
    QStringList resetArgs;
    resetArgs << "interface" << "ip" << "set" << "dns" << QString("name=%1").arg(safeIface) << "source=none";
    
    qInfo() << "[DNS] Executing reset command for restoration with args:" << resetArgs;
    if (!runNetshCommand("netsh", resetArgs)) {
        qWarning() << "[DNS] Failed to reset DNS settings for restoration";
        emit errorOccurred("Failed to reset DNS settings for restoration");
        return false;
    }
    qInfo() << "[DNS] Successfully reset DNS settings for restoration";

    // Set primary DNS server
    QStringList setPrimaryArgs;
    setPrimaryArgs << "interface" << "ip" << "set" << "dns" << QString("name=%1").arg(safeIface) << QString("static=%1").arg(m_originalDNSServers.first()) << "primary";

    qInfo() << "[DNS] Setting primary DNS server for restoration with args:" << setPrimaryArgs;
    if (!runNetshCommand("netsh", setPrimaryArgs)) {
        qWarning() << "[DNS] Failed to set primary DNS server for restoration:" << m_originalDNSServers.first();
        emit errorOccurred(QString("Failed to set primary DNS server %1 for restoration").arg(m_originalDNSServers.first()));
        return false;
    }
    qInfo() << "[DNS] Successfully set primary DNS server for restoration:" << m_originalDNSServers.first();

    // Add additional DNS servers if any
    for (int i = 1; i < m_originalDNSServers.size(); ++i) {
        QStringList addArgs;
        addArgs << "interface" << "ip" << "add" << "dns" << QString("name=%1").arg(safeIface) << QString("addr=%1").arg(m_originalDNSServers[i]) << QString("index=%1").arg(i + 1);

        qInfo() << "[DNS] Adding secondary DNS server for restoration with args:" << addArgs;
        if (!runNetshCommand("netsh", addArgs)) {
            qWarning() << "[DNS] Failed to add DNS server for restoration:" << m_originalDNSServers[i] << "with index" << (i + 1);
            emit errorOccurred(QString("Failed to add DNS server %1 for restoration").arg(m_originalDNSServers[i]));
            return false;
        }
        qInfo() << "[DNS] Successfully added secondary DNS server for restoration:" << m_originalDNSServers[i];
    }

    // Verify DNS settings were restored correctly
    QStringList restoredDns = getCurrentDNSServersWindows();
    if (!verifyDNSSettings(m_originalDNSServers, restoredDns)) {
        qWarning() << "[DNS] DNS restoration verification failed. Expected:" << m_originalDNSServers
                  << "but got:" << restoredDns;
        emit errorOccurred("DNS not restored successfully");
        return false;
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

    // Ensure interface name is properly quoted if it contains spaces
    QString safeIface = iface.contains(' ') ? QString("\"%1\"").arg(iface) : iface;

    // First reset current DNS settings
    QStringList resetArgs;
    resetArgs << "interface" << "ip" << "set" << "dns" << QString("name=%1").arg(safeIface) << "source=none";
    
    if (!runNetshCommand("netsh", resetArgs)) {
        qWarning() << "Failed to reset DNS settings for interface" << iface;
        emit errorOccurred(QString("Failed to reset DNS settings for interface %1").arg(iface));
        return false;
    }

    // Set primary DNS server
    QStringList setPrimaryArgs;
    setPrimaryArgs << "interface" << "ip" << "set" << "dns" << QString("name=%1").arg(safeIface) << QString("static=%1").arg(dnsList.first()) << "primary";

    if (!runNetshCommand("netsh", setPrimaryArgs)) {
        qWarning() << "Failed to set primary DNS server" << dnsList.first() << "for interface" << iface;
        emit errorOccurred(QString("Failed to set primary DNS server %1").arg(dnsList.first()));
        return false;
    }

    // Add remaining DNS servers
    for (int i = 1; i < dnsList.size(); ++i) {
        QStringList addArgs;
        addArgs << "interface" << "ip" << "add" << "dns" << QString("name=%1").arg(safeIface) << QString("addr=%1").arg(dnsList[i]) << QString("index=%1").arg(i + 1);

        if (!runNetshCommand("netsh", addArgs)) {
            qWarning() << "Failed to add DNS server" << dnsList[i] << "with index" << (i + 1) 
                      << "for interface" << iface;
            emit errorOccurred(QString("Failed to add DNS server %1").arg(dnsList[i]));
            return false;
        }
    }

    // Verify DNS settings were applied correctly
    QStringList appliedDns = getCurrentDNSServersWindows();
    if (!verifyDNSSettings(dnsList, appliedDns)) {
        qWarning() << "DNS verification failed. Expected:" << dnsList
                  << "but got:" << appliedDns;
        emit errorOccurred("DNS not applied successfully");
        return false;
    }

    return true;
}

QStringList DapDNSController::getCurrentDNSServersWindows() const
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

// New interface-specific methods using registry approach
bool DapDNSController::setDNSServersForInterface(ulong ifIndex, const QStringList &dnsServers)
{
    QMutexLocker locker(&m_mutex);
    
    qInfo() << "[DNS] Setting DNS servers for interface index:" << ifIndex;
    qInfo() << "[DNS] DNS servers to set:" << dnsServers;
    
    // Validate input
    if (dnsServers.isEmpty()) {
        qWarning() << "[DNS] Empty DNS servers list provided for interface" << ifIndex;
        emit errorOccurred("Empty DNS servers list provided");
        return false;
    }
    
    // Validate DNS addresses
    for (const QString &dns : dnsServers) {
        if (!isValidIPAddress(dns)) {
            qWarning() << "[DNS] Invalid DNS server address:" << dns;
            emit errorOccurred(QString("Invalid DNS server address: %1").arg(dns));
            return false;
        }
    }
    
    // Save original DNS servers if not already saved
    if (!m_originalInterfaceDNS.contains(ifIndex)) {
        if (!saveOriginalDNSForInterface(ifIndex)) {
            qWarning() << "[DNS] Failed to save original DNS for interface" << ifIndex;
        }
    }
    
    // Use registry-based approach
    bool result = setDNSServersWindowsRegistry(ifIndex, dnsServers);
    
    if (result) {
        qInfo() << "[DNS] Successfully set DNS servers for interface" << ifIndex;
        emit dnsServersChanged(dnsServers);
        m_isDNSSet = true;
    } else {
        qWarning() << "[DNS] Failed to set DNS servers for interface" << ifIndex;
        emit errorOccurred("Failed to set DNS servers");
    }
    
    return result;
}

bool DapDNSController::setDNSServerForInterface(ulong ifIndex, const QString &dnsServer)
{
    return setDNSServersForInterface(ifIndex, QStringList{dnsServer});
}

bool DapDNSController::restoreDNSForInterface(ulong ifIndex)
{
    QMutexLocker locker(&m_mutex);
    
    qInfo() << "[DNS] Restoring DNS for interface index:" << ifIndex;
    
    if (!m_originalInterfaceDNS.contains(ifIndex)) {
        qWarning() << "[DNS] No original DNS servers saved for interface" << ifIndex;
        emit errorOccurred("No original DNS servers to restore");
        return false;
    }
    
    QStringList originalDNS = m_originalInterfaceDNS[ifIndex];
    qInfo() << "[DNS] Original DNS servers to restore:" << originalDNS;
    
    bool result = restoreDNSWindowsRegistry(ifIndex);
    
    if (result) {
        qInfo() << "[DNS] Successfully restored DNS for interface" << ifIndex;
        emit dnsRestored();
        m_isDNSSet = false;
    } else {
        qWarning() << "[DNS] Failed to restore DNS for interface" << ifIndex;
        emit errorOccurred("Failed to restore DNS");
    }
    
    return result;
}

QStringList DapDNSController::getCurrentDNSServersForInterface(ulong ifIndex)
{
    Q_UNUSED(ifIndex);
    qWarning() << "getCurrentDNSServersForInterface: Not implemented on this platform";
    return QStringList();
}

// Universal interface index management functions (cross-platform)
void DapDNSController::setTargetInterfaceIndex(ulong ifIndex)
{
    QMutexLocker locker(&m_mutex);
    m_targetIfIndex = ifIndex;
    qInfo() << "[DNS] Target interface index set to:" << ifIndex;
}

ulong DapDNSController::getTargetInterfaceIndex() const
{
    QMutexLocker locker(&m_mutex);
    return m_targetIfIndex;
}

// Registry-based implementation methods
bool DapDNSController::setDNSServersWindowsRegistry(ulong ifIndex, const QStringList &dnsServers)
{
    qInfo() << "[DNS] Using registry method for interface" << ifIndex;
    
    QString registryPath = getInterfaceRegistryPath(ifIndex);
    if (registryPath.isEmpty()) {
        qWarning() << "[DNS] Failed to get registry path for interface" << ifIndex;
        return false;
    }
    
    qInfo() << "[DNS] Registry path:" << registryPath;
    
    HKEY hKey;
    LONG result = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                               reinterpret_cast<const wchar_t*>(registryPath.utf16()),
                               0,
                               KEY_WRITE,
                               &hKey);
    
    if (result != ERROR_SUCCESS) {
        qWarning() << "[DNS] Failed to open registry key:" << registryPath;
        qWarning() << "[DNS] Registry error code:" << result;
        return false;
    }
    
    // Convert QStringList to space-separated string
    QString dnsString = dnsServers.join(' ');
    qInfo() << "[DNS] Setting DNS string:" << dnsString;
    
    // Convert to wide string
    std::wstring wDnsString = dnsString.toStdWString();
    
    // Set the DNS servers
    result = RegSetValueExW(hKey,
                           L"NameServer",
                           0,
                           REG_SZ,
                           reinterpret_cast<const BYTE*>(wDnsString.c_str()),
                           static_cast<DWORD>((wDnsString.length() + 1) * sizeof(wchar_t)));
    
    RegCloseKey(hKey);
    
    if (result != ERROR_SUCCESS) {
        qWarning() << "[DNS] Failed to set DNS in registry, error code:" << result;
        return false;
    }
    
    qInfo() << "[DNS] Successfully set DNS servers in registry for interface" << ifIndex;
    return true;
}

bool DapDNSController::restoreDNSWindowsRegistry(ulong ifIndex)
{
    qInfo() << "[DNS] Restoring DNS using registry method for interface" << ifIndex;
    
    if (!m_originalInterfaceDNS.contains(ifIndex)) {
        qWarning() << "[DNS] No original DNS to restore for interface" << ifIndex;
        return false;
    }
    
    QStringList originalDNS = m_originalInterfaceDNS[ifIndex];
    
    if (originalDNS.isEmpty()) {
        // Clear DNS settings
        QString registryPath = getInterfaceRegistryPath(ifIndex);
        if (registryPath.isEmpty()) {
            return false;
        }
        
        HKEY hKey;
        if (RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                         reinterpret_cast<const wchar_t*>(registryPath.utf16()),
                         0,
                         KEY_WRITE,
                         &hKey) == ERROR_SUCCESS) {
            
            RegDeleteValueW(hKey, L"NameServer");
            RegCloseKey(hKey);
            qInfo() << "[DNS] Cleared DNS settings for interface" << ifIndex;
            return true;
        }
        return false;
    } else {
        // Restore original DNS servers
        return setDNSServersWindowsRegistry(ifIndex, originalDNS);
    }
}

QStringList DapDNSController::getCurrentDNSServersWindowsRegistry(ulong ifIndex) const
{
    QStringList dnsServers;
    
    QString registryPath = getInterfaceRegistryPath(ifIndex);
    if (registryPath.isEmpty()) {
        qWarning() << "[DNS] Failed to get registry path for interface" << ifIndex;
        return dnsServers;
    }
    
    HKEY hKey;
    LONG result = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                               reinterpret_cast<const wchar_t*>(registryPath.utf16()),
                               0,
                               KEY_READ,
                               &hKey);
    
    if (result != ERROR_SUCCESS) {
        qDebug() << "[DNS] Failed to open registry key for reading:" << registryPath;
        return dnsServers;
    }
    
    wchar_t buffer[1024] = {0};
    DWORD bufferSize = sizeof(buffer);
    DWORD valueType;
    
    result = RegQueryValueExW(hKey,
                             L"NameServer",
                             nullptr,
                             &valueType,
                             reinterpret_cast<BYTE*>(buffer),
                             &bufferSize);
    
    RegCloseKey(hKey);
    
    if (result == ERROR_SUCCESS && valueType == REG_SZ) {
        QString dnsString = QString::fromWCharArray(buffer);
        if (!dnsString.isEmpty()) {
            dnsServers = dnsString.split(' ', Qt::SkipEmptyParts);
            qDebug() << "[DNS] Found DNS servers for interface" << ifIndex << ":" << dnsServers;
        }
    } else {
        qDebug() << "[DNS] No DNS servers found in registry for interface" << ifIndex;
    }
    
    return dnsServers;
}

QString DapDNSController::getInterfaceRegistryPath(ulong ifIndex) const
{
    QString interfaceName = getInterfaceNameByIndex(ifIndex);
    if (interfaceName.isEmpty()) {
        qWarning() << "[DNS] Failed to get interface name for index" << ifIndex;
        return QString();
    }
    
    QString registryPath = QString("SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\%1")
                          .arg(interfaceName);
    
    qDebug() << "[DNS] Registry path for interface" << ifIndex << ":" << registryPath;
    return registryPath;
}

QString DapDNSController::getInterfaceNameByIndex(ulong ifIndex) const
{
    // Check cache first - use thread-safe access
    {
        QMutexLocker locker(&m_mutex);
        if (m_interfaceNames.contains(ifIndex)) {
            return m_interfaceNames[ifIndex];
        }
    }
    
    // Get adapter information
    ULONG outBufLen = 0;
    DWORD dwRetVal = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, nullptr, nullptr, &outBufLen);
    
    if (dwRetVal != ERROR_BUFFER_OVERFLOW) {
        qWarning() << "[DNS] Failed to get adapter addresses buffer size";
        return QString();
    }
    
    PIP_ADAPTER_ADDRESSES pAddresses = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(malloc(outBufLen));
    if (!pAddresses) {
        qWarning() << "[DNS] Failed to allocate memory for adapter addresses";
        return QString();
    }
    
    dwRetVal = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, nullptr, pAddresses, &outBufLen);
    
    QString interfaceName;
    if (dwRetVal == NO_ERROR) {
        for (PIP_ADAPTER_ADDRESSES adapter = pAddresses; adapter; adapter = adapter->Next) {
            if (adapter->IfIndex == ifIndex) {
                interfaceName = QString::fromLatin1(adapter->AdapterName);
                qInfo() << "[DNS] Found interface name for index" << ifIndex << ":" << interfaceName;
                
                // Cache the result - thread-safe modification
                {
                    QMutexLocker locker(&m_mutex);
                    m_interfaceNames[ifIndex] = interfaceName;
                }
                break;
            }
        }
    } else {
        qWarning() << "[DNS] GetAdaptersAddresses failed with error:" << dwRetVal;
    }
    
    free(pAddresses);
    
    if (interfaceName.isEmpty()) {
        qWarning() << "[DNS] Interface name not found for index" << ifIndex;
    }
    
    return interfaceName;
}

bool DapDNSController::saveOriginalDNSForInterface(ulong ifIndex)
{
    QStringList currentDNS = getCurrentDNSServersWindowsRegistry(ifIndex);
    m_originalInterfaceDNS[ifIndex] = currentDNS;
    
    qInfo() << "[DNS] Saved original DNS for interface" << ifIndex << ":" << currentDNS;
    return true;
}
#endif

#ifdef Q_OS_LINUX
bool DapDNSController::setDNSServersLinux(const QStringList &dnsServers)
{
    qInfo() << "[DapDNSController] Setting DNS servers on Linux:" << dnsServers;
    
    // First save original resolv.conf if not already saved
    if (m_originalDNSServers.isEmpty()) {
        QStringList currentDNS = getCurrentDNSServersLinux();
        if (!currentDNS.isEmpty()) {
            m_originalDNSServers = currentDNS;
            qInfo() << "[DapDNSController] Saved original DNS servers:" << m_originalDNSServers;
        }
    }
    
    // Try resolvectl first (modern systemd-resolved)
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    
    // Get active interface
    QStringList statusArgs;
    statusArgs << "status";
    process.start("resolvectl", statusArgs);
    
    QString activeInterface;
    if (process.waitForFinished(3000)) {
        QString output = QString::fromLocal8Bit(process.readAll());
        QStringList lines = output.split('\n', Qt::SkipEmptyParts);
        
        qDebug() << "[DapDNSController] resolvectl status output:" << output;
        
        // Parse the output to find interfaces with DNS scope
        QString currentInterface;
        bool hasDNSScope = false;
        
        for (const QString &line : lines) {
            // Check for interface line
            if (line.startsWith("Link")) {
                // Save previous interface if it had DNS scope
                if (!currentInterface.isEmpty() && hasDNSScope) {
                    // Check if this is a VPN interface
                    if (currentInterface.startsWith("tun") || currentInterface.startsWith("tap")) {
                        activeInterface = currentInterface;
                        qInfo() << "[DapDNSController] Found VPN interface with DNS scope:" << activeInterface;
                        break;
                    }
                }
                
                // Extract interface name from "Link X (interface_name)"
                QRegularExpression linkRegex("Link \\d+ \\(([^)]+)\\)");
                QRegularExpressionMatch match = linkRegex.match(line);
                if (match.hasMatch()) {
                    currentInterface = match.captured(1);
                    hasDNSScope = false; // Reset for new interface
                    qDebug() << "[DapDNSController] Found interface:" << currentInterface;
                }
            }
            // Check if current interface has DNS scope
            else if (!currentInterface.isEmpty() && line.contains("Current Scopes: DNS")) {
                hasDNSScope = true;
                qDebug() << "[DapDNSController] Interface" << currentInterface << "has DNS scope";
            }
        }
        
        // Check the last interface if it had DNS scope
        if (!currentInterface.isEmpty() && hasDNSScope) {
            if (currentInterface.startsWith("tun") || currentInterface.startsWith("tap")) {
                activeInterface = currentInterface;
                qInfo() << "[DapDNSController] Found VPN interface with DNS scope (last):" << activeInterface;
            }
        }
        
        // If no VPN interface found, look for any interface with DNS scope
        if (activeInterface.isEmpty()) {
            currentInterface.clear();
            hasDNSScope = false;
            
            for (const QString &line : lines) {
                if (line.startsWith("Link")) {
                    if (!currentInterface.isEmpty() && hasDNSScope) {
                        activeInterface = currentInterface;
                        qInfo() << "[DapDNSController] Found active interface with DNS scope:" << activeInterface;
                        break;
                    }
                    
                    QRegularExpression linkRegex("Link \\d+ \\(([^)]+)\\)");
                    QRegularExpressionMatch match = linkRegex.match(line);
                    if (match.hasMatch()) {
                        currentInterface = match.captured(1);
                        hasDNSScope = false;
                    }
                }
                else if (!currentInterface.isEmpty() && line.contains("Current Scopes: DNS")) {
                    hasDNSScope = true;
                }
            }
            
            // Check the last interface
            if (!currentInterface.isEmpty() && hasDNSScope) {
                activeInterface = currentInterface;
                qInfo() << "[DapDNSController] Found active interface with DNS scope (last):" << activeInterface;
            }
        }
        
        // If still no interface found, look for any interface (even without DNS scope)
        if (activeInterface.isEmpty()) {
            for (const QString &line : lines) {
                if (line.startsWith("Link")) {
                    QRegularExpression linkRegex("Link \\d+ \\(([^)]+)\\)");
                    QRegularExpressionMatch match = linkRegex.match(line);
                    if (match.hasMatch()) {
                        QString interfaceName = match.captured(1);
                        // Skip loopback and virtual interfaces
                        if (interfaceName != "lo" && !interfaceName.startsWith("virbr") && !interfaceName.startsWith("docker")) {
                            activeInterface = interfaceName;
                            qInfo() << "[DapDNSController] Found fallback interface:" << activeInterface;
                            break;
                        }
                    }
                }
            }
        }
    }
    
    if (!activeInterface.isEmpty()) {
        qInfo() << "[DapDNSController] Using interface:" << activeInterface;
        
        // Set DNS using resolvectl
        QStringList setArgs;
        setArgs << "dns" << activeInterface;
        setArgs.append(dnsServers);
        
        qInfo() << "[DapDNSController] Running resolvectl command:" << "resolvectl" << setArgs;
        process.start("resolvectl", setArgs);
        if (process.waitForFinished(5000)) {
            if (process.exitCode() == 0) {
                qInfo() << "[DapDNSController] Successfully set DNS using resolvectl";
                // Verify the change
                QStringList verifyArgs;
                verifyArgs << "status";
                process.start("resolvectl", verifyArgs);
                if (process.waitForFinished(3000)) {
                    QString verifyOutput = QString::fromLocal8Bit(process.readAll());
                    qInfo() << "[DapDNSController] DNS verification output:" << verifyOutput;
                }
                return true;
            } else {
                qWarning() << "[DapDNSController] resolvectl failed with exit code:" << process.exitCode();
                qWarning() << "[DapDNSController] Output:" << QString::fromLocal8Bit(process.readAll());
            }
        }
    } else {
        qWarning() << "[DapDNSController] No active interface found";
    }
    
    // If resolvectl failed, try NetworkManager
    if (trySetDNSWithNetworkManager(dnsServers)) {
        qInfo() << "[DapDNSController] Successfully set DNS using NetworkManager";
        return true;
    }
    
    // If NetworkManager failed, try direct resolv.conf modification
    qWarning() << "[DapDNSController] NetworkManager failed, trying direct resolv.conf modification";
    
    QFile resolvConf("/etc/resolv.conf");
    if (!resolvConf.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "[DapDNSController] Failed to open /etc/resolv.conf for writing";
        emit errorOccurred("Failed to open /etc/resolv.conf for writing");
        return false;
    }

    QTextStream out(&resolvConf);
    // Write DNS servers
    for (const QString &dnsServer : dnsServers) {
        out << "nameserver " << dnsServer << "\n";
    }
    resolvConf.close();

    // Set correct permissions
    if (chmod("/etc/resolv.conf", 0644) != 0) {
        qWarning() << "[DapDNSController] Failed to set permissions on /etc/resolv.conf";
        emit errorOccurred("Failed to set permissions on /etc/resolv.conf");
        return false;
    }

    qInfo() << "[DapDNSController] Successfully set DNS servers using direct method";
    return true;
}

bool DapDNSController::trySetDNSWithNetworkManager(const QStringList &dnsServers)
{
    // Try using NetworkManager via dbus
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    
    // Get active connection
    QStringList args;
    args << "--system" << "call" << "--print-reply" << "--dest=org.freedesktop.NetworkManager" << "/org/freedesktop/NetworkManager" << "org.freedesktop.NetworkManager.GetActiveConnections";
    
    process.start("dbus-send", args);
    if (!process.waitForFinished(5000)) {
        qWarning() << "[DapDNSController] Failed to get active connections via NetworkManager";
        return false;
    }
    
    QString output = QString::fromLocal8Bit(process.readAll());
    
    // Parse the output to get active connection path
    QRegularExpression pathRegex("object path \"([^\"]+)\"");
    QRegularExpressionMatch match = pathRegex.match(output);
    
    if (!match.hasMatch()) {
        qWarning() << "[DapDNSController] No active connection found";
        return false;
    }
    
    QString connectionPath = match.captured(1);
    qInfo() << "[DapDNSController] Found active connection:" << connectionPath;
    
    // Try to set DNS using NetworkManager
    // Note: This is a simplified approach. In a real implementation, you'd need to:
    // 1. Get the connection settings
    // 2. Modify the IPv4/IPv6 settings
    // 3. Update the connection
    
    // For now, we'll use a simpler approach with nmcli
    return trySetDNSWithNmcli(dnsServers);
}

bool DapDNSController::trySetDNSWithNmcli(const QStringList &dnsServers)
{
    // Try using nmcli to set DNS
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    
    // Get active connection name
    QStringList args;
    args << "-t" << "-f" << "NAME,TYPE,DEVICE" << "connection" << "show" << "--active";
    process.start("nmcli", args);
    
    if (!process.waitForFinished(5000)) {
        qWarning() << "[DapDNSController] Failed to get active connections via nmcli";
        return false;
    }
    
    QString output = QString::fromLocal8Bit(process.readAll());
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    
    QString activeConnection;
    for (const QString &line : lines) {
        QStringList parts = line.split(':');
        if (parts.size() >= 3) {
            QString name = parts[0];
            QString type = parts[1];
            QString device = parts[2];
            
            // Skip VPN connections and loopback
            if (type != "vpn" && device != "lo" && !device.isEmpty()) {
                activeConnection = name;
                break;
            }
        }
    }
    
    if (activeConnection.isEmpty()) {
        qWarning() << "[DapDNSController] No suitable active connection found";
        return false;
    }
    
    qInfo() << "[DapDNSController] Using connection:" << activeConnection;
    
    // Set DNS servers
    QStringList dnsArgs;
    dnsArgs << "connection" << "modify" << activeConnection << "ipv4.dns" << dnsServers.join(',');
    process.start("nmcli", dnsArgs);
    
    if (!process.waitForFinished(5000)) {
        qWarning() << "[DapDNSController] Failed to set DNS via nmcli";
        return false;
    }
    
    if (process.exitCode() != 0) {
        qWarning() << "[DapDNSController] nmcli failed with exit code:" << process.exitCode();
        qWarning() << "[DapDNSController] Output:" << QString::fromLocal8Bit(process.readAll());
        return false;
    }
    
    // Restart the connection to apply DNS changes
    QStringList restartArgs;
    restartArgs << "connection" << "up" << activeConnection;
    process.start("nmcli", restartArgs);
    process.waitForFinished(3000);
    
    restartArgs.clear();
    restartArgs << "connection" << "up" << activeConnection;
    process.start("nmcli", restartArgs);
    process.waitForFinished(10000);
    
    if (process.exitCode() != 0) {
        qWarning() << "[DapDNSController] Failed to restart connection";
        return false;
    }
    
    qInfo() << "[DapDNSController] Successfully set DNS via NetworkManager/nmcli";
    return true;
}

bool DapDNSController::restoreDefaultDNSLinux()
{
    qInfo() << "[DapDNSController] Restoring default DNS on Linux";
    
    if (m_originalDNSServers.isEmpty()) {
        qWarning() << "[DapDNSController] No original DNS servers to restore";
        emit errorOccurred("No original DNS servers to restore");
        return false;
    }

    // Try resolvectl first (modern systemd-resolved)
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    
    // Get active interface
    QStringList statusArgs;
    statusArgs << "status";
    process.start("resolvectl", statusArgs);
    
    QString activeInterface;
    if (process.waitForFinished(3000)) {
        QString output = QString::fromLocal8Bit(process.readAll());
        QStringList lines = output.split('\n', Qt::SkipEmptyParts);
        
        qDebug() << "[DapDNSController] resolvectl status output for restoration:" << output;
        
        // Parse the output to find interfaces with DNS scope
        QString currentInterface;
        bool hasDNSScope = false;
        
        for (const QString &line : lines) {
            // Check for interface line
            if (line.startsWith("Link")) {
                // Save previous interface if it had DNS scope
                if (!currentInterface.isEmpty() && hasDNSScope) {
                    // Check if this is a VPN interface
                    if (currentInterface.startsWith("tun") || currentInterface.startsWith("tap")) {
                        activeInterface = currentInterface;
                        qInfo() << "[DapDNSController] Found VPN interface for restoration:" << activeInterface;
                        break;
                    }
                }
                
                // Extract interface name from "Link X (interface_name)"
                QRegularExpression linkRegex("Link \\d+ \\(([^)]+)\\)");
                QRegularExpressionMatch match = linkRegex.match(line);
                if (match.hasMatch()) {
                    currentInterface = match.captured(1);
                    hasDNSScope = false; // Reset for new interface
                }
            }
            // Check if current interface has DNS scope
            else if (!currentInterface.isEmpty() && line.contains("Current Scopes: DNS")) {
                hasDNSScope = true;
            }
        }
        
        // Check the last interface if it had DNS scope
        if (!currentInterface.isEmpty() && hasDNSScope) {
            if (currentInterface.startsWith("tun") || currentInterface.startsWith("tap")) {
                activeInterface = currentInterface;
                qInfo() << "[DapDNSController] Found VPN interface for restoration (last):" << activeInterface;
            }
        }
        
        // If no VPN interface found, look for any interface with DNS scope
        if (activeInterface.isEmpty()) {
            currentInterface.clear();
            hasDNSScope = false;
            
            for (const QString &line : lines) {
                if (line.startsWith("Link")) {
                    if (!currentInterface.isEmpty() && hasDNSScope) {
                        activeInterface = currentInterface;
                        qInfo() << "[DapDNSController] Found active interface for restoration:" << activeInterface;
                        break;
                    }
                    
                    QRegularExpression linkRegex("Link \\d+ \\(([^)]+)\\)");
                    QRegularExpressionMatch match = linkRegex.match(line);
                    if (match.hasMatch()) {
                        currentInterface = match.captured(1);
                        hasDNSScope = false;
                    }
                }
                else if (!currentInterface.isEmpty() && line.contains("Current Scopes: DNS")) {
                    hasDNSScope = true;
                }
            }
            
            // Check the last interface
            if (!currentInterface.isEmpty() && hasDNSScope) {
                activeInterface = currentInterface;
                qInfo() << "[DapDNSController] Found active interface for restoration (last):" << activeInterface;
            }
        }
        
        // If still no interface found, look for any interface (even without DNS scope)
        if (activeInterface.isEmpty()) {
            for (const QString &line : lines) {
                if (line.startsWith("Link")) {
                    QRegularExpression linkRegex("Link \\d+ \\(([^)]+)\\)");
                    QRegularExpressionMatch match = linkRegex.match(line);
                    if (match.hasMatch()) {
                        QString interfaceName = match.captured(1);
                        // Skip loopback and virtual interfaces
                        if (interfaceName != "lo" && !interfaceName.startsWith("virbr") && !interfaceName.startsWith("docker")) {
                            activeInterface = interfaceName;
                            qInfo() << "[DapDNSController] Found fallback interface for restoration:" << activeInterface;
                            break;
                        }
                    }
                }
            }
        }
    }
    
    if (!activeInterface.isEmpty()) {
        qInfo() << "[DapDNSController] Restoring DNS for interface:" << activeInterface;
        
        // Restore DNS using resolvectl
        QStringList setArgs;
        setArgs << "dns" << activeInterface;
        setArgs.append(m_originalDNSServers);
        
        qInfo() << "[DapDNSController] Running resolvectl restore command:" << "resolvectl" << setArgs;
        process.start("resolvectl", setArgs);
        if (process.waitForFinished(5000)) {
            if (process.exitCode() == 0) {
                qInfo() << "[DapDNSController] Successfully restored DNS using resolvectl";
                // Verify the restoration
                QStringList verifyArgs;
                verifyArgs << "status";
                process.start("resolvectl", verifyArgs);
                if (process.waitForFinished(3000)) {
                    QString verifyOutput = QString::fromLocal8Bit(process.readAll());
                    qInfo() << "[DapDNSController] DNS restoration verification output:" << verifyOutput;
                }
                return true;
            } else {
                qWarning() << "[DapDNSController] resolvectl failed with exit code:" << process.exitCode();
                qWarning() << "[DapDNSController] Output:" << QString::fromLocal8Bit(process.readAll());
            }
        }
    } else {
        qWarning() << "[DapDNSController] No active interface found for restoration";
    }

    // Try NetworkManager first
    if (trySetDNSWithNetworkManager(m_originalDNSServers)) {
        qInfo() << "[DapDNSController] Successfully restored DNS using NetworkManager";
        return true;
    }

    // Fallback to direct resolv.conf modification
    QFile resolvConf("/etc/resolv.conf");
    if (!resolvConf.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "[DapDNSController] Failed to open /etc/resolv.conf for writing";
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
        qWarning() << "[DapDNSController] Failed to set permissions on /etc/resolv.conf";
        emit errorOccurred("Failed to set permissions on /etc/resolv.conf");
        return false;
    }

    qInfo() << "[DapDNSController] Successfully restored DNS servers:" << m_originalDNSServers;
    return true;
}

QStringList DapDNSController::getCurrentDNSServersLinux() const
{
    QStringList dnsServers;
    
    // Try resolvectl first (modern systemd-resolved)
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    
    QStringList args;
    args << "status";
    process.start("resolvectl", args);
    
    if (process.waitForFinished(3000)) {
        QString output = QString::fromLocal8Bit(process.readAll());
        QStringList lines = output.split('\n', Qt::SkipEmptyParts);
        
        qDebug() << "[DapDNSController] resolvectl status output for reading:" << output;
        
        // Parse the output to find interfaces with DNS scope
        QString currentInterface;
        bool hasDNSScope = false;
        bool inInterfaceSection = false;
        
        for (const QString &line : lines) {
            // Check for interface line
            if (line.startsWith("Link")) {
                // Process previous interface if it had DNS scope
                if (!currentInterface.isEmpty() && hasDNSScope && inInterfaceSection) {
                    // Check if this is a VPN interface
                    if (currentInterface.startsWith("tun") || currentInterface.startsWith("tap")) {
                        qDebug() << "[DapDNSController] Found VPN interface:" << currentInterface;
                        // DNS servers should already be collected
                        break;
                    }
                }
                
                // Extract interface name from "Link X (interface_name)"
                QRegularExpression linkRegex("Link \\d+ \\(([^)]+)\\)");
                QRegularExpressionMatch match = linkRegex.match(line);
                if (match.hasMatch()) {
                    currentInterface = match.captured(1);
                    hasDNSScope = false;
                    inInterfaceSection = true;
                    dnsServers.clear(); // Clear for new interface
                    qDebug() << "[DapDNSController] Found interface:" << currentInterface;
                }
            }
            // Check if current interface has DNS scope
            else if (!currentInterface.isEmpty() && line.contains("Current Scopes: DNS")) {
                hasDNSScope = true;
                qDebug() << "[DapDNSController] Interface" << currentInterface << "has DNS scope";
            }
            // Look for DNS servers in current interface section
            else if (!currentInterface.isEmpty() && inInterfaceSection && line.contains("DNS Servers:")) {
                // Extract DNS servers from the line
                QString dnsPart = line.split("DNS Servers:").last().trimmed();
                QStringList dnsList = dnsPart.split(' ', Qt::SkipEmptyParts);
                
                qDebug() << "[DapDNSController] Found DNS servers line:" << line;
                qDebug() << "[DapDNSController] Parsed DNS part:" << dnsPart;
                qDebug() << "[DapDNSController] DNS list:" << dnsList;
                
                for (const QString &dns : dnsList) {
                    QString cleanDns = dns.trimmed();
                    if (isValidIPAddress(cleanDns)) {
                        dnsServers.append(cleanDns);
                    }
                }
            }
        }
        
        // Process the last interface if it had DNS scope
        if (!currentInterface.isEmpty() && hasDNSScope) {
            if (currentInterface.startsWith("tun") || currentInterface.startsWith("tap")) {
                qDebug() << "[DapDNSController] Found VPN interface (last):" << currentInterface;
                // DNS servers should already be collected
            }
        }
        
        // If no VPN interface found, look for any interface with DNS scope
        if (dnsServers.isEmpty()) {
            currentInterface.clear();
            hasDNSScope = false;
            inInterfaceSection = false;
            
            for (const QString &line : lines) {
                if (line.startsWith("Link")) {
                    if (!currentInterface.isEmpty() && hasDNSScope && inInterfaceSection) {
                        qDebug() << "[DapDNSController] Found active interface:" << currentInterface;
                        // DNS servers should already be collected
                        break;
                    }
                    
                    QRegularExpression linkRegex("Link \\d+ \\(([^)]+)\\)");
                    QRegularExpressionMatch match = linkRegex.match(line);
                    if (match.hasMatch()) {
                        currentInterface = match.captured(1);
                        hasDNSScope = false;
                        inInterfaceSection = true;
                        dnsServers.clear();
                    }
                }
                else if (!currentInterface.isEmpty() && line.contains("Current Scopes: DNS")) {
                    hasDNSScope = true;
                }
                else if (!currentInterface.isEmpty() && inInterfaceSection && line.contains("DNS Servers:")) {
                    QString dnsPart = line.split("DNS Servers:").last().trimmed();
                    QStringList dnsList = dnsPart.split(' ', Qt::SkipEmptyParts);
                    
                    for (const QString &dns : dnsList) {
                        QString cleanDns = dns.trimmed();
                        if (isValidIPAddress(cleanDns)) {
                            dnsServers.append(cleanDns);
                        }
                    }
                }
            }
            
            // Process the last interface
            if (!currentInterface.isEmpty() && hasDNSScope) {
                qDebug() << "[DapDNSController] Found active interface (last):" << currentInterface;
                // DNS servers should already be collected
            }
        }
    }
    
    // If resolvectl failed, try NetworkManager
    if (dnsServers.isEmpty()) {
        process.setProcessChannelMode(QProcess::MergedChannels);
        
        QStringList nmArgs;
        nmArgs << "-t" << "-f" << "NAME,TYPE,DEVICE" << "connection" << "show" << "--active";
        process.start("nmcli", nmArgs);
        
        if (process.waitForFinished(3000)) {
            QString output = QString::fromLocal8Bit(process.readAll());
            QStringList lines = output.split('\n', Qt::SkipEmptyParts);
            
            for (const QString &line : lines) {
                QStringList parts = line.split(':');
                if (parts.size() >= 3) {
                    QString name = parts[0];
                    QString type = parts[1];
                    QString device = parts[2];
                    
                    // Skip VPN connections and loopback
                    if (type != "vpn" && device != "lo" && !device.isEmpty()) {
                        // Get DNS for this connection
                        QStringList dnsArgs;
                        dnsArgs << "-t" << "-f" << "IP4.DNS" << "connection" << "show" << name;
                        QProcess dnsProcess;
                        dnsProcess.setProcessChannelMode(QProcess::MergedChannels);
                        dnsProcess.start("nmcli", dnsArgs);
                        
                        if (dnsProcess.waitForFinished(3000)) {
                            QString dnsOutput = QString::fromLocal8Bit(dnsProcess.readAll());
                            QRegularExpression dnsRegex("IP4\\.DNS:([0-9.,]+)");
                            QRegularExpressionMatch match = dnsRegex.match(dnsOutput);
                            
                            if (match.hasMatch()) {
                                QString dnsString = match.captured(1);
                                QStringList dnsList = dnsString.split(',', Qt::SkipEmptyParts);
                                for (const QString &dns : dnsList) {
                                    if (isValidIPAddress(dns.trimmed())) {
                                        dnsServers.append(dns.trimmed());
                                    }
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
    
    // Fallback to resolv.conf (legacy)
    if (dnsServers.isEmpty()) {
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
    }

    if (dnsServers.isEmpty()) {
        qWarning() << "[DapDNSController] No DNS servers found";
    } else {
        qDebug() << "[DapDNSController] Current DNS servers:" << dnsServers;
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
    
    // Ensure interface name is properly quoted if it contains spaces
    QString safeIface = iface.contains(' ') ? QString("\"%1\"").arg(iface) : iface;
    
    QStringList args = {"interface", "ip", "show", "dns",
                       QString("name=%1").arg(safeIface)};
    
    qDebug() << "getCurrentDNSIndexes: Executing command: netsh" << args;
    
    if (!runNetshCommand("netsh", args, &output)) {
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
    if (!runNetshCommand(cmd, QStringList(), &output)) {
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

#ifndef Q_OS_WINDOWS
// Stub implementations for non-Windows platforms
bool DapDNSController::setDNSServersForInterface(ulong ifIndex, const QStringList &dnsServers)
{
    Q_UNUSED(ifIndex);
    Q_UNUSED(dnsServers);
    qWarning() << "setDNSServersForInterface: Not implemented on this platform";
    emit errorOccurred("Interface-specific DNS management not implemented on this platform");
    return false;
}

bool DapDNSController::setDNSServerForInterface(ulong ifIndex, const QString &dnsServer)
{
    Q_UNUSED(ifIndex);
    Q_UNUSED(dnsServer);
    qWarning() << "setDNSServerForInterface: Not implemented on this platform";
    emit errorOccurred("Interface-specific DNS management not implemented on this platform");
    return false;
}

bool DapDNSController::restoreDNSForInterface(ulong ifIndex)
{
    Q_UNUSED(ifIndex);
    qWarning() << "restoreDNSForInterface: Not implemented on this platform";
    emit errorOccurred("Interface-specific DNS management not implemented on this platform");
    return false;
}

QStringList DapDNSController::getCurrentDNSServersForInterface(ulong ifIndex)
{
    Q_UNUSED(ifIndex);
    qWarning() << "getCurrentDNSServersForInterface: Not implemented on this platform";
    return QStringList();
}

#endif

// Emergency DNS restoration methods
bool DapDNSController::hasVPNDNSMarkers() const
{
    QMutexLocker locker(&m_mutex);
    
    // Check if we have stored original DNS settings (indicates we modified DNS)
    if (!m_originalDNSServers.isEmpty()) {
        return true;
    }
    
    // Check for VPN DNS markers in system
#ifdef Q_OS_WINDOWS
    // Check Windows registry for our DNS markers
    QStringList currentDNS = getCurrentDNSServersWindows();
    if (!currentDNS.isEmpty()) {
        // Look for VPN server IPs in current DNS settings
        for (const QString &dns : currentDNS) {
            if (dns.contains("10.") || dns.contains("172.") || dns.contains("192.168.")) {
                // Likely VPN DNS server
                return true;
            }
        }
    }
#endif

#ifdef Q_OS_LINUX
    // Check Linux resolv.conf for VPN DNS markers
    QStringList currentDNS = getCurrentDNSServersLinux();
    if (!currentDNS.isEmpty()) {
        for (const QString &dns : currentDNS) {
            if (dns.contains("10.") || dns.contains("172.") || dns.contains("192.168.")) {
                return true;
            }
        }
    }
#endif

#ifdef Q_OS_MACOS
    // Check macOS SystemConfiguration for VPN DNS markers
    QStringList currentDNS = getCurrentDNSServersMacOS();
    if (!currentDNS.isEmpty()) {
        for (const QString &dns : currentDNS) {
            if (dns.contains("10.") || dns.contains("172.") || dns.contains("192.168.")) {
                return true;
            }
        }
    }
#endif

    return false;
}

bool DapDNSController::emergencyDNSRestoration()
{
    qCritical() << "[DapDNSController] Emergency DNS restoration initiated";
    
    bool success = false;
    
    // Try multiple restoration methods
#ifdef Q_OS_WINDOWS
    // Method 1: Try registry restoration
    if (restoreDefaultDNSWindows()) {
        success = true;
    } else {
        // Method 2: Try netsh commands
        QProcess process;
        process.start("netsh", QStringList() << "interface" << "ip" << "set" << "dns" << "name=\"*\"", "dhcp");
        if (process.waitForFinished(5000) && process.exitCode() == 0) {
            success = true;
        } else {
            // Method 3: Try TunTap fallback
            if (TunTap::getInstance().resetDNS()) {
                success = true;
            }
        }
    }
#endif

#ifdef Q_OS_LINUX
    // Method 1: Try NetworkManager
    if (restoreDefaultDNSLinux()) {
        success = true;
    } else {
        // Method 2: Try direct resolv.conf restoration
        QProcess process;
        process.start("systemctl", QStringList() << "restart" << "systemd-resolved");
        if (process.waitForFinished(5000) && process.exitCode() == 0) {
            success = true;
        }
    }
#endif

#ifdef Q_OS_MACOS
    // Method 1: Try SystemConfiguration
    if (restoreDefaultDNSMacOS()) {
        success = true;
    } else {
        // Method 2: Try networksetup commands
        QProcess process;
        process.start("networksetup", QStringList() << "-setdnsservers" << "Wi-Fi" << "empty");
        if (process.waitForFinished(5000) && process.exitCode() == 0) {
            success = true;
        }
    }
#endif

    if (success) {
        qInfo() << "[DapDNSController] Emergency DNS restoration completed successfully";
        emit dnsRestored();
    } else {
        qCritical() << "[DapDNSController] Emergency DNS restoration failed!";
        emit errorOccurred("Emergency DNS restoration failed");
    }
    
    return success;
}

bool DapDNSController::detectOrphanedVPNDNS() const
{
    // Check if current DNS settings look like they were set by VPN
    QStringList currentDNS = getCurrentDNSServers();
    
    for (const QString &dns : currentDNS) {
        // Check for private IP ranges commonly used by VPNs
        if (dns.startsWith("10.") || dns.startsWith("172.") || dns.startsWith("192.168.")) {
            qWarning() << "[DapDNSController] Detected potential orphaned VPN DNS:" << dns;
            return true;
        }
    }
    
    return false;
}

// DNS Monitoring System Implementation

void DapDNSController::startDNSMonitoring(int intervalMs)
{
    QMutexLocker locker(&m_mutex);
    
    if (m_dnsMonitoringActive) {
        qDebug() << "[DapDNSController] DNS monitoring is already active";
        return;
    }
    
    m_monitoringInterval = intervalMs;
    m_consecutiveFailures = 0;
    m_dnsMonitoringActive = true;
    
    qInfo() << "[DapDNSController] Starting DNS monitoring with interval:" << intervalMs << "ms";
    
    m_dnsMonitorTimer->start(intervalMs);
    emit dnsMonitoringStarted();
}

void DapDNSController::stopDNSMonitoring()
{
    QMutexLocker locker(&m_mutex);
    
    if (!m_dnsMonitoringActive) {
        return;
    }
    
    qInfo() << "[DapDNSController] Stopping DNS monitoring";
    
    m_dnsMonitorTimer->stop();
    m_dnsMonitoringActive = false;
    m_consecutiveFailures = 0;
    
    emit dnsMonitoringStopped();
}

bool DapDNSController::isDNSMonitoringActive() const
{
    QMutexLocker locker(&m_mutex);
    return m_dnsMonitoringActive;
}

void DapDNSController::setVPNConnectionState(bool connected)
{
    QMutexLocker locker(&m_mutex);
    
    if (m_vpnConnected != connected) {
        m_vpnConnected = connected;
        qDebug() << "[DapDNSController] VPN connection state changed to:" << (connected ? "connected" : "disconnected");
        
        // If VPN disconnected, stop monitoring
        if (!connected) {
            stopDNSMonitoring();
        }
    }
}

bool DapDNSController::isVPNConnected() const
{
    QMutexLocker locker(&m_mutex);
    return m_vpnConnected;
}

void DapDNSController::setExpectedDNSServers(const QStringList &servers)
{
    QMutexLocker locker(&m_mutex);
    m_expectedDNSServers = servers;
    qDebug() << "[DapDNSController] Expected DNS servers set to:" << servers;
}

QStringList DapDNSController::getExpectedDNSServers() const
{
    QMutexLocker locker(&m_mutex);
    return m_expectedDNSServers;
}

void DapDNSController::onMonitoringTimerTimeout()
{
    // This method runs in the timer's thread context
    // We need to be careful about thread safety
    
    if (!m_vpnConnected || !m_dnsMonitoringActive) {
        return;
    }
    
    // Check DNS integrity
    checkDNSIntegrity();
}

void DapDNSController::checkDNSIntegrity()
{
    // This method should be called from the main thread
    if (QThread::currentThread() != this->thread()) {
        // If called from different thread, use QMetaObject::invokeMethod
        QMetaObject::invokeMethod(this, "checkDNSIntegrity", Qt::QueuedConnection);
        return;
    }
    
    if (!m_vpnConnected || m_expectedDNSServers.isEmpty()) {
        return;
    }
    
    // Get current DNS servers
    QStringList currentDNSServers = getCurrentDNSServers();
    
    // Check if DNS has been manually changed
    if (detectDNSChanges()) {
        qWarning() << "[DapDNSController] DNS manually changed detected!";
        qWarning() << "[DapDNSController] Expected:" << m_expectedDNSServers;
        qWarning() << "[DapDNSController] Current:" << currentDNSServers;
        
        // Log the change
        logDNSChange(currentDNSServers, m_expectedDNSServers);
        
        // Emit signal about manual change
        emit dnsManuallyChanged(currentDNSServers, m_expectedDNSServers);
        
        // Check if we should restore DNS
        if (shouldRestoreDNS()) {
            qInfo() << "[DapDNSController] Auto-restoring DNS settings...";
            
            // Attempt to restore DNS
            if (setDNSServers(m_expectedDNSServers)) {
                qInfo() << "[DapDNSController] DNS auto-restored successfully";
                emit dnsAutoRestored(m_expectedDNSServers);
                resetFailureCounter();
            } else {
                qCritical() << "[DapDNSController] Failed to auto-restore DNS";
                incrementFailureCounter();
                
                // If too many consecutive failures, stop monitoring
                if (m_consecutiveFailures >= MAX_CONSECUTIVE_FAILURES) {
                    qCritical() << "[DapDNSController] Too many consecutive failures, stopping DNS monitoring";
                    stopDNSMonitoring();
                    emit errorOccurred("DNS monitoring stopped due to repeated failures");
                }
            }
        }
    } else {
        // DNS is correct, reset failure counter
        resetFailureCounter();
    }
}

bool DapDNSController::detectDNSChanges()
{
    if (m_expectedDNSServers.isEmpty()) {
        return false;
    }
    
    QStringList currentDNSServers = getCurrentDNSServers();
    
    // If no current DNS servers, consider it a change
    if (currentDNSServers.isEmpty()) {
        return true;
    }
    
    // Check if current DNS servers match expected ones
    if (currentDNSServers.size() != m_expectedDNSServers.size()) {
        return true;
    }
    
    // Compare each DNS server
    for (int i = 0; i < currentDNSServers.size(); ++i) {
        if (currentDNSServers[i] != m_expectedDNSServers[i]) {
            return true;
        }
    }
    
    return false;
}

bool DapDNSController::shouldRestoreDNS() const
{
    // Always restore if VPN is connected and we have expected DNS servers
    return m_vpnConnected && !m_expectedDNSServers.isEmpty();
}

void DapDNSController::logDNSChange(const QStringList &detected, const QStringList &expected)
{
    QString activeInterface = getActiveNetworkInterface();
    
    qWarning() << "[DapDNSController] ===== DNS CHANGE DETECTED =====";
    qWarning() << "[DapDNSController] Timestamp:" << QDateTime::currentDateTime().toString(Qt::ISODate);
    qWarning() << "[DapDNSController] VPN Connected:" << m_vpnConnected;
    qWarning() << "[DapDNSController] Expected DNS:" << expected;
    qWarning() << "[DapDNSController] Detected DNS:" << detected;
    qWarning() << "[DapDNSController] Interface:" << activeInterface;
    qWarning() << "[DapDNSController] ================================";
}

QString DapDNSController::getActiveNetworkInterface() const
{
#ifdef Q_OS_LINUX
    // Try to get active interface using NetworkManager
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    
    QStringList args = {"-t", "-f", "NAME,TYPE,DEVICE", "connection", "show", "--active"};
    process.start("nmcli", args);
    
    if (process.waitForFinished(3000)) {
        QString output = QString::fromLocal8Bit(process.readAll());
        QStringList lines = output.split('\n', Qt::SkipEmptyParts);
        
        for (const QString &line : lines) {
            QStringList parts = line.split(':');
            if (parts.size() >= 3) {
                QString name = parts[0];
                QString type = parts[1];
                QString device = parts[2];
                
                // Skip VPN connections and loopback
                if (type != "vpn" && device != "lo" && !device.isEmpty()) {
                    return device;
                }
            }
        }
    }
    
    // Fallback: try to get default route interface
    process.start("ip", QStringList() << "route" << "get" << "8.8.8.8");
    if (process.waitForFinished(3000)) {
        QString output = QString::fromLocal8Bit(process.readAll());
        QRegularExpression devRegex(R"(dev\s+(\w+))");
        QRegularExpressionMatch match = devRegex.match(output);
        if (match.hasMatch()) {
            return match.captured(1);
        }
    }
#endif

#ifdef Q_OS_WINDOWS
    // Windows implementation would go here
    return "Windows Interface";
#endif

#ifdef Q_OS_MACOS
    // macOS implementation would go here
    return "macOS Interface";
#endif

    return "Unknown Interface";
}

void DapDNSController::resetFailureCounter()
{
    if (m_consecutiveFailures > 0) {
        qDebug() << "[DapDNSController] Resetting failure counter from" << m_consecutiveFailures << "to 0";
        m_consecutiveFailures = 0;
    }
}

void DapDNSController::incrementFailureCounter()
{
    m_consecutiveFailures++;
    qWarning() << "[DapDNSController] Incremented failure counter to" << m_consecutiveFailures;
}
