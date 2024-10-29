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
    qDebug() << "DapTunDarwin::DapTunDarwin( )";
    tunThread = new QThread();
    tunWorker = new DapTunWorkerDarwin(this);
    connect(this, &DapTunAbstract::sigStartWorkerLoop, tunWorker, &DapTunWorkerAbstract::loop);
    initWorker();
    tunnelManagerStart();
}

void DapTunDarwin::tunnelManagerStart()
{
    qDebug() << "DapTunDarwin::tunnelManagerStart";

    //NSLog (@"Call loadAllFromPreferencesWithCompletionHandler");
    __block NETunnelProviderManager *manager;

    manager = [[NETunnelProviderManager alloc] init];

    NETunnelProviderProtocol *protocol = [[NETunnelProviderProtocol alloc] init];
    [protocol.includeAllNetworks: YES];
   // [protocol.enforceRoutes: YES];
    [protocol.serverAddress: [NSString stringWithUTF8String: upstreamAddress().toLatin1().constData()]];
    [protocol.providerBundleIdentifier: [NSString stringWithUTF8String: "com.kelvpn" ]];

    [manager.protocolConfiguration: protocol];
    [manager.enabled: YES];
    [manager setLocalizedDescription:@DAP_BRAND];

    tunnelProtocol = protocol;
    tunnelManager = manager;

    NSLog(@"Connection desciption: %@", manager.localizedDescription);
    NSLog(@"VPN status:  %i", manager.connection.status);
    //[[NSNotificationCenter defaultCenter] addObserver:this selector:@selector(vpnConnectionStatusChanged) name:NEVPNStatusDidChangeNotification object:nil];

    [manager saveToPreferencesWithCompletionHandler:^(NSError *error) {
        if(error) {
           NSLog(@"Save error: %@", error);
        }else {
            NSLog(@"No error");
            [manager loadFromPreferencesWithCompletionHandler:^(NSError *error) {
                if(error) {
                    NSLog(@"Load error: %@", error);
                }else {
                    NEPacketTunnelNetworkSettings * settings = [[NEPacketTunnelNetworkSettings alloc] init];

                    NSError *startError;
                    [manager.connection startVPNTunnelAndReturnError:&startError];
                    if(startError) {
                        NSLog(@"Start error: %@", startError.localizedDescription);
                    }
                }
            }];
        }
    }];

/*    [NETunnelProviderManager loadAllFromPreferencesWithCompletionHandler:^(NSArray<NETunnelProviderManager *> * _Nullable managers, NSError * _Nullable error) {
        if (error) {
            NSLog(@"Load Error: %@", error.description);
            return;
        }else {
            NSLog (@"Loaded well");
        }

        if (managers.count > 0) {
            manager = managers[0];
        }else {
            [manager saveToPreferencesWithCompletionHandler:^(NSError *error) {
                if(error) {
                   NSLog(@"Save error: %@", error);
                }else {
                    NSLog(@"No error");
                    [manager loadFromPreferencesWithCompletionHandler:^(NSError *error) {
                        if(error) {
                            NSLog(@"Load error: %@", error);
                        }else {
                            NEPacketTunnelNetworkSettings * settings = [[NEPacketTunnelNetworkSettings alloc] init];

                            NSError *startError;
                            [manager.connection startVPNTunnelAndReturnError:&startError];
                            if(startError) {
                                NSLog(@"Start error: %@", startError.localizedDescription);
                            }
                        }
                    }];
                }
            }];
        }
    }];*/

    //__block DapPacketTunnelProvider *provider = [[DapPacketTunnelProvider alloc] init];
    //tunnelProvider = provider;
    //[provider startTunnel];
    @try{
        NSError *startError;
        NSDictionary *providerConfiguration = [protocol providerConfiguration];
        [manager.connection startTunnelWithOptions:&startError];
        if(startError) {
            NSLog(@"Start error: %@", startError.localizedDescription);
        }
    }@catch (NSException *e) {
        NSLog(@"Error %@",e);
    }


    qDebug() << "Tunnel manager start over";

}

/**
 * @brief DapTunDarwin::tunnelManagerStop
 */
void DapTunDarwin::tunnelManagerStop()
{
    qDebug() << "DapTunDarwin::tunnelManagerStop()";
    NETunnelProviderManager * manager = ((NETunnelProviderManager *) tunnelManager);
    [manager stop];
}


/**
 * @brief DapTunDarwin::requestTunDeviceCreate
 */
void DapTunDarwin::tunDeviceCreate()
{
    qDebug() << "DapTunDarwin::tunDeviceCreate()";

    tunnelManagerStart();
}

void DapTunDarwin::workerStart() {
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

/**
 * @brief DapTunDarwin::onWorkerStarted
 */
void DapTunDarwin::onWorkerStarted()
{
    qDebug() << "DapTunDarwin::OnWorkerStarted()";
}

/**
 * @brief DapTunDarwin::onWorkerStopped
 */
void DapTunDarwin::onWorkerStopped()
{
    qDebug() << "DapTunDarwin::OnWorkerStopped()";
}
