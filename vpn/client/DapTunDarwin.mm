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

    // Create provider configuration with server details
    NSDictionary *providerConfig = @{
        @"server": [NSString stringWithUTF8String: upstreamAddress().toLatin1().constData()],
        @"mtu": @1380,
        @"routes": @{
            @"mode": @"full"  // KILL-SWITCH: Default to full tunnel for security
        },
        @"dns": @{
            @"servers": @[@"1.1.1.1", @"8.8.8.8"],
            @"domains": @[@"."]
        }
    };

    // Load existing managers first
    [NETunnelProviderManager loadAllFromPreferencesWithCompletionHandler:^(NSArray<NETunnelProviderManager *> * _Nullable managers, NSError * _Nullable error) {
        if (error) {
            NSLog(@"Load Error: %@", error.description);
            return;
        }

        NETunnelProviderManager *manager = nil;
        
        // Check if we already have a manager
        for (NETunnelProviderManager *existingManager in managers) {
            if ([existingManager.localizedDescription isEqualToString:@DAP_BRAND]) {
                manager = existingManager;
                break;
            }
        }
        
        // Create new manager if none exists
        if (!manager) {
            manager = [[NETunnelProviderManager alloc] init];
            
            NETunnelProviderProtocol *protocol = [[NETunnelProviderProtocol alloc] init];
            protocol.serverAddress = [NSString stringWithUTF8String: upstreamAddress().toLatin1().constData()];
            // FIXED: Use correct bundle identifier that matches the extension's Info.plist
            protocol.providerBundleIdentifier = [NSString stringWithUTF8String: "com.kelvpn.packettunnel"];
            protocol.providerConfiguration = providerConfig;
            
            manager.protocolConfiguration = protocol;
            manager.enabled = YES;
            manager.localizedDescription = @DAP_BRAND;
            
            tunnelProtocol = protocol;
            tunnelManager = manager;
            
            // Save the new manager
            [manager saveToPreferencesWithCompletionHandler:^(NSError *saveError) {
                if (saveError) {
                    NSLog(@"Save error: %@", saveError);
                } else {
                    NSLog(@"Manager saved successfully");
                    [self startTunnelWithManager:manager];
                }
            }];
        } else {
            // Update existing manager configuration
            NETunnelProviderProtocol *protocol = (NETunnelProviderProtocol *)manager.protocolConfiguration;
            protocol.providerConfiguration = providerConfig;
            protocol.serverAddress = [NSString stringWithUTF8String: upstreamAddress().toLatin1().constData()];
            
            tunnelProtocol = protocol;
            tunnelManager = manager;
            
            [manager saveToPreferencesWithCompletionHandler:^(NSError *saveError) {
                if (saveError) {
                    NSLog(@"Update error: %@", saveError);
                } else {
                    NSLog(@"Manager updated successfully");
                    [self startTunnelWithManager:manager];
                }
            }];
        }
    }];
}

- (void)startTunnelWithManager:(NETunnelProviderManager *)manager {
    NSLog(@"Starting tunnel with manager: %@", manager.localizedDescription);
    
    // Set up connection status monitoring
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(vpnConnectionStatusChanged:)
                                                 name:NEVPNStatusDidChangeNotification
                                               object:manager.connection];
    
    // Start the tunnel
    NSError *startError;
    [manager.connection startVPNTunnelWithOptions:nil andReturnError:&startError];
    
    if (startError) {
        NSLog(@"Start error: %@", startError.localizedDescription);
    } else {
        NSLog(@"Tunnel start initiated successfully");
    }
}

- (void)vpnConnectionStatusChanged:(NSNotification *)notification {
    NEVPNConnection *connection = notification.object;
    NEVPNStatus status = connection.status;
    
    NSLog(@"VPN Status changed to: %ld", (long)status);
    
    switch (status) {
        case NEVPNStatusConnected:
            NSLog(@"VPN Connected");
            emit connected();
            break;
        case NEVPNStatusDisconnected:
            NSLog(@"VPN Disconnected");
            emit disconnected();
            break;
        case NEVPNStatusConnecting:
            NSLog(@"VPN Connecting");
            emit connecting();
            break;
        case NEVPNStatusDisconnecting:
            NSLog(@"VPN Disconnecting");
            emit disconnecting();
            break;
        case NEVPNStatusInvalid:
            NSLog(@"VPN Invalid");
            emit error("Invalid VPN configuration");
            break;
        case NEVPNStatusReasserting:
            NSLog(@"VPN Reasserting");
            break;
    }
}

/**
 * @brief DapTunDarwin::tunnelManagerStop
 */
void DapTunDarwin::tunnelManagerStop()
{
    qDebug() << "DapTunDarwin::tunnelManagerStop()";
    
    NETunnelProviderManager *manager = (__bridge NETunnelProviderManager *)tunnelManager;
    if (manager) {
        // Remove status monitoring
        [[NSNotificationCenter defaultCenter] removeObserver:self
                                                        name:NEVPNStatusDidChangeNotification
                                                      object:manager.connection];
        
        // Stop the tunnel
        [manager.connection stopVPNTunnel];
        NSLog(@"Tunnel stop initiated");
    }
}


/**
 * @brief DapTunDarwin::tunDeviceCreate
 */
void DapTunDarwin::tunDeviceCreate()
{
    qDebug() << "DapTunDarwin::tunDeviceCreate() - Starting NetworkExtension tunnel";
    
    // NetworkExtension handles tunnel creation automatically
    tunnelManagerStart();
}

void DapTunDarwin::workerStart() {
    qInfo() << "DapTunDarwin::workerStart() - NetworkExtension handles packet processing";
    
    // NetworkExtension handles packet processing in PacketTunnelProvider
    // No need for separate worker thread
    onWorkerStarted();
}

/**
 * @brief DapTunDarwin::onWorkerStarted
 */
void DapTunDarwin::onWorkerStarted()
{
    qDebug() << "DapTunDarwin::onWorkerStarted() - NetworkExtension tunnel ready";
    emit created();
}

/**
 * @brief DapTunDarwin::onWorkerStopped
 */
void DapTunDarwin::onWorkerStopped()
{
    qDebug() << "DapTunDarwin::onWorkerStopped() - NetworkExtension tunnel stopped";
    emit destroyed();
}
