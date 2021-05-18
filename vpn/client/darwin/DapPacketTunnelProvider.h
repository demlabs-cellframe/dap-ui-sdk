#pragma once
#import <Foundation/Foundation.h>
#import <NetworkExtension/NEPacketTunnelProvider.h>


NS_ASSUME_NONNULL_BEGIN
@interface DapPacketTunnelProvider : NEPacketTunnelProvider

- (void)startTunnelWithOptions:(NSDictionary<NSString *,NSObject *> *)options
          completionHandler:(void (^)(NSError *error))completionHandler;
- (void)stopTunnelWithReason:(NEProviderStopReason)reason
           completionHandler:(void (^)(void))completionHandler;

- (void)cancelTunnelWithError:(NSError *)error;

@end

NS_ASSUME_NONNULL_END
