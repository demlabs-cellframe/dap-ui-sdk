#pragma once

#import <Foundation/Foundation.h>
#import <NetworkExtension/NEPacketTunnelProvider.h>

@interface DapPacketTunnelProvider : NEPacketTunnelProvider

- (void) startTunnel;
- (void)startTunnelWithOptions:(NSDictionary<NSString *,NSObject *> *)options
          completionHandler:(void (^)(NSError *error))completionHandler;
- (void)stopTunnelWithReason:(NEProviderStopReason)reason
           completionHandler:(void (^)(void))completionHandler;

- (void)cancelTunnelWithError:(NSError *)error;

@end

