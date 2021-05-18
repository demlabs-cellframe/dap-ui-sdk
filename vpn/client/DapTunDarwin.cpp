/*
 * Authors:
 * Dmitriy A. Gearasimov <gerasimov.dmitriy@demlabs.net>
 * DeM Labs Ltd.   https://demlabs.net
 * Copyright  (c) 2021
 * All rights reserved.

 This file is part of DAP SDK project

    DAP SDK is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DAP SDK is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with any DAP SDK based project.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <QtDebug>
#include <QProcess>

#include <sys/socket.h>
#include <sys/sys_domain.h>
#include <sys/ioctl.h> // ioctl
#include <sys/kern_control.h> // struct socketaddr_ctl
#include <sys/event.h>
#include <sys/time.h>
#include <net/if.h>
#include <net/if_utun.h> // UTUN_CONTROL_NAME
#include <netinet/in.h>
#include <poll.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>


#include "DapSession.h"
#include "DapTunDarwin.h"
#include "DapTunWorkerDarwin.h"

/**
 * @brief DapTunDarwin::DapTunDarwin
 */
DapTunDarwin::DapTunDarwin( )
{
    tunWorker = new DapTunWorkerDarwin(this);
    initWorker();
}

/**
 * @brief DapTunDarwin::requestTunDeviceCreate
 */
void DapTunDarwin::tunDeviceCreate()
{
    if (m_tunSocket > 0) {
        qInfo() << "Socket already open";
        return;
    }
    long a_num = 5;
    int fd;
    m_tunDeviceName=QString("tun%1").arg(a_num);

    fd = ::open (QString("/dev/%1").arg(m_tunDeviceName).toLatin1().constData(),O_RDWR);

    if(fd<=0){
        qCritical() << "Can't open "<< m_tunDeviceName;
        emit error(tr("Can't open %1 device").arg(m_tunDeviceName));
    }else if (::fcntl(fd, F_SETFL, O_NONBLOCK) < 0){    ;
        qCritical() << "Can't switch socket to non-block mode";
        emit error(tr("Can't switch socket to non-block mode"));
    }else if (::fcntl(fd, F_SETFD, FD_CLOEXEC) < 0){    ;
        qCritical() << "Can't switch socket to not be passed across execs";
        emit error(tr("Can't switch socket to not be passed across execs"));
    }else{
        m_tunSocket = fd;
        qInfo()<<"Tun socket descriptor is ready for usage";
    }

}

/**
 * @brief DapTunDarwin::onWorkerStarted
 */
void DapTunDarwin::onWorkerStarted()
{
    QProcess process;
    DapTunUnixAbstract::onWorkerStarted();

    process.start("bash",QStringList() << "-c" << "netstat -rn|grep default| head -n 1| awk '{print $2;}'");

    if (m_defaultGwOld.isEmpty()) {
        qWarning() << "There is no default gateway, may be we've broken that last time? Trying to check that...";
        process.start("bash",QStringList() << "-c" << QString("netstat -rn|grep %1|awk '{print $2;}'").arg(upstreamAddress())  );
        process.waitForFinished(-1);
        m_defaultGwOld=process.readAllStandardOutput();
        m_defaultGwOld.chop(1);
        if (m_defaultGwOld.isEmpty()) {
            qWarning() << "Not found old gateway, looks like its better to restart the network";
            return;
        }
    } else {
        QString run = QString("route add -host %2 %1; route delete default %1")
            .arg(m_defaultGwOld).arg(upstreamAddress()).toLatin1().constData();
    }

    QString run = QString("route add default %1").arg( gw()  );

}

/**
 * @brief DapTunDarwin::onWorkerStopped
 */
void DapTunDarwin::onWorkerStopped()
{
    qInfo() <<"Restore network configuration";
    DapTunUnixAbstract::onWorkerStopped();

    ::system(QString(
       "route delete -host %2 %1; route delete default %3; route add default %1")
       .arg(m_defaultGwOld)
       .arg(upstreamAddress())
       .arg(gw() ).toLatin1().constData());

}
