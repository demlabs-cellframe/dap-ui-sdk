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
#include "../unix/DapTunUnixAbstract.h"

class DapUtun : public DapTunUnixAbstract
{
public:
    DapUtun();
    ~DapUtun() {}
protected:
    void tunDeviceCreate();
    void tunDeviceDestroy();
    void onWorkerStarted();

    // Getting currently using connection interface name from nmcli command-line tool
    // and save to m_lastUsedConnectionName and m_lastUsedConnectionDevice
    void saveCurrentConnectionInterfaceData();

private:

    // Connection witch used before creating utun Interface
    QString m_lastUsedConnectionName;
    QString m_lastUsedConnectionDevice;

    QStringList appleAdditionalRoutes; // List of Apple addresses needed to be routed directly through the tunnel

    QString runShellCmd(const QString& cmd);

    // Get current active internet interface
    QString getInternetInterface();
};
