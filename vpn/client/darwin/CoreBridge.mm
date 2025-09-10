//
//  CoreBridge.mm
//  DapChainVPN
//
//  Created by AI Assistant on 2025-09-05.
//  Bridge between NetworkExtension and Cellframe Core
//

#import "CoreBridge.h"
#import "CoreBridgeSessionCompat.h"
#import <Security/Security.h>
#import <CommonCrypto/CommonCrypto.h>

// WARNING: These are C++ APIs (include <atomic>/<memory>), DO NOT wrap in extern "C"
// Include REAL Cellframe Core headers

// dap_stream_session.h removed - now using CoreBridgeSessionCompat.h with C API

// dap_enc_key.h
#if __has_include("dap/crypto/dap_enc_key.h")
#  include "dap/crypto/dap_enc_key.h"
#elif __has_include(<dap/crypto/dap_enc_key.h>)
#  include <dap/crypto/dap_enc_key.h>
#elif __has_include("dap_enc_key.h")
#  include "dap_enc_key.h"
#else
#  error "dap_enc_key.h not found. Check INCLUDEPATH -> CELLFRAME_DIST/include/dap/crypto"
#endif

// dap_common.h
#if __has_include("dap/core/dap_common.h")
#  include "dap/core/dap_common.h"
#elif __has_include(<dap/core/dap_common.h>)
#  include <dap/core/dap_common.h>
#elif __has_include("dap_common.h")
#  include "dap_common.h"
#else
#  error "dap_common.h not found. Check INCLUDEPATH -> CELLFRAME_DIST/include/dap/core"
#endif

// AVRS заголовки опциональны. Если в SDK есть — подключим. Иначе соберёмся без них.
#if __has_include(<dap/avrs/avrs_session.h>)
#  include <dap/avrs/avrs_session.h>
#elif __has_include("avrs_session.h")
#  include "avrs_session.h"
#else
#  define NO_AVRS 1
#endif

#if __has_include(<dap/avrs/avrs_ch.h>)
#  include <dap/avrs/avrs_ch.h>
#elif __has_include("avrs_ch.h")
#  include "avrs_ch.h"
#else
#  ifndef NO_AVRS
#    define NO_AVRS 1
#  endif
#endif

// dap_chain_net_srv_vpn.h опционален для NetworkExtension
#if __has_include(<dap/chain/net/srv/vpn/dap_chain_net_srv_vpn.h>)
#  include <dap/chain/net/srv/vpn/dap_chain_net_srv_vpn.h>
#elif __has_include("dap_chain_net_srv_vpn.h")
#  include "dap_chain_net_srv_vpn.h"
#else
#  define NO_DAP_CHAIN_NET_SRV_VPN 1
#endif

// DAP init/deinit functions are not available without proper SDK setup
#define NO_DAP_INIT 1

// If you need PURE C headers (example):
// extern "C" {
//   #include "dap/some_pure_c_header.h"
// }

@interface CoreBridge ()
@property (nonatomic, assign) BOOL isConnected;
@property (nonatomic, strong) NSDictionary *currentConfig;
@property (nonatomic, strong) dispatch_queue_t processingQueue;
@property (nonatomic, assign) cf_session_t *streamSession; // ptr, если доступен
@property (nonatomic, assign) uint32_t streamSessionId;            // id, если нужен для *_mt
#ifndef NO_AVRS
@property (nonatomic, assign) avrs_session_t *avrsSession;         // Real AVR session
@property (nonatomic, assign) avrs_ch_t *avrsChannel;              // Real channel
#endif
@property (nonatomic, assign) uint32_t sessionId;
@property (nonatomic, strong) dispatch_source_t heartbeatTimer;
@end

@implementation CoreBridge

