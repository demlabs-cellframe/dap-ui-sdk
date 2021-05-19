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

#define __NE_INDIRECT__
#import "darwin/DapPacketTunnelProvider.h"
#import <NetworkExtension/NETunnelProviderManager.h>


/**
 * @brief DapTunDarwin::DapTunDarwin
 */
DapTunDarwin::DapTunDarwin( )
{
    tunWorker = new DapTunWorkerDarwin(this);

    initWorker();
}

void DapTunDarwin::refreshManager()
{
    //[NETunnelProviderManager loadAllFromPreferences completionHandler:{}];
    /*NETunnelProviderManager.loadAllFromPreferences(   completionHandler: { (managers, error) in
                if nil == error {
                    if let managers = managers {
                        for manager in managers {
                            if manager.localizedDescription == VPNConnect.vpnDescription {
                                self.manager = manager
                                return
                            }
                        }
                    }
                }
                self.setPreferences()
            })*/
}

/**
 * @brief DapTunDarwin::requestTunDeviceCreate
 */
void DapTunDarwin::tunDeviceCreate()
{
}

/**
 * @brief DapTunDarwin::onWorkerStarted
 */
void DapTunDarwin::onWorkerStarted()
{
    __block NETunnelProviderManager *manager = [[NETunnelProviderManager alloc] init];

    tunnelManager = manager;

    DapPacketTunnelProvider *protocol = [[DapPacketTunnelProvider alloc] init];
//    manager.protocolConfiguration = protocol;
    manager.enabled = true;

    [NETunnelProviderManager loadAllFromPreferencesWithCompletionHandler:^(NSArray<NETunnelProviderManager *> * _Nullable managers, NSError * _Nullable error) {
        if ([managers count] > 0) {
            manager = [managers objectAtIndex:0];
            [manager start];
        } else {
            [manager saveToPreferencesWithCompletionHandler:^(NSError *error) {
                if (error) {
                    NSLog(@"Error 1: %@", error.description);
                } else {
                    [manager loadFromPreferencesWithCompletionHandler:^(NSError * _Nullable error) {
                        if (error) {
                            NSLog(@"Error 2: %@", error.description);
                        } else {
                            [manager start];
                        }
                    }];
                }
            }];
        }
    }];

}

/**
 * @brief DapTunDarwin::onWorkerStopped
 */
void DapTunDarwin::onWorkerStopped()
{
    qInfo() <<"Restore network configuration";
}
