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
    // CRITICAL FIX: Do NOT call tunnelManagerStart() here - m_addr and m_gw are empty
    // tunnelManagerStart() will be called from tunDeviceCreate() after network config is set
    NSLog(@"DapTunDarwin::DapTunDarwin() - Constructor completed, waiting for network config");
}

void DapTunDarwin::tunnelManagerStart()
{
    NSLog(@"DapTunDarwin::tunnelManagerStart() - Starting NetworkExtension manager");
    qDebug() << "DapTunDarwin::tunnelManagerStart";

    // Create provider configuration with server details
    // Get network configuration from DapTunAbstract
    NSString *addr = [NSString stringWithUTF8String: m_addr.toLatin1().constData()];
    NSString *gw = [NSString stringWithUTF8String: m_gw.toLatin1().constData()];
    NSArray *dnsArray = @[@"1.1.1.1", @"8.8.8.8"];
    NSArray *includeRoutes = @[@"0.0.0.0/0"];  // Full tunnel
    NSArray *excludeRoutes = @[];
    NSNumber *mtuValue = @1380;
    
    NSDictionary *providerConfig = @{
        @"address": addr ?: @"",
        @"gateway": gw ?: @"",
        @"dns": dnsArray ?: @[],
        @"routesInclude": includeRoutes ?: @[],
        @"routesExclude": excludeRoutes ?: @[],
        @"mtu": mtuValue,
        @"server": [NSString stringWithUTF8String: upstreamAddress().toLatin1().constData()],
        @"routes": @{
            @"mode": @"full"  // KILL-SWITCH: Default to full tunnel for security
        }
    };
    
    NSLog(@"DapTunDarwin::tunnelManagerStart() - Provider config: %@", providerConfig);

    // Load existing managers first
    NSLog(@"DapTunDarwin::tunnelManagerStart() - Loading existing NetworkExtension managers...");
    [NETunnelProviderManager loadAllFromPreferencesWithCompletionHandler:^(NSArray<NETunnelProviderManager *> * _Nullable managers, NSError * _Nullable error) {
        if (error) {
            NSLog(@"CRITICAL ERROR: Failed to load NetworkExtension managers: %@", error.description);
            NSLog(@"Error domain: %@, code: %ld", error.domain, (long)error.code);
            NSLog(@"Error userInfo: %@", error.userInfo);
            return;
        }
        
        NSLog(@"DapTunDarwin::tunnelManagerStart() - Loaded %lu existing managers", (unsigned long)managers.count);
        
        // Log details about existing managers
        for (NETunnelProviderManager *existingManager in managers) {
            NSString *bundleId = @"unknown";
            if ([existingManager.protocolConfiguration isKindOfClass:[NETunnelProviderProtocol class]]) {
                NETunnelProviderProtocol *tunnelProtocol = (NETunnelProviderProtocol *)existingManager.protocolConfiguration;
                bundleId = tunnelProtocol.providerBundleIdentifier ?: @"nil";
            }
            NSLog(@"DapTunDarwin::tunnelManagerStart() - Found manager: %@, bundle: %@, enabled: %@", 
                  existingManager.localizedDescription,
                  bundleId,
                  existingManager.enabled ? @"YES" : @"NO");
        }

        NETunnelProviderManager *manager = nil;
        
        // Check if we already have a manager
        for (NETunnelProviderManager *existingManager in managers) {
            if ([existingManager.localizedDescription isEqualToString:@DAP_BRAND]) {
                manager = existingManager;
                NSLog(@"DapTunDarwin::tunnelManagerStart() - Found existing manager for %@", @DAP_BRAND);
                break;
            }
        }
        
        // Create new manager if none exists
        if (!manager) {
            NSLog(@"DapTunDarwin::tunnelManagerStart() - Creating new NetworkExtension manager");
            manager = [[NETunnelProviderManager alloc] init];
            
            NETunnelProviderProtocol *protocol = [[NETunnelProviderProtocol alloc] init];
            protocol.serverAddress = [NSString stringWithUTF8String: upstreamAddress().toLatin1().constData()];
            // Use the bundle identifier of the Packet Tunnel Provider extension
            protocol.providerBundleIdentifier = @"com.kelvpn.client.pktunnel";
            protocol.providerConfiguration = providerConfig;
            
            NSLog(@"DapTunDarwin::tunnelManagerStart() - Protocol configured: server=%@, bundle=%@", 
                  protocol.serverAddress, protocol.providerBundleIdentifier);
            
            manager.protocolConfiguration = protocol;
            manager.enabled = YES;
            manager.localizedDescription = @DAP_BRAND;
            
            tunnelProtocol = protocol;
            tunnelManager = manager;
            
            // Save the new manager
            [manager saveToPreferencesWithCompletionHandler:^(NSError *saveError) {
                if (saveError) {
                    NSLog(@"CRITICAL ERROR: Failed to save NetworkExtension manager: %@", saveError);
                    NSLog(@"Save error domain: %@, code: %ld", saveError.domain, (long)saveError.code);
                } else {
                    NSLog(@"DapTunDarwin::tunnelManagerStart() - Manager saved successfully, starting tunnel");
                    startTunnelWithManager((__bridge void*)manager);
                }
            }];
        } else {
            NSLog(@"DapTunDarwin::tunnelManagerStart() - Using existing NetworkExtension manager");
            // Update existing manager configuration
            NETunnelProviderProtocol *protocol = (NETunnelProviderProtocol *)manager.protocolConfiguration;
            protocol.providerConfiguration = providerConfig;
            protocol.serverAddress = [NSString stringWithUTF8String: upstreamAddress().toLatin1().constData()];
            
            NSLog(@"DapTunDarwin::tunnelManagerStart() - Updated existing protocol: server=%@, bundle=%@", 
                  protocol.serverAddress, protocol.providerBundleIdentifier);
            
            tunnelProtocol = protocol;
            tunnelManager = manager;
            
            [manager saveToPreferencesWithCompletionHandler:^(NSError *saveError) {
                if (saveError) {
                    NSLog(@"CRITICAL ERROR: Failed to update NetworkExtension manager: %@", saveError);
                    NSLog(@"Update error domain: %@, code: %ld", saveError.domain, (long)saveError.code);
                } else {
                    NSLog(@"DapTunDarwin::tunnelManagerStart() - Manager updated successfully, starting tunnel");
                    startTunnelWithManager((__bridge void*)manager);
                }
            }];
        }
    }];
}