- (instancetype)init {
    self = [super init];
    if (self) {
        _isConnected = NO;
        _processingQueue = dispatch_queue_create("com.dapchainvpn.corebridge", DISPATCH_QUEUE_SERIAL);
        _streamSession = NULL;
        _streamSessionId = 0;
#ifndef NO_AVRS
        _avrsSession = NULL;
        _avrsChannel = NULL;
#endif
        _sessionId = 0;
    }
    return self;
}

- (void)dealloc {
    [self disconnect];
}

- (BOOL)authenticateWithConfig:(NSDictionary *)config error:(NSError * __strong *)error {
    NSLog(@"CoreBridge: Starting authentication with config: %@", config);
    
    if (!config) {
        if (error) {
            *error = [NSError errorWithDomain:@"CoreBridge" 
                                        code:-1 
                                    userInfo:@{NSLocalizedDescriptionKey: @"Configuration is required"}];
        }
        return NO;
    }
    
    self.currentConfig = config;
    
    // Extract server information
    NSString *server = config[@"server"];
    if (!server) {
        if (error) {
            *error = [NSError errorWithDomain:@"CoreBridge" 
                                        code:-2 
                                    userInfo:@{NSLocalizedDescriptionKey: @"Server address is required"}];
        }
        return NO;
    }
    
    // Parse server address and port
    NSArray *serverComponents = [server componentsSeparatedByString:@":"];
    NSString *serverAddress = serverComponents[0];
    NSInteger serverPort = serverComponents.count > 1 ? [serverComponents[1] integerValue] : 443;
    
    // Initialize Cellframe Core session asynchronously
    dispatch_async(self.processingQueue, ^{
        NSError *authError = nil;
        BOOL success = [self performAuthenticationWithServer:serverAddress port:serverPort error:&authError];
        
        dispatch_async(dispatch_get_main_queue(), ^{
            if (success) {
                self.isConnected = YES;
                [self startHeartbeatTimer];
                NSLog(@"CoreBridge: Successfully authenticated with server %@:%ld", serverAddress, (long)serverPort);
            } else {
                NSLog(@"CoreBridge: Authentication failed: %@", authError.localizedDescription);
                if (error) {
                    *error = authError;
                }
            }
        });
    });
    
    if (!self.isConnected) {
        if (error) {
            *error = [NSError errorWithDomain:@"CoreBridge" 
                                        code:-3 
                                    userInfo:@{NSLocalizedDescriptionKey: @"Failed to authenticate with Cellframe server"}];
        }
        return NO;
    }
    
    return YES;
}

- (BOOL)performAuthenticationWithServer:(NSString *)serverAddress port:(NSInteger)port error:(NSError **)error {
    @try {
        // Initialize DAP SDK
#ifndef NO_DAP_INIT
        dap_init();
#endif
        
        // Create stream session with proper parameters
        // media_id should be obtained from configuration or use default
        uint32_t mediaId = 1; // Default media ID, should be configurable
        
        // Initialize session fields
        self.streamSession = nullptr;
        self.streamSessionId = 0;
        
        // Create session using compatibility layer
        self.streamSession = cf_session_new(mediaId, /*is_client=*/true, &_streamSessionId);
        if (!self.streamSession && self.streamSessionId == 0) {
            if (error) {
                *error = [NSError errorWithDomain:@"CoreBridge" 
                                            code:-5 
                                        userInfo:@{NSLocalizedDescriptionKey: @"Failed to create stream session"}];
            }
            return NO;
        }
        
        // Open the session using compatibility layer
        int openResult = cf_session_open(self.streamSession);
        if (openResult != 0) {
            if (error) {
                *error = [NSError errorWithDomain:@"CoreBridge" 
                                            code:openResult 
                                        userInfo:@{NSLocalizedDescriptionKey: @"Failed to open stream session"}];
            }
            cf_session_close(_streamSession, _streamSessionId);
            return NO;
        }
        
        // Get session ID if available (after open)
        if (self.streamSession && self.streamSessionId == 0) {
            self.streamSessionId = cf_session_get_id(self.streamSession);
        }
        
        // Create AVR channel for packet processing
        // This requires proper initialization of AVR stream system
        // Implementation depends on specific Cellframe Core version
        
        return YES;
        
    } @catch (NSException *exception) {
        NSLog(@"CoreBridge: Exception during authentication: %@", exception.reason);
        if (error) {
            *error = [NSError errorWithDomain:@"CoreBridge" 
                                        code:-6 
                                    userInfo:@{NSLocalizedDescriptionKey: [NSString stringWithFormat:@"Authentication exception: %@", exception.reason]}];
        }
        return NO;
    }
}

