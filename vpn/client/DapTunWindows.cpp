#include <QtDebug>
#include <QProcess>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QThread>

#include <QStringBuilder>

#ifdef Q_OS_WINDOWS
#include <windows.h>
#include <processthreadsapi.h>
#include <securitybaseapi.h>
#include <winnt.h>
#endif

#include "DapTunWorkerWindows.h"
#include "DapTunWindows.h"
#include "DapNetworkMonitorWindows.h"
#include "DapDNSController.h"
#include "../../os/windows/tuntap/tuntap.h"

// Forward declarations for functions we need  
void exec_silent(const QString &command);

DapTunWindows::DapTunWindows()
    : m_dnsController(new DapDNSController(this))
{
    qInfo() << "[TUN] Native windows tun driver ctl initialized";
    tunThread = new QThread();
    tunWorker = new DapTunWorkerWindows(this);
    connect(this, &DapTunAbstract::sigStartWorkerLoop, tunWorker, &DapTunWorkerAbstract::loop);
    initWorker();
}

/**
 * @brief DapTunWindows::tunDeviceCreate
 */
void DapTunWindows::tunDeviceCreate()
{
    if (m_tunSocket <= 0) {
        // Enhanced TUN device creation with TAP adapter checks and remediation
        qInfo() << "[TUN] Creating TUN device...";
        
        // Check administrator privileges
        if (!isRunAsAdmin()) {
            qWarning() << "[TUN] Warning: Not running as administrator - TAP operations may fail";
        }
        
        // Pre-creation check for TAP adapter
        if (!preTunCreationCheck()) {
            qWarning() << "[TUN] TAP adapter check failed, attempting remediation...";
            if (!attemptTapRemediation()) {
                qCritical() << "[TUN] TAP adapter remediation failed";
            }
        }
        
        // Attempt to create TUN device
        m_tunSocket = TunTap::getInstance().makeTunTapDevice(m_tunDeviceName);
        
        // If creation failed, try alternative methods
        if (m_tunSocket <= 0) {
            qWarning() << "[TUN] Primary TUN creation failed, trying alternatives...";
            if (!attemptAlternativeTunCreation()) {
                qCritical() << "[TUN] All TUN creation methods failed";
                emit error("Failed to create TUN device - please check TAP driver installation");
                return;
            }
        }
        
        m_isCreated = true;
        qInfo() << "[TUN] TUN device created successfully, socket:" << m_tunSocket;
    } else {
        qDebug() << "Wake up";
        TunTap::getInstance().wakeupTun();
    }
    TunTap::getInstance().unassignTunAdp();
}

void DapTunWindows::tunDeviceDestroy()
{
    qInfo() << "Closing tunnel";
    m_gw.chop(1);
    m_gw.append('0');
    TunTap::getInstance().deleteRoutesByGw(qPrintable(m_gw));
    TunTap::getInstance().enableDefaultRoutes();
    //TunTap::getInstance().resetDNS();
    TunTap::getInstance().deleteRoutesByIfIndex(static_cast<DWORD>(TunTap::getInstance().getTunTapAdapterIndex()));
    TunTap::getInstance().deleteCustomRoutes();
    TunTap::getInstance().determineValidArgs(metric_eth, metric_tun);
    //TunTap::getInstance().makeRoute(TunTap::ETH, "0.0.0.0", m_defaultGwOld, metric_eth, "0.0.0.0", false);
    TunTap::getInstance().closeTun();

    /*if (dhcpEnabled) {
        if (TunTap::getInstance().ipReleaseAddr(TunTap::getInstance().getDefaultAdapterIndex()) == 0) {
            qInfo() << "[TunTap] ip config released";
        } else {
            qCritical() << "[TunTap] ip config couldn't be released";
        }

        if (TunTap::getInstance().ipRenewAddr(TunTap::getInstance().getDefaultAdapterIndex()) == 0) {
            qInfo() << "[TunTap] ip address renewed";
        } else {
            qCritical() << "[TunTap] ip config couldn't be renewed";
        }

        if (!TunTap::getInstance().notifyIPChange(m_ethDeviceName)) {
            qInfo() << "Re-registering DNS...";
            exec_silent("ipconfig /registerdns");
        }
    }*/

    qInfo() << "Flushing DNS cache...";
    if(!TunTap::getInstance().flushDNS()) {
        qDebug() << "Re-flushing DNS...";
            exec_silent("ipconfig /flushdns");
    }

    emit destroyed();
}

