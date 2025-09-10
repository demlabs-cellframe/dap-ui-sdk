//
//  CoreBridge.h
//  DapChainVPN
//
//  Created by AI Assistant on 2025-09-05.
//  Bridge between NetworkExtension and Cellframe Core
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * CoreBridge provides an Objective-C interface to the Cellframe Core C++ library
 * for NetworkExtension integration. It handles authentication, packet processing,
 * and maintains the connection to Cellframe servers.
 */
@interface CoreBridge : NSObject

/**
 * Initialize the CoreBridge with default settings
 */
- (instancetype)init;

/**
 * Authenticate with Cellframe server using the provided configuration
 * @param config Configuration dictionary containing server details and credentials
 * @param error Error object to be populated if authentication fails
 * @return YES if authentication succeeds, NO otherwise
 */
- (BOOL)authenticateWithConfig:(NSDictionary *)config error:(NSError * __strong *)error;

/**
 * Process incoming packet from the tunnel
 * @param packet Raw packet data
 * @param protocol Protocol type (AF_INET, AF_INET6, etc.)
 * @param completion Completion handler called with processed packet or error
 */
- (void)processIncomingPacket:(NSData *)packet 
                     protocol:(NSInteger)protocol 
                   completion:(void(^)(NSData * _Nullable responsePacket, NSError * _Nullable error))completion;

/**
 * Send packet to the tunnel
 * @param packet Packet data to send
 * @param protocol Protocol type
 * @param completion Completion handler called with success status or error
 */
- (void)sendPacket:(NSData *)packet 
          protocol:(NSInteger)protocol 
        completion:(void(^)(BOOL success, NSError * _Nullable error))completion;

/**
 * Disconnect from Cellframe server and cleanup resources
 */
- (void)disconnect;

/**
 * Get current connection status
 * @return YES if connected, NO otherwise
 */
- (BOOL)isConnected;

/**
 * Get connection statistics
 * @return Dictionary containing bytes sent/received, connection time, etc.
 */
- (NSDictionary *)getConnectionStats;

/**
 * Update tunnel configuration (for reconnection scenarios)
 * @param config New configuration dictionary
 * @param error Error object to be populated if update fails
 * @return YES if update succeeds, NO otherwise
 */
- (BOOL)updateConfiguration:(NSDictionary *)config error:(NSError * __strong *)error;

@end

NS_ASSUME_NONNULL_END