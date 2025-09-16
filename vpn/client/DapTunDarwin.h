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
#pragma once

#include "DapTunUnixAbstract.h"
#include <objc/objc-runtime.h>

// Forward declarations for Objective-C types (using void* for C++ compatibility)
typedef void* NETunnelProviderManagerRef;
typedef void* NSNotificationRef;


class DapTunDarwin:public DapTunUnixAbstract
{
public:
    DapTunDarwin();
    void workerStart()     override;
protected:
    QThread *tunThread;
    void tunDeviceCreate() override;
    void onWorkerStarted() override;
    void onWorkerStopped() override;
protected slots:
    void tunnelManagerStart();
    void tunnelManagerStop();
private:
    void startTunnelWithManager(NETunnelProviderManagerRef manager);
    void vpnConnectionStatusChanged(NSNotificationRef notification);
private:
#ifdef __cplusplus
    id tunnelProtocol;
    id tunnelManager;
#else
    DapPacketTunnelProvider* tunnelProvider;
    DapPacketTunnelProtocol* tunnelProtocol;
    NETunnelProviderManager* tunnelManager;
#endif
};