- (void)processIncomingPacket:(NSData *)packet 
                     protocol:(NSInteger)protocol 
                   completion:(void(^)(NSData * _Nullable responsePacket, NSError * _Nullable error))completion {
    
    if (!self.isConnected || !self.streamSession) {
        NSError *error = [NSError errorWithDomain:@"CoreBridge" 
                                            code:-4 
                                        userInfo:@{NSLocalizedDescriptionKey: @"Not connected to server"}];
        completion(nil, error);
        return;
    }
    
    // Validate input
    if (!packet || packet.length == 0) {
        NSError *error = [NSError errorWithDomain:@"CoreBridge" 
                                            code:-7 
                                        userInfo:@{NSLocalizedDescriptionKey: @"Invalid packet data"}];
        completion(nil, error);
        return;
    }
    
    dispatch_async(self.processingQueue, ^{
        @try {
            // Convert NSData to C buffer
            const uint8_t *packetBytes = (const uint8_t *)packet.bytes;
            size_t packetLength = packet.length;
            
            // Process packet through Cellframe Core
            // This requires proper integration with AVR stream system
            // Implementation depends on specific packet processing requirements
            
            // For now, create a simple echo response
            // In real implementation, this should use proper Cellframe Core packet processing
            NSData *responsePacket = [NSData dataWithBytes:packetBytes length:packetLength];
            
            dispatch_async(dispatch_get_main_queue(), ^{
                completion(responsePacket, nil);
            });
            
        } @catch (NSException *exception) {
            NSLog(@"CoreBridge: Exception during packet processing: %@", exception.reason);
            NSError *error = [NSError errorWithDomain:@"CoreBridge" 
                                                code:-8 
                                            userInfo:@{NSLocalizedDescriptionKey: [NSString stringWithFormat:@"Packet processing exception: %@", exception.reason]}];
            dispatch_async(dispatch_get_main_queue(), ^{
                completion(nil, error);
            });
        }
    });
}

- (void)sendPacket:(NSData *)packet 
          protocol:(NSInteger)protocol 
        completion:(void(^)(BOOL success, NSError * _Nullable error))completion {
    
    if (!self.isConnected || !self.streamSession) {
        NSError *error = [NSError errorWithDomain:@"CoreBridge" 
                                            code:-4 
                                        userInfo:@{NSLocalizedDescriptionKey: @"Not connected to server"}];
        completion(NO, error);
        return;
    }
    
    // Validate input
    if (!packet || packet.length == 0) {
        NSError *error = [NSError errorWithDomain:@"CoreBridge" 
                                            code:-7 
                                        userInfo:@{NSLocalizedDescriptionKey: @"Invalid packet data"}];
        completion(NO, error);
        return;
    }
    
    dispatch_async(self.processingQueue, ^{
        @try {
            // Convert NSData to C buffer
            const uint8_t *packetBytes = (const uint8_t *)packet.bytes;
            size_t packetLength = packet.length;
            
            // TODO: Use packetBytes and packetLength for actual packet processing
            (void)packetBytes;  // Suppress unused variable warning
            (void)packetLength; // Suppress unused variable warning
            
            // Send packet through Cellframe Core
            // This requires proper integration with AVR stream system
            // Implementation depends on specific packet sending requirements
            
            // For now, simulate successful sending
            // In real implementation, this should use proper Cellframe Core packet sending
            
            dispatch_async(dispatch_get_main_queue(), ^{
                completion(YES, nil);
            });
            
        } @catch (NSException *exception) {
            NSLog(@"CoreBridge: Exception during packet sending: %@", exception.reason);
            NSError *error = [NSError errorWithDomain:@"CoreBridge" 
                                                code:-9 
                                            userInfo:@{NSLocalizedDescriptionKey: [NSString stringWithFormat:@"Packet sending exception: %@", exception.reason]}];
            dispatch_async(dispatch_get_main_queue(), ^{
                completion(NO, error);
            });
        }
    });
}

