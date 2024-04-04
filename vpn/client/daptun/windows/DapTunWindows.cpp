#include <QtDebug>
#include <QProcess>

#include <QStringBuilder>

#include "DapTunWorkerWindows.h"
#include "DapTunWindows.h"
#include "DapNetworkMonitorWindows.h"

DapTunWindows::DapTunWindows()
{
    qInfo() << "Native windows tun driver ctl initialized";
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

    QFile f(QString("%1/%2/etc/%3").arg(regWGetUsrPath()).arg(DAP_BRAND).arg("split.json"));
    if (!f.open(QIODevice::ReadOnly)) {
        qInfo() << "No custom config found, route all traffic to tunnel";
        TunTap::getInstance().makeRoute(TunTap::TUN, "0.0.0.0", m_gw, metric_tun, "0.0.0.0");
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



