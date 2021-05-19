#import <NetworkExtension/NetworkExtension.h>

#import "DapPacketTunnelProvider.h"

@implementation DapPacketTunnelProvider
- (void)startTunnelWithOptions:(NSDictionary<NSString *,NSObject *> *)options
          completionHandler:(void (^)(NSError *error))completionHandler
{

}

- (void)stopTunnelWithReason:(NEProviderStopReason)reason
           completionHandler:(void (^)(void))completionHandler
{

}


- (void)cancelTunnelWithError:(NSError *)error
{

}

/**
 *
 */
+ (void) mainInit
{
 //
}

@end
