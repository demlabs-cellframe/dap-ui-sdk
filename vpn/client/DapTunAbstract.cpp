#include <QtDebug>
#include <QXmlStreamReader>
#include <QDomElement>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QFile>
#include <unistd.h>

#include "DapTunAbstract.h"
#include "DapSockForwPacket.h"
#ifndef Q_OS_ANDROID
#include "DapNetworkMonitor.h"
#endif

#define STREAM_SF_PACKET_OP_CODE_RAW_SEND 0xbc

/**
 * @brief DapTunAbstract::DapTunAbstract
 */
DapTunAbstract::DapTunAbstract()
    : QObject(nullptr)
{
    tunWorker = nullptr;
    m_tunSocket = -1;
    m_isCreated = false;
}

/**
 * @brief DapTunAbstract::isCreated
 */
bool DapTunAbstract::isCreated()
{
    return m_isCreated;
}

/**
 * @brief DapTunAbstract::initWorker
 */
void DapTunAbstract::initWorker()
{
    qDebug() << "[DapTunAbstract] Tun worker init";
#ifndef Q_OS_ANDROID
    DapNetworkMonitor::instance(); // To call a contructor in main thread.
    DapNetworkMonitor::instance()->monitoringStart();
    /*connect(tunThread, &QThread::started, [&] {
        DapNetworkMonitor::instance()->sltSetServerAddress(this->m_sUpstreamAddress);
        DapNetworkMonitor::instance()->sltSetTunnelDestination(this->m_addr);
        DapNetworkMonitor::instance()->sltSetTunGateway(this->m_gw);
    });*/
#endif

    if (tunWorker == nullptr) {
        qWarning() << "[DapTunAbstract::initWorker] tunWorker is nullptr";
        return;
    }
    connect(tunWorker,&DapTunWorkerAbstract::loopStarted,this,&DapTunAbstract::created);
    connect(tunWorker,&DapTunWorkerAbstract::packetOut,this, &DapTunAbstract::packetOut);
    connect(tunWorker,&DapTunWorkerAbstract::bytesRead, this, &DapTunAbstract::bytesRead);
    connect(tunWorker,&DapTunWorkerAbstract::bytesWrite, this, &DapTunAbstract::bytesWrite);
    connect(tunWorker,&DapTunWorkerAbstract::loopError,this, &DapTunAbstract::error);
    connect(tunWorker,&DapTunWorkerAbstract::loopStopped,this,&DapTunAbstract::onWorkerStopped);
}

/**
 * @brief DapTunAbstract::create
 * @param a_addr
 * @param a_gw
 * @param a_upstreamAddress
 * @param a_upstreamSocket
 * @details Create tunnel with pointed address, gateway and protection of upstream socket and address
 */
void DapTunAbstract::create(const QString &a_addr, const QString &a_gw,
                            const QString & a_upstreamAddress , qint16 a_upstreamPort,
                            int a_upstreamSocket, QStringList a_routing_exceptions)
{
    qInfo() << "address = "        << a_addr
            << "gw ="              << a_gw
            << "upstreamAddress =" << a_upstreamAddress
            << "upstreamSocket ="  << a_upstreamSocket;
    m_addr = a_addr;
    m_gw = a_gw;
    m_sUpstreamAddress = a_upstreamAddress;
    m_iUpstreamPort = a_upstreamPort;
    m_upstreamSocket = a_upstreamSocket;
    m_routingExceptionAddrs = a_routing_exceptions;
    
#ifdef Q_OS_DARWIN
    // macOS: NetworkExtension manages TUN interface automatically
    // Call tunDeviceCreate() to start NetworkExtension Provider with proper network config
    qInfo() << "[DapTunAbstract] macOS: NE manages TUN; calling tunDeviceCreate() for NetworkExtension";
    qInfo() << "[DapTunAbstract] Platform detected: Q_OS_DARWIN defined";
    // CRITICAL FIX: Call tunDeviceCreate() to start NetworkExtension Provider
    // emit created() will be called from DapTunDarwin::vpnConnectionStatusChanged when status is NEVPNStatusConnected
    qInfo() << "[DapTunAbstract] macOS: Starting NetworkExtension Provider with network config";
    tunDeviceCreate();
    return;
#else
    qInfo() << "[DapTunAbstract] Platform: NOT macOS, using legacy TUN";
#endif
    
    tunDeviceCreate();
}

