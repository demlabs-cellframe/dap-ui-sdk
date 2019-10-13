#include <QtDebug>
#include <QProcess>

#include <winsock2.h>
#include <iphlpapi.h>

#include <QStringBuilder>

#include "DapTunWorkerWindows.h"
#include "DapTunWindows.h"

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
    //TunTap::getInstance().unassignTunAdp();
    m_tunSocket=TunTap::getInstance().makeTunTapDevice(m_tunDeviceName);
    m_isCreated = true;
}

void DapTunWindows::tunDeviceDestroy()
{
    qDebug() << "Closing tunnel";
    TunTap::getInstance().setDNS(m_tunDeviceReg, "");
    TunTap::getInstance().closeTun();
    QString _gw(m_gw);
    _gw.chop(1);
    _gw.append('0');
    qDebug() << "Restore ROUTE ... ";

    //QString run = QString("ROUTE DELETE 0.0.0.0 & ROUTE DELETE 128.0.0.0 & ROUTE DELETE %6 & ROUTE DELETE %1 & ROUTE DELETE %4 & ROUTE DELETE %5 & ROUTE ADD 0.0.0.0 MASK 0.0.0.0 %2 IF %3").arg(upstreamResolved).arg(m_defaultGwOld).arg(TunTap::getInstance().getDefaultAdapterIndex()).arg(_gw).arg(m_gw).arg(m_addr);
    //exec_silent(qPrintable(run));
    dwDests.append(_gw);
    TunTap::getInstance().deleteRoutes(dwDests);
    dwDests.clear();
    TunTap::getInstance().makeRoute(TunTap::ETH, "0.0.0.0", m_defaultGwOld, metric_eth, "0.0.0.0");
    if (!dhcpEnabled) {
        /*run = m_defaultDNSRecord.join(',');
        TunTap::getInstance().setDNS(m_ethDeviceName, run); */ // pconst: not yet sure whether it's totally useless
    }
    else {
        /*run = "";
        TunTap::getInstance().setDNS(m_ethDeviceName, run); */

        if (TunTap::getInstance().ipReleaseAddr(TunTap::getInstance().getDefaultAdapterIndex()) == 0) {
            qDebug() << "[TunTap] ip config released";
        } else {
            qCritical() << "[TunTap] ip config couldn't be released";
        }

        if (TunTap::getInstance().ipRenewAddr(TunTap::getInstance().getDefaultAdapterIndex()) == 0) {
            qDebug() << "[TunTap] ip address renewed";
        } else {
            qCritical() << "[TunTap] ip config couldn't be renewed";
        }

        if (!TunTap::getInstance().notifyIPChange(m_ethDeviceName)) {
            qDebug() << "Re-registering DNS...";
            exec_silent("ipconfig /registerdns");
        }
    }

    qDebug() << "Flushing DNS cache...";
    if(!TunTap::getInstance().flushDNS()) {
        qDebug() << "Re-flushing DNS...";
        exec_silent("ipconfig /flushdns");
    }

    emit destroyed();
}

void DapTunWindows::onWorkerStarted() {
    DapTunAbstract::onWorkerStarted();
    dhcpEnabled = false;

    m_defaultGwOld = TunTap::getInstance().getDefaultGateWay();
    QString m_defaultGwOld_test = TunTap::getInstance()._getDefaultGateWay(dhcpEnabled);
    qDebug() << "default GW as to route: " << m_defaultGwOld;
    qDebug() << "default GW as to IpString: " << m_defaultGwOld_test;
    QStringList dDevice = TunTap::getInstance().getFriendlyNameAndDefaultDNS(TunTap::getInstance().getDefaultAdapterIndex());
    m_ethDevice = dDevice.at(0);
    m_ethDeviceName = dDevice.at(1);
    dDevice.removeFirst();
    dDevice.removeFirst();
    m_defaultDNSRecord = dDevice; // pconst: not yet sure whether it's totally useless.
    qDebug() << "default dns records:" << m_defaultDNSRecord;

    QStringList tunDevice = TunTap::getInstance().getFriendlyNameAndDefaultDNS(TunTap::getInstance().getTunTapAdapterIndex());
    m_tunDeviceHuman = tunDevice.at(0);
    m_tunDeviceReg = tunDevice.at(1);
    qDebug() << "tun device reg name: " << qPrintable(m_tunDeviceReg);
    if (m_defaultGwOld.isEmpty()) {
        qWarning() << "[DapChSockForw] Not found old gateway, looks like its better to restart the network";
        return;
    }

    qDebug() << "Attempt to resolve domain name address...";
    upstreamResolved = TunTap::getInstance().lookupHost(m_sUpstreamAddress, QString::number(m_iUpstreamPort));
    qDebug() << "upstream address is " << upstreamResolved;

    if (!TunTap::getInstance().determineValidArgs(metric_eth, metric_tun)) {
        metric_eth = 35;
        metric_tun = 4280; // pconst: no idea, depends on machine...
    }

    //TunTap::getInstance().setDNS(m_ethDeviceName, m_gw); // pconst: not yet sure whether it's totally useless...
    TunTap::getInstance().setDNS(m_tunDeviceReg, m_gw);

    if (dhcpEnabled) {
        if (!TunTap::getInstance().notifyIPChange(m_ethDeviceName)) {
            qDebug() << "Re-registering DNS...";
            exec_silent("ipconfig /registerdns");
        }
    }
    TunTap::getInstance().defaultRouteDelete();
    TunTap::getInstance().makeRoute(TunTap::ETH, "0.0.0.0",         m_defaultGwOld, metric_eth, "0.0.0.0"   );
    TunTap::getInstance().makeRoute(TunTap::TUN, "0.0.0.0",         m_gw,           metric_tun, "128.0.0.0" );
    TunTap::getInstance().makeRoute(TunTap::TUN, "128.0.0.0",       m_gw,           metric_tun, "128.0.0.0" );
    TunTap::getInstance().makeRoute(TunTap::ETH, upstreamResolved,  m_defaultGwOld, metric_eth              );

    qInfo() << "Flushing DNS cache...";
    if(!TunTap::getInstance().flushDNS()) {
        qDebug() << "Re-flushing DNS...";
        exec_silent("ipconfig /flushdns");
    }

    emit created();

    dwDests.append("0.0.0.0");
    dwDests.append("128.0.0.0");
    dwDests.append(m_addr);
    dwDests.append(upstreamResolved);
    dwDests.append(m_gw);
}

void DapTunWindows::workerPrepare() {
    qInfo() <<"Prepare worker";
}

void DapTunWindows::workerStop() {
    TunTap::getInstance().doCloseTun();
    onWorkerStopped();
}

void DapTunWindows::signalWriteQueueProc() {
    DapSockForwPacket *ret;
    if ((ret = writeDequeuePacket()) != nullptr) {
        emit bytesRead(TunTap::getInstance().write_tun(m_tunSocket, ret->data, ret->header.op_data.data_size));
        delete ret;
        ret = nullptr;
    }
}





