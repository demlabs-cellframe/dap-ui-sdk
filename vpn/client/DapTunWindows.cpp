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
    
    // Restore original DNS settings before closing tunnel
    if (m_dnsController->isDNSSet()) {
        qInfo() << "[TUN] Restoring original DNS settings...";
        if (!m_dnsController->restoreDefaultDNS()) {
            qWarning() << "[TUN] Failed to restore original DNS settings";
        }
    }

    qInfo() << "[TUN] Flushing DNS cache...";
    if(!m_dnsController->flushDNSCache()) {
        qWarning() << "[TUN] Failed to flush DNS cache";
    }

    qInfo() << "[TUN] Closing TUN device";
    TunTap::getInstance().closeTun();

    emit destroyed();
}

void DapTunWindows::onWorkerStarted()
{
    qInfo() << "[TUN] Worker starting...";
    DapTunAbstract::onWorkerStarted();
    
    m_defaultGwOld = TunTap::getInstance().getDefaultGateWay();
    if (m_defaultGwOld.isEmpty()) {
        qCritical() << "[TUN] Default gateway is undefined, odd network settings!";
        return;
    } else {
        qInfo() << "[TUN] Default gateway:" << m_defaultGwOld;
    }
    
    m_ethDeviceName = TunTap::getInstance().getNameAndDefaultDNS(TunTap::getInstance().getDefaultAdapterIndex());
    qInfo() << "[TUN] Ethernet device name:" << m_ethDeviceName;
    
    m_tunDeviceReg = TunTap::getInstance().getNameAndDefaultDNS(TunTap::getInstance().getTunTapAdapterIndex());
    qInfo() << "[TUN] TUN device name:" << m_tunDeviceReg;
    
    DapNetworkMonitorWindows::instance()->sltSetIfIndex(static_cast<ulong>(TunTap::getInstance().getDefaultAdapterIndex()));
    DapNetworkMonitorWindows::instance()->sltSetTapIfIndex(static_cast<ulong>(TunTap::getInstance().getTunTapAdapterIndex()));
    DapNetworkMonitorWindows::instance()->sltSetAdpDefined(true);
    
    upstreamResolved = TunTap::getInstance().lookupHost(m_sUpstreamAddress, QString::number(m_iUpstreamPort));
    qInfo() << "[TUN] Upstream address resolved to:" << upstreamResolved;

    if (!TunTap::getInstance().determineValidArgs(metric_eth, metric_tun)) {
        qCritical() << "[TUN] Couldn't determine proper metrics, using defaults";
        metric_eth = 35;
        metric_tun = 100;
    }
    qInfo() << "[TUN] Using metrics - ETH:" << metric_eth << "TUN:" << metric_tun;

    // Set VPN DNS server
    qInfo() << "[TUN] Setting VPN DNS server:" << m_gw;
    if (!m_dnsController->setDNSServers(QStringList{m_gw})) {
        qWarning() << "[TUN] Failed to set VPN DNS server. Check administrator privileges and network interface status";
    }

    qInfo() << "[TUN] Deleting routes by interface index:" << TunTap::getInstance().getTunTapAdapterIndex();
    TunTap::getInstance().deleteRoutesByIfIndex(TunTap::getInstance().getTunTapAdapterIndex());
    
    qInfo() << "[TUN] Making route - ETH to:" << upstreamResolved << "via" << m_defaultGwOld << "metric:" << metric_eth;
    TunTap::getInstance().makeRoute(TunTap::ETH, upstreamResolved, m_defaultGwOld, metric_eth);

    QFile f(QString("%1/%2/etc/%3").arg(regWGetUsrPath()).arg(DAP_BRAND).arg("split.json"));
    if (!f.open(QIODevice::ReadOnly)) {
        qInfo() << "No custom config found, route all traffic to tunnel";
        TunTap::getInstance().makeRoute(TunTap::TUN, "0.0.0.0", m_gw, metric_tun, "0.0.0.0");
        // Add all CDBs into routing exeption
        for (const auto &str : m_routingExceptionAddrs)
            TunTap::getInstance().makeRoute(TunTap::ETH, str, m_defaultGwOld, metric_eth);


        TunTap::getInstance().enableDefaultRoutes(static_cast<ulong>(TunTap::getInstance().getDefaultAdapterIndex()), false);
    } else {
        QByteArray configJsonBytes = f.readAll();
        f.close();
        QJsonParseError jsonErr;
        auto configJsonDoc = QJsonDocument::fromJson(configJsonBytes, &jsonErr);
        if (configJsonDoc.isNull()) {
            qCritical() << "JSON parse error " << jsonErr.errorString() << " on pos " << jsonErr.offset;
        }

        auto configArray = configJsonDoc.array();
        for (const auto obj : configArray) {
            auto configObject = obj.toObject();
            qInfo() << "Applying config for " << configObject.value("Name").toString();
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

    qInfo() << "Flushing DNS cache...";
    if(!m_dnsController->flushDNSCache()) {
        qDebug() << "Failed to flush DNS cache";
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



