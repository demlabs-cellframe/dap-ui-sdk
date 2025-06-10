#include <QtDebug>
#include <QProcess>

#include <QStringBuilder>

#include "DapTunWorkerWindows.h"
#include "DapTunWindows.h"
#include "DapNetworkMonitorWindows.h"
#include "DapDNSController.h"

DapTunWindows::DapTunWindows()
    : m_dnsController(new DapDNSController(this))
{
    qInfo() << "[TUN] Native windows tun driver ctl initialized";
    qInfo() << "[TUN] Creating DNS controller instance";
    
    tunThread = new QThread();
    tunWorker = new DapTunWorkerWindows(this);
    connect(this, &DapTunAbstract::sigStartWorkerLoop, tunWorker, &DapTunWorkerAbstract::loop);
    
    // Connect DNS controller signals
    connect(m_dnsController, &DapDNSController::errorOccurred, this, [this](const QString &error) {
        qWarning() << "[TUN] DNS Controller error:" << error;
    });
    
    connect(m_dnsController, &DapDNSController::dnsServersChanged, this, [this](const QStringList &servers) {
        qInfo() << "[TUN] DNS servers updated to:" << servers;
    });
    
    connect(m_dnsController, &DapDNSController::dnsRestored, this, []() {
        qInfo() << "[TUN] DNS settings restored to original configuration";
    });
    
    initWorker();
}

DapTunWindows::~DapTunWindows() = default;

/**
 * @brief DapTunWindows::tunDeviceCreate
 */
void DapTunWindows::tunDeviceCreate()
{
    if (m_tunSocket <= 0) {
        m_tunSocket=TunTap::getInstance().makeTunTapDevice(m_tunDeviceName);
        m_isCreated = true;
    } else {
        qDebug() << "Wake up";
        TunTap::getInstance().wakeupTun();
    }
    TunTap::getInstance().unassignTunAdp();
}

void DapTunWindows::tunDeviceDestroy()
{
    qInfo() << "[TUN] Starting tunnel destruction...";
    m_gw.chop(1);
    m_gw.append('0');
    
    qInfo() << "[TUN] Deleting routes by gateway:" << m_gw;
    TunTap::getInstance().deleteRoutesByGw(qPrintable(m_gw));
    
    qInfo() << "[TUN] Enabling default routes";
    TunTap::getInstance().enableDefaultRoutes();
    
    qInfo() << "[TUN] Deleting routes by interface index:" << TunTap::getInstance().getTunTapAdapterIndex();
    TunTap::getInstance().deleteRoutesByIfIndex(static_cast<DWORD>(TunTap::getInstance().getTunTapAdapterIndex()));
    
    qInfo() << "[TUN] Deleting custom routes";
    TunTap::getInstance().deleteCustomRoutes();
    
    qInfo() << "[TUN] Determining valid metrics";
    TunTap::getInstance().determineValidArgs(metric_eth, metric_tun);
    
    // Restore original DNS settings using improved DapDNSController
    qInfo() << "[TUN] Restoring original DNS settings...";
    
    // Restore DNS for TUN adapter
    ulong tunIfIndex = static_cast<ulong>(TunTap::getInstance().getTunTapAdapterIndex());
    if (!m_dnsController->restoreDNSForInterface(tunIfIndex)) {
        qWarning() << "[TUN] Failed to restore DNS for TUN adapter via DapDNSController";
        // Fallback to TunTap method
        if (!TunTap::getInstance().setDNS(tunIfIndex, "")) {
            qWarning() << "[TUN] Failed to clear DNS for TUN adapter via TunTap";
        }
    }
    
    // Restore DNS for ethernet adapter if it was modified
    bool dhcpEnabled = TunTap::getInstance().dhcpEnabled(TunTap::getInstance().getDefaultAdapterIndex());
    if (dhcpEnabled) {
        ulong ethIfIndex = static_cast<ulong>(TunTap::getInstance().getDefaultAdapterIndex());
        if (!m_dnsController->restoreDNSForInterface(ethIfIndex)) {
            qWarning() << "[TUN] Failed to restore DNS for ethernet adapter via DapDNSController";
            // Use TunTap fallback if available
            TunTap::getInstance().resetDNS();
        }
    }
    
    // Alternative: use general restore method if interface-specific restore not available
    if (m_dnsController->isDNSSet()) {
        qInfo() << "[TUN] Using general DNS restore method...";
        if (!m_dnsController->restoreDefaultDNS()) {
            qWarning() << "[TUN] Failed to restore original DNS settings via general method";
        }
    }

    qInfo() << "[TUN] Flushing DNS cache...";
    if (!m_dnsController->flushDNSCache()) {
        qInfo() << "[TUN] DapDNSController flush failed, trying TunTap method...";
        if (!TunTap::getInstance().flushDNS()) {
            qWarning() << "[TUN] TunTap flush failed, using command line fallback...";
            exec_silent("ipconfig /flushdns");
        }
    }

    qInfo() << "[TUN] Closing TUN device";
    TunTap::getInstance().closeTun();

    emit destroyed();
}

