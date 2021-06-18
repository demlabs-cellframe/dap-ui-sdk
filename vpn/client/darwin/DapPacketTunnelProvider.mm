#import <NetworkExtension/NetworkExtension.h>

#import "DapPacketTunnelProvider.h"

@implementation DapPacketTunnelProvider

- (void)startTunnelWithOptions:(NSDictionary<NSString *,NSObject *> *)options
          completionHandler:(void (^)(NSError *error))completionHandler
{
    NSLog(@"DapPacketTunnelProvider:: startTunnelWithOptions");

    //setTunnelNetworkSettings
}

- (void) startTunnel
{
    NSLog(@"DapPacketTunnelProvider:: startTunnel");
}

- (void)stopTunnelWithReason:(NEProviderStopReason)reason
           completionHandler:(void (^)(void))completionHandler
{
NSLog(@"DapPacketTunnelProvider:: stopTunnelWithReason");
}


- (void)cancelTunnelWithError:(NSError *)error
{

    NSLog(@"DapPacketTunnelProvider:: cancelTunnelWithError");
}

/**
 *
 */
+ (void) mainInit
{
    NSLog(@"DapPacketTunnelProvider:: mainInit");
}

@end
