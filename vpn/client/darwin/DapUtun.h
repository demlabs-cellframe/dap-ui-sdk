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

#pragma once

#include <QtDebug>
#include <QProcess>
#include <QFile>

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

#include "DapTunUnixAbstract.h"
#include "DapTunWorkerUnix.h"
#include "DapNetworkMonitor.h"
#include "SigUnixHandler.h"
#include "DapTunUnixAbstract.h"

class DapDNSController;

class DapUtun : public DapTunUnixAbstract
{
public:
    DapUtun();
    ~DapUtun() {}
protected:
    void executeCommand(const QString &cmd);
    void tunDeviceCreate();
    void tunDeviceDestroy();
    void onWorkerStarted();
    void clearAllDNS();
    void configureDNS(const QMap<QString, QString> &dnsMap);
    QString getCurrentNetworkInterface();


    // Getting currently using connection interface name from nmcli command-line tool
    // and save to m_lastUsedConnectionName and m_lastUsedConnectionDevice
    void saveCurrentConnectionInterfaceData();

private:

    QStringList getNetworkServices();
    QString getDNSServers(const QString &service);
    bool setDNS(const QString &service, const QString &dnsServer);

    // Connection witch used before creating utun Interface
    QString m_lastUsedConnectionName;
    QString m_lastUsedConnectionDevice;

    QString m_currentInterface;

    QStringList appleAdditionalRoutes; // List of Apple addresses needed to be routed directly through the tunnel

    // QString runShellCmd(const QString& cmd);

    // Get current active internet interface
    QString getInternetInterface();

    QMap<QString, QString> m_dnsMap;
    
    // DNS Controller for monitoring and managing DNS settings
    DapDNSController *m_dnsController;

};
