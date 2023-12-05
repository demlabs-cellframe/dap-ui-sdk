/* Authors:
* Dmitriy A. Gearasimov <gerasimov.dmitriy@demlabs.net>
* Demlabs Ltd   https://demlabs.net
* DAP UI SDK  https://gitlab.demlabs.net/dap/dap-ui-sdk
* Copyright  (c) 2022
* All rights reserved.

This file is part of DAP UI SDK the open source project

   DAP UI SDK is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   DAP SDK is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with any DAP based project.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtDebug>
#include <QProcess>
#include <QFile>
#include <QSettings>

#include <stdarg.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/sys_domain.h>
#include <sys/kern_control.h>
#include <net/if_utun.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>



#include "DapUtils.h"
#include "DapTunUnixAbstract.h"
#include "QtCore/qdebug.h"
#include "darwin/DapUtun.h"
#include "DapTunWorkerUnix.h"
#include "DapNetworkMonitor.h"
#include "SigUnixHandler.h"

/**
 * @brief DapUtun::DapUtun
 */
DapUtun::DapUtun()
    :DapTunUnixAbstract()
{
    // List of Apple addresses needed to be routed directly through the tunnel
    appleAdditionalRoutes = QStringList({
        "18.203.89.116","23.56.184.20","35.190.80.1","52.12.45.81","52.215.192.131","54.64.248.78",
        "74.125.140.188","74.125.206.189", "91.105.192.100", "104.17.244.204","104.18.28.208",
        "104.18.29.120","104.18.157.26","108.138.217.3","108.138.233.14","108.156.28.57","108.156.32.59",
        "108.177.15.189","136.143.189.136","136.143.190.84","136.143.191.197","142.250.178.10",
        "142.250.179.234","142.250.187.196","142.250.200.37","142.250.200.46","149.154.167.41",
        "157.240.240.17","157.240.240.18","172.67.167.199","172.217.169.34","172.217.169.35",
        "172.217.169.67","172.217.169.78","185.20.209.147","185.64.190.78","195.93.246.49",
        "199.60.103.225","216.239.32.116",
        "224.0.0.0 240.0.0.0",
        "255.255.255.255 255.255.255.255" });

    connect(SigUnixHandler::getInstance(), &SigUnixHandler::sigKill,
            this, &DapUtun::tunDeviceDestroy, Qt::DirectConnection);
}

/**
 * @brief DapUtun::requestTunDeviceCreate
 */