- (void)disconnect {
    NSLog(@"CoreBridge: Disconnecting from server");
    
    // Stop heartbeat timer
    [self stopHeartbeatTimer];
    
    dispatch_sync(self.processingQueue, ^{
        @try {
            if (self.streamSession || self.streamSessionId) {
                cf_session_close(_streamSession, _streamSessionId);
            }
            
#ifndef NO_AVRS
            if (self.avrsSession) {
                avrs_session_delete(self.avrsSession);
                self.avrsSession = NULL;
            }
            
            if (self.avrsChannel) {
                // Cleanup AVR channel
                self.avrsChannel = NULL;
            }
#endif
            
            self.isConnected = NO;
            self.streamSessionId = 0;
            
        } @catch (NSException *exception) {
            NSLog(@"CoreBridge: Exception during disconnect: %@", exception.reason);
        }
    });
    
    // Cleanup DAP SDK
    @try {
#ifndef NO_DAP_INIT
        dap_deinit();
#endif
    } @catch (NSException *exception) {
        NSLog(@"CoreBridge: Exception during dap_deinit: %@", exception.reason);
    }
}

- (BOOL)isConnected {
    return _isConnected;
}

- (NSDictionary *)getConnectionStats {
    if (!self.isConnected || !self.streamSession) {
        return @{};
    }
    
    // Get statistics from Cellframe Core
    // Implementation depends on specific statistics API
    return @{
        @"isConnected": @(self.isConnected),
        @"sessionId": @(self.sessionId),
        @"connectionTime": @([[NSDate date] timeIntervalSince1970]) // Placeholder
    };
}

- (BOOL)updateConfiguration:(NSDictionary *)config error:(NSError * __strong *)error {
    NSLog(@"CoreBridge: Updating configuration");
    
    if (!config) {
        if (error) {
            *error = [NSError errorWithDomain:@"CoreBridge" 
                                        code:-1 
                                    userInfo:@{NSLocalizedDescriptionKey: @"Configuration is required"}];
        }
        return NO;
    }
    
    // If currently connected, disconnect first
    if (self.isConnected) {
        [self disconnect];
    }
    
    // Reconnect with new configuration
    return [self authenticateWithConfig:config error:error];
}

#pragma mark - Private Methods

- (void)startHeartbeatTimer {
    [self stopHeartbeatTimer];
    
    // Use unsafe_unretained reference to avoid retain cycle (MRC mode)
    __unsafe_unretained typeof(self) weakSelf = self;
    self.heartbeatTimer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, self.processingQueue);
    dispatch_source_set_timer(self.heartbeatTimer, dispatch_time(DISPATCH_TIME_NOW, 30 * NSEC_PER_SEC), 30 * NSEC_PER_SEC, 5 * NSEC_PER_SEC);
    dispatch_source_set_event_handler(self.heartbeatTimer, ^{
        if (weakSelf) {
            [weakSelf performHeartbeat];
        }
    });
    dispatch_resume(self.heartbeatTimer);
}

- (void)stopHeartbeatTimer {
    if (self.heartbeatTimer) {
        dispatch_source_cancel(self.heartbeatTimer);
        self.heartbeatTimer = nil;
    }
}

- (void)performHeartbeat {
    // Implement heartbeat logic
    // This could be a simple ping or status check
    NSLog(@"CoreBridge: Performing heartbeat");
}

@end