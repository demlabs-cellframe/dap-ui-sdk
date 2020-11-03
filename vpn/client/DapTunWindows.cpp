#include <QtDebug>
#include <QProcess>

#include <QStringBuilder>

#include "DapTunWorkerWindows.h"
#include "DapTunWindows.h"
#include "DapNetworkMonitorWindows.h"

DapTunWindows::DapTunWindows()
{
    qInfo() << "Native windows tun driver ctl initialized";
    tunWorker = new DapTunWorkerWindows(this);
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
        TunTap::getInstance().wakeupTun();
    }
    TunTap::getInstance().unassignTunAdp();
}

void DapTunWindows::tunDeviceDestroy()
{
    qInfo() << "Closing tunnel";
    TunTap::getInstance().setDNS(TunTap::getInstance().getDefaultAdapterIndex(), "");
    m_gw.chop(1);
    m_gw.append('0');
    TunTap::getInstance().deleteRoutesByGw(qPrintable(m_gw));
    //QString run = QString("ROUTE DELETE 0.0.0.0 & ROUTE DELETE 128.0.0.0 & ROUTE DELETE %6 & ROUTE DELETE %1 & ROUTE DELETE %4 & ROUTE DELETE %5 & ROUTE ADD 0.0.0.0 MASK 0.0.0.0 %2 IF %3").arg(upstreamResolved).arg(m_defaultGwOld).arg(TunTap::getInstance().getDefaultAdapterIndex()).arg(_gw).arg(m_gw).arg(m_addr);
    //exec_silent(qPrintable(run));
    TunTap::getInstance().enableDefaultRoutes();
    TunTap::getInstance().resetDNS();
    TunTap::getInstance().deleteCustomRoutes();
    TunTap::getInstance().determineValidArgs(metric_eth, metric_tun);
    TunTap::getInstance().makeRoute(TunTap::ETH, "0.0.0.0", m_defaultGwOld, metric_eth, "0.0.0.0", false);
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
    qInfo() << "Default gateway: " << m_defaultGwOld;
    m_ethDeviceName = TunTap::getInstance().getNameAndDefaultDNS(TunTap::getInstance().getDefaultAdapterIndex());
    m_tunDeviceReg = TunTap::getInstance().getNameAndDefaultDNS(TunTap::getInstance().getTunTapAdapterIndex());
    if (m_defaultGwOld.isEmpty()) {
        qWarning() << "[DapChSockForw] Not found old gateway, looks like its better to restart the network";
        return;
    }

    qInfo() << "Attempt to resolve domain name address...";
    upstreamResolved = TunTap::getInstance().lookupHost(m_sUpstreamAddress, QString::number(m_iUpstreamPort));
    qInfo() << "upstream address is " << upstreamResolved;


    if (!TunTap::getInstance().determineValidArgs(metric_eth, metric_tun)) {
        qCritical() << "Couldn't determine proper metrics...";
        metric_eth = 35;
        metric_tun = 100; // pconst: no idea, depends on machine...
    }

    TunTap::getInstance().setDNS(TunTap::getInstance().getTunTapAdapterIndex(), m_gw);
    dhcpEnabled = TunTap::getInstance().dhcpEnabled(TunTap::getInstance().getDefaultAdapterIndex());
    if (dhcpEnabled) {
        TunTap::getInstance().setDNS(TunTap::getInstance().getDefaultAdapterIndex(), m_gw);
        if (!TunTap::getInstance().notifyIPChange(m_ethDeviceName)) {
            qInfo() << "Re-registering DNS...";
            exec_silent("ipconfig /registerdns");
        }
    }
    TunTap::getInstance().deleteRoutesByIfIndex(TunTap::getInstance().getTunTapAdapterIndex());
    TunTap::getInstance().makeRoute(TunTap::TUN, "0.0.0.0",         m_gw,           metric_tun, "0.0.0.0" );
    TunTap::getInstance().makeRoute(TunTap::ETH, upstreamResolved,  m_defaultGwOld, metric_eth);
    TunTap::getInstance().enableDefaultRoutes(TunTap::getInstance().getDefaultAdapterIndex(), false);

    qInfo() << "Flushing DNS cache...";
    if(!TunTap::getInstance().flushDNS()) {
        qDebug() << "Re-flushing DNS...";
        exec_silent("ipconfig /flushdns");
    }
    //DapNetworkMonitorWindows::instance()->monitoringStart();
    emit created();
}

void DapTunWindows::workerPrepare() {
    qInfo() <<"Prepare worker";
}

void DapTunWindows::workerStop() {
    TunTap::getInstance().doCloseTun();
    onWorkerStopped();
}

void DapTunWindows::workerPause() {
    TunTap::getInstance().doCloseTun();
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