void DapTunWindows::onWorkerStarted()
{
    qInfo() << "[TUN] Worker starting...";
    DapTunAbstract::onWorkerStarted();
    
    // Get default gateway with error handling
    m_defaultGwOld = TunTap::getInstance().getDefaultGateWay();
    if (m_defaultGwOld.isEmpty()) {
        qCritical() << "[TUN] Default gateway is undefined, odd network settings!";
        emit error("Failed to determine default gateway");
        return;
    }
    qInfo() << "[TUN] Default gateway:" << m_defaultGwOld;
    
    // Get network device information with logging
    m_ethDeviceName = TunTap::getInstance().getNameAndDefaultDNS(TunTap::getInstance().getDefaultAdapterIndex());
    qInfo() << "[TUN] Ethernet device name:" << m_ethDeviceName;
    
    m_tunDeviceReg = TunTap::getInstance().getNameAndDefaultDNS(TunTap::getInstance().getTunTapAdapterIndex());
    qInfo() << "[TUN] TUN device name:" << m_tunDeviceReg;
    
    // Configure network monitoring
    DapNetworkMonitorWindows::instance()->sltSetIfIndex(static_cast<ulong>(TunTap::getInstance().getDefaultAdapterIndex()));
    DapNetworkMonitorWindows::instance()->sltSetTapIfIndex(static_cast<ulong>(TunTap::getInstance().getTunTapAdapterIndex()));
    DapNetworkMonitorWindows::instance()->sltSetAdpDefined(true);
    
    // Resolve upstream address with validation
    upstreamResolved = TunTap::getInstance().lookupHost(m_sUpstreamAddress, QString::number(m_iUpstreamPort));
    if (upstreamResolved.isEmpty()) {
        qCritical() << "[TUN] Failed to resolve upstream address:" << m_sUpstreamAddress;
        emit error("Failed to resolve VPN server address");
        return;
    }
    qInfo() << "[TUN] Upstream address resolved to:" << upstreamResolved;

    // Determine network metrics with fallback
    if (!TunTap::getInstance().determineValidArgs(metric_eth, metric_tun)) {
        qWarning() << "[TUN] Couldn't determine proper metrics, using defaults";
        metric_eth = 35;
        metric_tun = 100;
    }
    qInfo() << "[TUN] Using metrics - ETH:" << metric_eth << "TUN:" << metric_tun;

    // Set DNS using improved DapDNSController with registry method
    qInfo() << "[TUN] Setting VPN DNS server:" << m_gw;
    ulong tunIfIndex = static_cast<ulong>(TunTap::getInstance().getTunTapAdapterIndex());
    if (!m_dnsController->setDNSServerForInterface(tunIfIndex, m_gw)) {
        qWarning() << "[TUN] Failed to set VPN DNS server using DapDNSController";
        // Fallback to direct TunTap method
        qInfo() << "[TUN] Trying fallback DNS setting method...";
        if (!TunTap::getInstance().setDNS(tunIfIndex, m_gw)) {
            qWarning() << "[TUN] Fallback DNS setting also failed";
        }
    }

    // Handle DHCP interfaces if needed
    bool dhcpEnabled = TunTap::getInstance().dhcpEnabled(TunTap::getInstance().getDefaultAdapterIndex());
    if (dhcpEnabled) {
        qInfo() << "[TUN] DHCP detected on default adapter";
        ulong ethIfIndex = static_cast<ulong>(TunTap::getInstance().getDefaultAdapterIndex());
        
        // Try setting DNS on ethernet adapter using DapDNSController
        if (!m_dnsController->setDNSServerForInterface(ethIfIndex, m_gw)) {
            qWarning() << "[TUN] Failed to set DNS on ethernet adapter via DapDNSController";
            // Fallback to TunTap method
            if (!TunTap::getInstance().setDNS(ethIfIndex, m_gw)) {
                qWarning() << "[TUN] Failed to set DNS on ethernet adapter via TunTap";
            }
        }
        
        // Notify system of IP changes
        if (!TunTap::getInstance().notifyIPChange(m_ethDeviceName)) {
            qInfo() << "[TUN] Re-registering DNS via command line...";
            exec_silent("ipconfig /registerdns");
        }
    }

    // Clear existing TUN routes
    qInfo() << "[TUN] Deleting routes by interface index:" << TunTap::getInstance().getTunTapAdapterIndex();
    TunTap::getInstance().deleteRoutesByIfIndex(TunTap::getInstance().getTunTapAdapterIndex());
    
    // Create route to VPN server via ethernet
    qInfo() << "[TUN] Making route - ETH to:" << upstreamResolved << "via" << m_defaultGwOld << "metric:" << metric_eth;
    TunTap::getInstance().makeRoute(TunTap::ETH, upstreamResolved, m_defaultGwOld, metric_eth);

    // Handle routing configuration (split tunneling vs full tunnel)
    QFile f(QString("%1/%2/etc/%3").arg(regWGetUsrPath()).arg(DAP_BRAND).arg("split.json"));
    if (!f.open(QIODevice::ReadOnly)) {
        qInfo() << "[TUN] No custom config found, route all traffic to tunnel";
        TunTap::getInstance().makeRoute(TunTap::TUN, "0.0.0.0", m_gw, metric_tun, "0.0.0.0");
        
        // Add CDB routing exceptions
        for (const auto &str : m_routingExceptionAddrs) {
            TunTap::getInstance().makeRoute(TunTap::ETH, str, m_defaultGwOld, metric_eth);
        }
        
        TunTap::getInstance().enableDefaultRoutes(static_cast<ulong>(TunTap::getInstance().getDefaultAdapterIndex()), false);
    } else {
        // Handle split tunneling configuration
        QByteArray configJsonBytes = f.readAll();
        f.close();
        QJsonParseError jsonErr;
        auto configJsonDoc = QJsonDocument::fromJson(configJsonBytes, &jsonErr);
        if (configJsonDoc.isNull()) {
            qCritical() << "[TUN] JSON parse error" << jsonErr.errorString() << "at position" << jsonErr.offset;
            // Fallback to full tunneling
            qWarning() << "[TUN] Using full tunneling due to JSON parse error";
            TunTap::getInstance().makeRoute(TunTap::TUN, "0.0.0.0", m_gw, metric_tun, "0.0.0.0");
        } else {
            qInfo() << "[TUN] Applying split tunneling configuration";
            auto configArray = configJsonDoc.array();
            for (const auto obj : configArray) {
                auto configObject = obj.toObject();
                qInfo() << "[TUN] Applying config for" << configObject.value("Name").toString();
                auto addrsArray = configObject.value("Addresses").toArray();
                for (const auto addr : addrsArray) {
                    auto l = addr.toString().split("/");
                    if (l.count() == 2) {
                        if (l.last().contains(".")) {
                            TunTap::getInstance().makeRoute(TunTap::TUN, l.first(), m_gw, metric_tun, l.last());
                        } else {
                            uint l_umask = (0xFFFFFFFF << (32 - l.last().toInt())) & 0xFFFFFFFF;
                            char l_mask[16] = {'\0'};
                            sprintf_s(l_mask, sizeof(l_mask), "%u.%u.%u.%u", l_umask >> 24, (l_umask >> 16) & 0xFF, (l_umask >> 8) & 0xFF, l_umask & 0xFF);
                            TunTap::getInstance().makeRoute(TunTap::TUN, l.first(), m_gw, metric_tun, l_mask);
                        }
                    } else if (l.count() == 1) {
                        TunTap::getInstance().makeRoute(TunTap::TUN, l.first(), m_gw, metric_tun);
                    }
                }
            }
        }
    }

    // Flush DNS cache with improved fallback handling
    qInfo() << "[TUN] Flushing DNS cache...";
    if (!m_dnsController->flushDNSCache()) {
        qInfo() << "[TUN] DapDNSController flush failed, trying TunTap method...";
        if (!TunTap::getInstance().flushDNS()) {
            qInfo() << "[TUN] TunTap flush failed, using command line fallback...";
            if (exec_silent("ipconfig /flushdns") != 0) {
                qWarning() << "[TUN] All DNS flush methods failed";
            }
        }
    }
    
    emit created();
}