void DapTunDarwin::startTunnelWithManager(NETunnelProviderManagerRef manager) {
    NETunnelProviderManager *neManager = (__bridge NETunnelProviderManager *)manager;
    NSLog(@"DapTunDarwin::startTunnelWithManager() - Starting tunnel with manager: %@", neManager.localizedDescription);
    
    // Set up connection status monitoring
    // Note: Notification observer setup simplified for C++ context
    // Status monitoring will be handled through NetworkExtension framework directly
    
    // Start the tunnel
    NSError *startError;
    [neManager.connection startVPNTunnelWithOptions:nil andReturnError:&startError];
    
    if (startError) {
        NSLog(@"CRITICAL ERROR: Failed to start VPN tunnel: %@", startError.localizedDescription);
        NSLog(@"Start error domain: %@, code: %ld", startError.domain, (long)startError.code);
    } else {
        NSLog(@"DapTunDarwin::startTunnelWithManager() - Tunnel start initiated successfully");
    }
}

void DapTunDarwin::tunDeviceCreate()
{
    NSLog(@"DapTunDarwin::tunDeviceCreate() - Creating NetworkExtension tunnel device");
    qDebug() << "DapTunDarwin::tunDeviceCreate";
    
    // Verify that network configuration is available
    if (m_addr.isEmpty() || m_gw.isEmpty()) {
        NSLog(@"CRITICAL ERROR: Network configuration not available - address: %s, gateway: %s", 
              m_addr.toLatin1().constData(), m_gw.toLatin1().constData());
        qCritical() << "DapTunDarwin::tunDeviceCreate() - Network configuration not available";
        return;
    }
    
    NSLog(@"DapTunDarwin::tunDeviceCreate() - Network config available - address: %s, gateway: %s", 
          m_addr.toLatin1().constData(), m_gw.toLatin1().constData());
    
    // Now call tunnelManagerStart() with proper network configuration
    tunnelManagerStart();
}

void DapTunDarwin::vpnConnectionStatusChanged(NSNotificationRef notification) {
    NSNotification *nsNotification = (__bridge NSNotification *)notification;
    NEVPNConnection *connection = nsNotification.object;
    NEVPNStatus status = connection.status;
    
    NSLog(@"VPN Status changed to: %ld", (long)status);
    
    switch (status) {
        case NEVPNStatusConnected:
            NSLog(@"VPN Connected");
            emit created();
            break;
        case NEVPNStatusDisconnected:
            NSLog(@"VPN Disconnected");
            emit destroyed();
            break;
        case NEVPNStatusConnecting:
            NSLog(@"VPN Connecting");
            // No specific signal for connecting state
            break;
        case NEVPNStatusDisconnecting:
            NSLog(@"VPN Disconnecting");
            // No specific signal for disconnecting state
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
    
    NETunnelProviderManager *manager = (NETunnelProviderManager *)tunnelManager;
    if (manager) {
        // Remove status monitoring
        // Note: Notification cleanup simplified for C++ context
        
        // Stop the tunnel
        [manager.connection stopVPNTunnel];
        NSLog(@"Tunnel stop initiated");
    }
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