void DapTunWindows::onWorkerStarted() {
    DapTunAbstract::onWorkerStarted();
    m_defaultGwOld = TunTap::getInstance().getDefaultGateWay();
    if (m_defaultGwOld.isEmpty()) {
        qCritical() << "Default gateway is undefined, odd network settings!";
        return;
    } else {
        qInfo() << "Default gateway: " << m_defaultGwOld;
    }
    m_ethDeviceName = TunTap::getInstance().getNameAndDefaultDNS(TunTap::getInstance().getDefaultAdapterIndex());
    m_tunDeviceReg = TunTap::getInstance().getNameAndDefaultDNS(TunTap::getInstance().getTunTapAdapterIndex());
    DapNetworkMonitorWindows::instance()->sltSetIfIndex(static_cast<ulong>(TunTap::getInstance().getDefaultAdapterIndex()));
    DapNetworkMonitorWindows::instance()->sltSetTapIfIndex(static_cast<ulong>(TunTap::getInstance().getTunTapAdapterIndex()));
    DapNetworkMonitorWindows::instance()->sltSetAdpDefined(true);
    upstreamResolved = TunTap::getInstance().lookupHost(m_sUpstreamAddress, QString::number(m_iUpstreamPort));
    qInfo() << "Upstream address: " << upstreamResolved;

    if (!TunTap::getInstance().determineValidArgs(metric_eth, metric_tun)) {
        qCritical() << "Couldn't determine proper metrics...";
        metric_eth = 35;
        metric_tun = 100;
    }

    TunTap::getInstance().setDNS(TunTap::getInstance().getTunTapAdapterIndex(), m_gw);

    /*dhcpEnabled = TunTap::getInstance().dhcpEnabled(TunTap::getInstance().getDefaultAdapterIndex());
    if (dhcpEnabled) {
        TunTap::getInstance().setDNS(TunTap::getInstance().getDefaultAdapterIndex(), m_gw);
        if (!TunTap::getInstance().notifyIPChange(m_ethDeviceName)) {
            qInfo() << "Re-registering DNS...";
            exec_silent("ipconfig /registerdns");
        }
    }*/

    TunTap::getInstance().deleteRoutesByIfIndex(TunTap::getInstance().getTunTapAdapterIndex());
    //TunTap::getInstance().defaultRouteDelete();
    //TunTap::getInstance().deleteRoutesByGw("0.0.0.0");
    TunTap::getInstance().makeRoute(TunTap::ETH, upstreamResolved,  m_defaultGwOld, metric_eth);

    QFile f(QString("%1/%2/etc/%3").arg(QString::fromWCharArray(regWGetUsrPath())).arg(DAP_BRAND).arg("split.json"));
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
    if(!TunTap::getInstance().flushDNS()) {
        qDebug() << "Re-flushing DNS...";
        exec_silent("ipconfig /flushdns");
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

// TAP adapter troubleshooting helper methods
bool DapTunWindows::isRunAsAdmin() {
    // Check if the current process is running as administrator
    BOOL isElevated = FALSE;
    HANDLE hToken = NULL;
    
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION elevation;
        DWORD cbSize = sizeof(TOKEN_ELEVATION);
        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &cbSize)) {
            isElevated = elevation.TokenIsElevated;
        }
    }
    
    if (hToken) {
        CloseHandle(hToken);
    }
    
    qInfo() << "[TUN] Administrator privileges:" << (isElevated ? "YES" : "NO");
    return isElevated;
}

bool DapTunWindows::preTunCreationCheck() {
    // Check if TAP adapter exists and is enabled using netsh
    qInfo() << "[TUN] Checking TAP adapter status...";
    
    QProcess process;
    process.start("netsh", QStringList() << "interface" << "show" << "interface");
    process.waitForFinished(5000);
    
    QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
    qDebug() << "[TUN] Interface list:" << output;
    
    // Look for TAP adapter in the output
    QStringList lines = output.split('\n');
    for (const QString &line : lines) {
        if (line.contains("TAP", Qt::CaseInsensitive) || 
            line.contains("KelVPN", Qt::CaseInsensitive) ||
            line.contains("Ethernet") && line.contains("Connected")) {
            qInfo() << "[TUN] Found potential TAP interface:" << line.trimmed();
            return true;
        }
    }
    
    qWarning() << "[TUN] No TAP adapter found or adapter not connected";
    return false;
}

bool DapTunWindows::attemptTapRemediation() {
    qInfo() << "[TUN] Attempting TAP adapter remediation...";
    
    // Try to enable TAP adapter using simplified TunTap method
    if (TunTap::getInstance().enableTapAdapter()) {
        qInfo() << "[TUN] TAP adapter enabled successfully";
        return true;
    }
    
    qWarning() << "[TUN] TAP adapter enablement failed - please ensure TAP driver is installed and user has administrator privileges";
    return false;
}

bool DapTunWindows::attemptAlternativeTunCreation() {
    qInfo() << "[TUN] Attempting alternative TUN creation methods...";
    
    // Wait a bit and retry
    QThread::msleep(2000);
    qInfo() << "[TUN] Retrying TUN device creation after delay...";
    
    int retrySocket = TunTap::getInstance().makeTunTapDevice(m_tunDeviceName);
    if (retrySocket > 0) {
        qInfo() << "[TUN] Alternative TUN creation successful, socket:" << retrySocket;
        m_tunSocket = retrySocket;
        return true;
    }
    
    qCritical() << "[TUN] All TUN creation attempts failed";
    return false;
}