void DapTunWindows::workerPrepare() {
    qInfo() <<"Prepare worker";
}

void DapTunWindows::workerStart() {
    qInfo() <<"Tun device created" << m_tunSocket;
    if (m_tunSocket > 0){
        tunWorker->setTunSocket(m_tunSocket);
        tunWorker->moveToThread(tunThread);
        tunThread->start();
        emit sigStartWorkerLoop();
        qInfo() << "tunThread started, tun socket: " << m_tunSocket;
    } else {
        qInfo() << "Tun socket " << m_tunSocket << " already opened";
        return;
    }
    onWorkerStarted();
}

void DapTunWindows::workerStop() {
    TunTap::getInstance().doCloseTun();
    tunThread->quit();
    tunThread->wait();
    onWorkerStopped();
}

void DapTunWindows::workerPause() {
    TunTap::getInstance().doCloseTun();
    tunThread->quit();
    tunThread->wait();
}

void DapTunWindows::signalWriteQueueProc() {
    Dap::Stream::Packet *ret;
    if ((ret = writeDequeuePacket()) != nullptr) {
        emit bytesRead(TunTap::getInstance().write_tun(m_tunSocket, ret->data, ret->header.op_data.data_size));
        delete ret;
        ret = nullptr;
    }
}

void DapTunWindows::addNewUpstreamRoute(const QString &a_addr) {
    TunTap::getInstance().determineValidArgs(metric_eth, metric_tun);
    TunTap::getInstance().makeRoute(TunTap::ETH, a_addr,  m_defaultGwOld, metric_eth);
}

void DapTunWindows::onWorkerStopped()
{
    DapTunAbstract::onWorkerStopped();
}