void DapUtun::tunDeviceCreate()
{
    qDebug() << "[DapUtun::tunDeviceCreate]";
    if (m_tunSocket > 0) {
        qInfo() << "Socket already open";
        return;
    }
    // Prepare structs
    struct ctl_info l_ctl_info = {0};

    // Copy utun control name
    if (::strlcpy(l_ctl_info.ctl_name, UTUN_CONTROL_NAME, sizeof(l_ctl_info.ctl_name))
            >= sizeof(l_ctl_info.ctl_name)){
        emit error( QString("UTUN_CONTROL_NAME % is too long").arg(UTUN_CONTROL_NAME));
        return;
    }

    // Create utun socket
    int l_tun_fd = ::socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
    if( l_tun_fd < 0){
        int l_errno = errno;
        char l_errbuf[256];
        ::strerror_r(l_errno, l_errbuf,sizeof(l_errbuf));
        emit error ( QString("Opening utun device control (SYSPROTO_CONTROL) error: '%1' (code %2)").arg(l_errbuf).arg(l_errno));
        return;
    }
    qInfo() << "Utun SYSPROTO_CONTROL descriptor obtained";

    // Pass control structure to the utun socket
    if( ::ioctl(l_tun_fd, CTLIOCGINFO, &l_ctl_info ) < 0 ){
        int l_errno = errno;
        char l_errbuf[256];
        ::strerror_r(l_errno, l_errbuf,sizeof(l_errbuf));
        emit error (QString("Can't execute ioctl(CTLIOCGINFO): '%1' (code %1)").arg(l_errbuf).arg(l_errno));
        return;
    }
    qInfo() << "Utun CTLIOCGINFO structure passed through ioctl";

    // Trying to connect with one of utunX devices
    int l_ret = -1;
    for(int l_unit = 0; l_unit < 256; l_unit++){
        struct sockaddr_ctl l_sa_ctl = {0};
        l_sa_ctl.sc_id = l_ctl_info.ctl_id;
        l_sa_ctl.sc_len = sizeof(l_sa_ctl);
        l_sa_ctl.sc_family = AF_SYSTEM;
        l_sa_ctl.ss_sysaddr = AF_SYS_CONTROL;
        l_sa_ctl.sc_unit = l_unit + 1;

        // If connect successful, new utunX device should be created
        l_ret = ::connect(l_tun_fd, (struct sockaddr *)&l_sa_ctl, sizeof(l_sa_ctl));
        if(l_ret == 0)
            break;
    }
    // Check if the previous step was successful
    if (l_ret < 0){
        int l_errno = errno;
        char l_errbuf[256];
        ::strerror_r(l_errno, l_errbuf,sizeof(l_errbuf));
        emit error( QString("Can't create utun device: '%1' (code %2)").arg(l_errbuf).arg(l_errno));
        return;
    }

    // Get iface name of newly created utun dev.
    qInfo() << "Utun device created";
    char l_utunname[20];
    l_utunname[0] = '\0';
    socklen_t l_utunname_len = sizeof(l_utunname);
    if (::getsockopt(l_tun_fd, SYSPROTO_CONTROL, UTUN_OPT_IFNAME, l_utunname, &l_utunname_len) ){
        int l_errno = errno;
        char l_errbuf[256];
        ::strerror_r(l_errno, l_errbuf,sizeof(l_errbuf));
        emit error ( QString("Can't get utun device name: '%s' (code %d)").arg(l_errbuf).arg(l_errno));
        return;
    }

    m_tunDeviceName = QString::fromLatin1(l_utunname,l_utunname_len-1);
    qInfo() << "Created utun "<<m_tunDeviceName<<" network interface";
    
    /* this is the special file descriptor that the caller will use to talk
    * with the virtual interface */
    m_tunSocket = l_tun_fd;
}

/**
 * @brief Detects active interface by default routing to 8.8.8.8
 * @return  Returns current active internet interface
 */
QString DapUtun::getInternetInterface()
{
    return DapUtils::shellCmd("route get 8.8.8.8 | grep interface | awk '{print $2;}'");
}

/**
 * @brief DapUtun::saveCurrentConnectionInterfaceData
 */
void DapUtun::saveCurrentConnectionInterfaceData()
{

    m_lastUsedConnectionDevice = this->getInternetInterface();
    qDebug() << "Current internet interface "<< m_lastUsedConnectionDevice;
    QString result = DapUtils::shellCmd(QString("networksetup -listnetworkserviceorder | grep 'Hardware Port' | grep %1").arg(m_lastUsedConnectionDevice));

    QStringList res1 = result.split(":"); // Break down answer by Hardware port: and Device

    if(res1.length() < 3) {
        qWarning() << "Can't get current connection interface name! Command returns "<< result;
        return;
    }

    QStringList res2 = res1[1].split(","); // Split from ,Device

    if(res2.length() < 2) {
        qWarning() << " Wrong networksetup answer! Command returns "<< result;
        return;
    }

    m_lastUsedConnectionName = res2[0].trimmed();
    qDebug() << "Current internet connection name" << m_lastUsedConnectionName;

    result = DapUtils::shellCmd(QString("networksetup -getinfo \"%1\" | grep Router").arg(m_lastUsedConnectionName));
    QStringList res3 =result.split("\n", Qt::SkipEmptyParts);
    if(res3.length() < 1) {
        qWarning() << "No default router at all";
    }else{
        QStringList res4 = res3[0].split(":");
        if(res3.length() != 2) {
            qWarning() << "No default route address in line";
        }else{
            m_defaultGwOld =  res4[1] == "none" ? QString() : res4[1];
        }
    }

    qInfo() << "DeviceName name:" << m_lastUsedConnectionDevice
             << "Interface Name:" << m_lastUsedConnectionName
             << "Router" << m_defaultGwOld
                ;

}