/**
 * @brief DapTunAbstract::destroy
 */
void DapTunAbstract::destroy()
{
    if(m_tunSocket) {
        workerStop();
    } else {
        qWarning() << "Tunnel is not working";
    }
}

void DapTunAbstract::standby() {
    workerPause();
}

/**
 * @brief DapTunAbstract::onWorkerStarted
 */
void DapTunAbstract::onWorkerStarted()
{
    qInfo()<<"Worker loop started";
}

/**
 * @brief DapTunAbstract::onWorkerStopped
 */
void DapTunAbstract::onWorkerStopped()
{
    qInfo()<<"Worker loop stopped";
    tunDeviceDestroy();
    afterTunDeviceDestroyed();
}


/**
 * @brief DapTunAbstract::afterTunDeviceDestroyed
 * @details Clear everything after tunnel destroy
 */
void DapTunAbstract::afterTunDeviceDestroyed()
{
    qInfo() <<"Tun device destroyed";
    m_addr.clear();
    m_gw.clear();
    m_gwOld.clear();
    m_sUpstreamAddress.clear();
    m_upstreamSocket=0;
    m_tunSocket=0;
    m_isCreated = false;
}

int parseTextToInt(QString text, int a, int b, char splitChar)
{
    return text.split(' ')[a].split(splitChar)[b].toInt();
}

/**
 * @brief DapTunAbstract::readNetrowkInformFromFile
 * @details Restores network settings from the file
 */
void DapTunAbstract::readNetrowkInformFromFile()
{
    QFile file(tempNetFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Error open " << tempNetFileName;
        return;
    }

    QXmlStreamReader xml(&file);
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();

        if(xml.name() == "tunAddr") {
            xml.readNext();
            qDebug() << "tunAddr = " << xml.text();
            xml.readNext();
        } else if(xml.name() == "tunDest") {
            xml.readNext();
            qDebug() << "tunDest = " << xml.text();
            xml.readNext();
        } else if(xml.name() == "ip_host") {
            xml.readNext();
            qDebug() << "ip_host = " << xml.text();
            xml.readNext();
        } else if(xml.name() == "default_gw") {
            xml.readNext();
            qDebug() << "default_gw = " << xml.text();
            xml.readNext();
        } else if(xml.name() == "date_create") {
            xml.readNext();
            QString strDate = xml.text().toString();
            int year  = parseTextToInt(strDate, 0, 0, '.');
            int month = parseTextToInt(strDate, 0, 1, '.');
            int day   = parseTextToInt(strDate, 0, 2, '.');
            int hours = parseTextToInt(strDate, 1, 0, ':');
            int min   = parseTextToInt(strDate, 1, 1, ':');
            QDateTime dt(
                QDate(year, month, day),
                QTime(hours, min));

            qDebug() << "Differense(sec) = " << dt.secsTo(QDateTime::currentDateTime());
            xml.readNext();
        }
    }

    file.close();
}

bool DapTunAbstract::isLocalAddress(const QString& address)
{
    //In accordance with the IANA standard
    QStringList octets = address.split('.');
    if(octets.size() < 4)
        return false;
    int firstOctet = octets[0].toInt();
    int secondOctet = octets[1].toInt();
    if(firstOctet == 10)
        return true;
    else if(firstOctet == 172 && secondOctet >= 16 && secondOctet < 32)
        return true;
    else if(firstOctet == 192 && secondOctet == 168)
        return true;
    return false;

}

/// Get upstream address.
/// @return Upstream address.
QString DapTunAbstract::upstreamAddress() const
{
    return m_sUpstreamAddress;
}

/// Set upstream address.
/// @param arsUpstreamAddress Upstream address.
void DapTunAbstract::setUpstreamAddress(const QString &arsUpstreamAddress)
{
    m_sUpstreamAddress = arsUpstreamAddress;
}

/// Get upstream port.
/// @return Upstream port.
qint16 DapTunAbstract::upstreamPort() const
{
    return m_iUpstreamPort;
}

/// Set upstream port.
/// @param aiUpstreamPort Upstream port.
void DapTunAbstract::setUpstreamPort(qint16 iUpstreamPort)
{
    m_iUpstreamPort = iUpstreamPort;
}