/**
 * @brief DapUtun::onWorkerStarted
 */
void DapUtun::onWorkerStarted()
{
    qDebug() << "tunnelCreate()";
    QProcess process;
    
    if(m_tunSocket <=0){
        qCritical()<< "Can't bring up network interface ";
        return;
    }

    saveCurrentConnectionInterfaceData();

//    networksetup -ordernetworkservices

    // Update route table for upstream if its not local
    if(!isLocalAddress(upstreamAddress()))
    {
        // This route dont need if address is local
        //QString run = QString("networksetup -setadditionalroute \"%3\" %2 255.255.255.255 %1")
        //        .arg(m_defaultGwOld).arg(upstreamAddress()).arg(m_lastUsedConnectionName) ;
        QString run = QString("route add -host %2 %1")
                .arg(m_defaultGwOld).arg(upstreamAddress()) ;
        qDebug() << "Execute "<<run;
        ::system(run.toLatin1().constData());
    }


    // Create connection
    /*
    QString cmdConnAdd = QString(
                "networksetup -createnetworkservice %1 %2 ;"
                "networksetup -setnetworkserviceenabled %1 on;"
                "networksetup -setmanual %1 %3 255.255.255.255 %4;"
                "networksetup -setdnsservers %4"
                ) .arg (DAP_BRAND)
                  .arg(tunDeviceName())
                  .arg(addr())
                  .arg(gw());
    
    qDebug() << "[Cmd to created interface: " <<  cmdConnAdd;
    
    DapTunUnixAbstract::runShellCmd( cmdConnAdd );


    // Add additional Apple routes
    QString cmdAddAdditionalRoutes = QString("networksetup -setadditionalroutes %1").arg(DAP_BRAND);
    foreach(QString additionalRoute, appleAdditionalRoutes){
        cmdAddAdditionalRoutes += additionalRoute;
        cmdAddAdditionalRoutes += "\"\"";
    }

    DapTunUnixAbstract::runShellCmd( cmdAddAdditionalRoutes);*/

    // Bring up network interface

    ::system ( QString("ifconfig %1 %2 %3").arg(tunDeviceName() ).arg(addr()).arg(gw()).toLatin1().constData());

    qDebug() << "Configured " << tunDeviceName()<< " with " << addr() << "-->" << gw();

    // Remove old default route
    ::system( QString("route delete default %1").arg(m_defaultGwOld).toLatin1().constData() );


    qDebug() << "Removed default route " <<m_defaultGwOld;

    // Setup default route
    ::system ( QString("route add default %1").arg(gw()).toLatin1().constData() );
    qDebug() << "Added default route" <<gw();
    // Add additional Apple routes
    foreach(QString additionalRoute, appleAdditionalRoutes){
        QString routeNet, routeMask;
        QStringList routeArgs = additionalRoute.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

        if(routeArgs.length() == 1 ){
            ::system( QString("route add -host %1 %2")
                                 .arg(routeArgs[0]).arg(gw()).toLatin1().constData() );
        }else if (routeArgs.length() == 2){
            ::system( QString("route add -net %1 %3")
                                 .arg(routeArgs[0]).arg(routeArgs[1]).arg(gw()).toLatin1().constData());
        }else{
            qWarning() << "Unparseble additional route line " << additionalRoute;
        }
    }

    m_isCreated = true;
    emit created();
}

/**
 * @brief DapUtun::onWorkerStopped
 */
void DapUtun::tunDeviceDestroy()
{
    DapTunUnixAbstract::tunDeviceDestroy();
    // Delete upstream routes
    if(!isLocalAddress(upstreamAddress()))
    {
        QString run = QString("route del -host %2 %1")
                .arg(m_defaultGwOld).arg(upstreamAddress()) ;
        qDebug() << "Execute "<<run;
        ::system( run.toLatin1().constData() );
    }

    // Other routes connected with tunnel should be destroyed autimaticly

    // Restore default gateway
    ::system ( QString("route add default %1").arg(m_defaultGwOld).toLatin1().constData());
    qInfo() << "Restored default route "<< m_defaultGwOld;
}

