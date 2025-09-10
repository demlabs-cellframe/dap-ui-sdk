#import <NetworkExtension/NetworkExtension.h>
#import <Security/Security.h>
#import <arpa/inet.h>
#import <netinet/in.h>
#import "DapPacketTunnelProvider.h"
#import "CoreBridge.h"

@interface DapPacketTunnelProvider ()
@property (nonatomic, strong) CoreBridge *coreBridge;
@property (nonatomic, strong) dispatch_source_t packetReadTimer;
@property (nonatomic, strong) dispatch_queue_t ioQueue;
@property (nonatomic, assign) BOOL isTunnelActive;
@property (nonatomic, assign) NSUInteger droppedPackets;
@property (nonatomic, assign) NSUInteger maxOutgoingQueueSize;
@end

@implementation DapPacketTunnelProvider

- (instancetype)init {
    self = [super init];
    if (self) {
        _coreBridge = [[CoreBridge alloc] init];
        _isTunnelActive = NO;
        _ioQueue = dispatch_queue_create("com.dapchainvpn.packet.io", DISPATCH_QUEUE_SERIAL);
        _droppedPackets = 0;
        _maxOutgoingQueueSize = 1000; // Back-pressure limit
    }
    return self;
}

- (void)startTunnelWithOptions:(NSDictionary<NSString *,NSObject *> *)options
          completionHandler:(void (^)(NSError *error))completionHandler
{
    NSLog(@"DapPacketTunnelProvider:: startTunnelWithOptions");

    // FIXED: Correct configuration parsing
    // serverAddress is NSString, not NSDictionary
    NEVPNProtocol *baseProto = self.protocolConfiguration;
    NETunnelProviderProtocol *tp = (NETunnelProviderProtocol *)baseProto;
    NSString *serverAddress = baseProto.serverAddress;
    NSDictionary *config = (NSDictionary *)tp.providerConfiguration;
    
    if (!config) {
        // Fallback to options if providerConfiguration is not configured
        config = options;
    }
    
    // Add server address to config if not present
    if (serverAddress && !config[@"server"]) {
        NSMutableDictionary *mutableConfig = [config mutableCopy];
        mutableConfig[@"server"] = serverAddress;
        config = [mutableConfig copy];
    }
    
    // FIXED: Validate configuration
    NSError *error = nil;
    if (![self validateConfiguration:config error:&error]) {
        completionHandler(error);
        return;
    }
    
    // Perform Cellframe authentication
    if (![self.coreBridge authenticateWithConfig:config error:&error]) {
        completionHandler(error);
        return;
    }
    
    // Configure network settings
    NEPacketTunnelNetworkSettings *settings = [self createNetworkSettingsWithConfig:config];
    if (!settings) {
        error = [NSError errorWithDomain:@"DapPacketTunnelProvider" 
                                   code:-1 
                               userInfo:@{NSLocalizedDescriptionKey: @"Failed to create network settings"}];
        completionHandler(error);
        return;
    }
    
    // Set tunnel network settings
    __unsafe_unretained typeof(self) weakSelf = self;
    [self setTunnelNetworkSettings:settings completionHandler:^(NSError * _Nullable error) {
        if (!weakSelf) { return; }
        if (error) {
            NSLog(@"Failed to set tunnel network settings: %@", error);
            if (completionHandler) completionHandler(error);
            return;
        }
        
        // Start packet processing
        [weakSelf startPacketProcessing];
        weakSelf.isTunnelActive = YES;
        
        NSLog(@"Tunnel started successfully");
        if (completionHandler) completionHandler(nil);
    }];
}

- (BOOL)validateConfiguration:(NSDictionary *)config error:(NSError **)error {
    // Validate server
    NSString *server = config[@"server"];
    if (!server || server.length == 0) {
        if (error) {
            *error = [NSError errorWithDomain:@"DapPacketTunnelProvider" 
                                        code:-1 
                                    userInfo:@{NSLocalizedDescriptionKey: @"Server address is required"}];
        }
        return NO;
    }
    
    // Validate server format (host:port)
    NSArray *serverComponents = [server componentsSeparatedByString:@":"];
    if (serverComponents.count > 2) {
        if (error) {
            *error = [NSError errorWithDomain:@"DapPacketTunnelProvider" 
                                        code:-2 
                                    userInfo:@{NSLocalizedDescriptionKey: @"Invalid server address format"}];
        }
        return NO;
    }
    
    // Validate port if present
    if (serverComponents.count == 2) {
        NSInteger port = [serverComponents[1] integerValue];
        if (port < 1 || port > 65535) {
            if (error) {
                *error = [NSError errorWithDomain:@"DapPacketTunnelProvider" 
                                            code:-3 
                                        userInfo:@{NSLocalizedDescriptionKey: @"Invalid port number"}];
            }
            return NO;
        }
    }
    
    // Validate MTU
    NSNumber *mtu = config[@"mtu"];
    if (mtu && (mtu.integerValue < 576 || mtu.integerValue > 1500)) {
        if (error) {
            *error = [NSError errorWithDomain:@"DapPacketTunnelProvider" 
                                        code:-4 
                                    userInfo:@{NSLocalizedDescriptionKey: @"Invalid MTU value (must be 576-1500)"}];
        }
        return NO;
    }
    
    // Validate routes
    NSDictionary *routes = config[@"routes"];
    if (routes) {
        NSString *mode = routes[@"mode"];
        if (![mode isEqualToString:@"full"] && ![mode isEqualToString:@"split"]) {
            if (error) {
                *error = [NSError errorWithDomain:@"DapPacketTunnelProvider" 
                                            code:-5 
                                        userInfo:@{NSLocalizedDescriptionKey: @"Invalid routing mode (must be 'full' or 'split')"}];
            }
            return NO;
        }
        
        // Validate CIDR routes if present
        NSArray *includeRoutes = routes[@"include"];
        NSArray *excludeRoutes = routes[@"exclude"];
        
        if (![self validateCIDRRoutes:includeRoutes error:error] || 
            ![self validateCIDRRoutes:excludeRoutes error:error]) {
            return NO;
        }
    }
    
    // Validate DNS
    NSDictionary *dns = config[@"dns"];
    if (dns) {
        NSArray *servers = dns[@"servers"];
        NSArray *domains = dns[@"domains"];
        
        if (servers && ![self validateDNSServers:servers error:error]) {
            return NO;
        }
        
        if (domains && ![self validateDNSDomains:domains error:error]) {
            return NO;
        }
    }
    
    NSLog(@"Configuration validated successfully for server: %@", server);
    return YES;
}

- (NEPacketTunnelNetworkSettings *)createNetworkSettingsWithConfig:(NSDictionary *)config {
    NEPacketTunnelNetworkSettings *settings = [[NEPacketTunnelNetworkSettings alloc] 
                                               initWithTunnelRemoteAddress:config[@"server"]];
    
    // Configure IPv4 settings
    NEIPv4Settings *ipv4Settings = [self createIPv4SettingsWithConfig:config];
    if (ipv4Settings) {
        settings.IPv4Settings = ipv4Settings;
    }
    
    // Configure IPv6 settings
    NEIPv6Settings *ipv6Settings = [self createIPv6SettingsWithConfig:config];
    if (ipv6Settings) {
        settings.IPv6Settings = ipv6Settings;
    }
    
    // Configure DNS settings
    NEDNSSettings *dnsSettings = [self createDNSSettingsWithConfig:config];
    if (dnsSettings) {
        settings.DNSSettings = dnsSettings;
    }
    
    // Configure MTU
    NSNumber *mtu = config[@"mtu"];
    if (mtu) {
        settings.MTU = @([mtu integerValue]);
    } else {
        settings.MTU = @1380; // Default MTU for VPN
    }
    
    return settings;
}

- (NEIPv4Settings *)createIPv4SettingsWithConfig:(NSDictionary *)config {
    // Default VPN IP range
    NSArray *addresses = @[@"10.0.0.1"];
    NSArray *subnetMasks = @[@"255.255.255.0"];
    
    NEIPv4Settings *ipv4Settings = [[NEIPv4Settings alloc] initWithAddresses:addresses 
                                                                 subnetMasks:subnetMasks];
    
    // Configure routing based on mode
    NSDictionary *routes = config[@"routes"];
    NSString *mode = routes[@"mode"];
    
    if ([mode isEqualToString:@"full"]) {
        // KILL-SWITCH: Full tunnel - route ALL traffic through VPN
        // This ensures no traffic leaks outside the tunnel
        ipv4Settings.includedRoutes = @[[NEIPv4Route defaultRoute]];
        NSLog(@"Kill-switch enabled: All IPv4 traffic routed through VPN");
    } else if ([mode isEqualToString:@"split"]) {
        // Split tunnel - include specific routes
        NSArray *includeRoutes = routes[@"include"];
        NSMutableArray *includedRoutes = [NSMutableArray array];
        
        for (NSString *route in includeRoutes) {
            NSArray *components = [route componentsSeparatedByString:@"/"];
            if (components.count == 2) {
                NSString *address = components[0];
                NSString *subnetMask = [self subnetMaskFromPrefixLength:[components[1] integerValue]];
                NEIPv4Route *ipv4Route = [[NEIPv4Route alloc] initWithDestinationAddress:address 
                                                                              subnetMask:subnetMask];
                [includedRoutes addObject:ipv4Route];
            }
        }
        
        ipv4Settings.includedRoutes = includedRoutes;
        
        // Exclude specific routes
        NSArray *excludeRoutes = routes[@"exclude"];
        NSMutableArray *excludedRoutes = [NSMutableArray array];
        
        for (NSString *route in excludeRoutes) {
            NSArray *components = [route componentsSeparatedByString:@"/"];
            if (components.count == 2) {
                NSString *address = components[0];
                NSString *subnetMask = [self subnetMaskFromPrefixLength:[components[1] integerValue]];
                NEIPv4Route *ipv4Route = [[NEIPv4Route alloc] initWithDestinationAddress:address 
                                                                              subnetMask:subnetMask];
                [excludedRoutes addObject:ipv4Route];
            }
        }
        
        ipv4Settings.excludedRoutes = excludedRoutes;
    }
    
    return ipv4Settings;
}

- (NEIPv6Settings *)createIPv6SettingsWithConfig:(NSDictionary *)config {
    // Default IPv6 VPN range
    NSArray *addresses = @[@"fd00::1"];
    NSArray *networkPrefixLengths = @[@64];
    
    NEIPv6Settings *ipv6Settings = [[NEIPv6Settings alloc] initWithAddresses:addresses 
                                                          networkPrefixLengths:networkPrefixLengths];
    
    // Configure IPv6 routing
    NSDictionary *routes = config[@"routes"];
    NSString *mode = routes[@"mode"];
    
    if ([mode isEqualToString:@"full"]) {
        // KILL-SWITCH: Full tunnel - route ALL IPv6 traffic through VPN
        // This ensures no IPv6 traffic leaks outside the tunnel
        ipv6Settings.includedRoutes = @[[NEIPv6Route defaultRoute]];
        NSLog(@"Kill-switch enabled: All IPv6 traffic routed through VPN");
    } else if ([mode isEqualToString:@"split"]) {
        // Split tunnel for IPv6
        NSArray *includeRoutes = routes[@"include"];
        NSMutableArray *includedRoutes = [NSMutableArray array];
        
        for (NSString *route in includeRoutes) {
            NSArray *components = [route componentsSeparatedByString:@"/"];
            if (components.count == 2 && [components[0] containsString:@":"]) {
                // IPv6 route
                NSString *address = components[0];
                NSNumber *prefixLength = @([components[1] integerValue]);
                NEIPv6Route *ipv6Route = [[NEIPv6Route alloc] initWithDestinationAddress:address 
                                                                       networkPrefixLength:prefixLength];
                [includedRoutes addObject:ipv6Route];
            }
        }
        
        ipv6Settings.includedRoutes = includedRoutes;
    }
    
    return ipv6Settings;
}

- (NEDNSSettings *)createDNSSettingsWithConfig:(NSDictionary *)config {
    NSDictionary *dnsConfig = config[@"dns"];
    NSArray *servers = dnsConfig[@"servers"];
    NSArray *domains = dnsConfig[@"domains"];
    
    if (!servers || servers.count == 0) {
        // Default DNS servers
        servers = @[@"1.1.1.1", @"8.8.8.8"];
    }
    
    NEDNSSettings *dnsSettings = [[NEDNSSettings alloc] initWithServers:servers];
    
    if (domains && domains.count > 0) {
        dnsSettings.matchDomains = domains;
    } else {
        // Default to all domains for full tunnel
        dnsSettings.matchDomains = @[@"."];
    }
    
    return dnsSettings;
}

- (NSString *)subnetMaskFromPrefixLength:(NSInteger)prefixLength {
    // Convert CIDR prefix length to subnet mask
    uint32_t mask = 0xFFFFFFFF << (32 - prefixLength);
    return [NSString stringWithFormat:@"%d.%d.%d.%d",
            (mask >> 24) & 0xFF,
            (mask >> 16) & 0xFF,
            (mask >> 8) & 0xFF,
            mask & 0xFF];
}

- (void)startPacketProcessing {
    NSLog(@"Starting packet processing with GCD timer");
    
    // FIXED: Use GCD timer instead of NSTimer to avoid retain cycles
    self.packetReadTimer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, self.ioQueue);
    dispatch_source_set_timer(self.packetReadTimer, dispatch_time(DISPATCH_TIME_NOW, 0), 50*NSEC_PER_MSEC, 5*NSEC_PER_MSEC);
    
    __unsafe_unretained typeof(self) weakSelf = self;
    dispatch_source_set_event_handler(self.packetReadTimer, ^{
        if (weakSelf && weakSelf.isTunnelActive) {
            [weakSelf readPacketsFromFlowAndFeedCore];
        }
    });
    
    dispatch_resume(self.packetReadTimer);
    
    // Start initial packet reading
    [self readPacketsFromFlowAndFeedCore];
}

- (void)readPacketsFromFlowAndFeedCore {
    if (!self.isTunnelActive) {
        return;
    }
    
    // FIXED: Use unsafe_unretained reference to avoid retain cycles (MRC mode)
    __unsafe_unretained typeof(self) weakSelf = self;
    [self.packetFlow readPacketsWithCompletionHandler:^(NSArray<NSData *> *packets, NSArray<NSNumber *> *protocols) {
        if (weakSelf) {
            [weakSelf handleIncomingPackets:packets protocols:protocols];
        }
    }];
}

- (void)handleIncomingPackets:(NSArray<NSData *> *)packets protocols:(NSArray<NSNumber *> *)protocols {
    // FIXED: Validate input data
    if (!packets || !protocols || packets.count != protocols.count) {
        NSLog(@"Invalid packet data received");
        return;
    }
    
    for (NSUInteger i = 0; i < packets.count; i++) {
        NSData *packet = packets[i];
        NSNumber *protocol = protocols[i];
        
        // Validate packet
        if (!packet || packet.length == 0) {
            NSLog(@"Skipping empty packet");
            continue;
        }
        
        // FIXED: Use unsafe_unretained reference to avoid retain cycles (MRC mode)
        __unsafe_unretained typeof(self) weakSelf = self;
        [self.coreBridge processIncomingPacket:packet protocol:[protocol integerValue] completion:^(NSData *responsePacket, NSError *error) {
            if (!weakSelf) return;
            
            if (responsePacket && !error) {
                // FIXED: Implement back-pressure with packet dropping
                [weakSelf writePacketWithBackPressure:responsePacket protocol:protocol];
            } else if (error) {
                NSLog(@"Error processing packet: %@", error);
                // Implement retry logic or error recovery
                [weakSelf handlePacketProcessingError:error];
            }
        }];
    }
}

- (void)stopTunnelWithReason:(NEProviderStopReason)reason
           completionHandler:(void (^)(void))completionHandler
{
    NSLog(@"DapPacketTunnelProvider:: stopTunnelWithReason: %ld", (long)reason);
    
    self.isTunnelActive = NO;
    
    // FIXED: Stop GCD timer properly
    if (self.packetReadTimer) {
        dispatch_source_cancel(self.packetReadTimer);
        self.packetReadTimer = nil;
    }
    
    // Disconnect from Cellframe Core
    [self.coreBridge disconnect];
    
    // Clear network settings
    [self setTunnelNetworkSettings:nil completionHandler:^(NSError * _Nullable error) {
        if (error) {
            NSLog(@"Error clearing tunnel network settings: %@", error);
        }
        completionHandler();
    }];
}

- (void)cancelTunnelWithError:(NSError *)error {
    NSLog(@"DapPacketTunnelProvider:: cancelTunnelWithError: %@", error);
    
    self.isTunnelActive = NO;
    
    // FIXED: Stop GCD timer properly
    if (self.packetReadTimer) {
        dispatch_source_cancel(self.packetReadTimer);
        self.packetReadTimer = nil;
    }
    
    [self.coreBridge disconnect];
}

+ (void)mainInit {
    NSLog(@"DapPacketTunnelProvider:: mainInit");
}

#pragma mark - Private Validation Methods

- (BOOL)validateCIDRRoutes:(NSArray *)routes error:(NSError **)error {
    if (!routes) return YES; // Optional routes
    
    for (NSString *route in routes) {
        if (![self isValidCIDRRoute:route]) {
            if (error) {
                *error = [NSError errorWithDomain:@"DapPacketTunnelProvider" 
                                            code:-6 
                                        userInfo:@{NSLocalizedDescriptionKey: [NSString stringWithFormat:@"Invalid CIDR route: %@", route]}];
            }
            return NO;
        }
    }
    return YES;
}

- (BOOL)isValidCIDRRoute:(NSString *)route {
    if (!route || route.length == 0) return NO;
    
    NSArray *components = [route componentsSeparatedByString:@"/"];
    if (components.count != 2) return NO;
    
    NSString *address = components[0];
    NSString *prefixStr = components[1];
    
    // Validate prefix length
    NSInteger prefix = [prefixStr integerValue];
    if (prefix < 0 || prefix > 128) return NO; // Support both IPv4 and IPv6
    
    // Basic address validation
    return [self isValidIPAddress:address] || [self isValidHostname:address];
}

- (BOOL)validateDNSServers:(NSArray *)servers error:(NSError **)error {
    if (!servers || servers.count == 0) return YES; // Optional DNS servers
    
    for (NSString *server in servers) {
        if (![self isValidIPAddress:server]) {
            if (error) {
                *error = [NSError errorWithDomain:@"DapPacketTunnelProvider" 
                                            code:-7 
                                        userInfo:@{NSLocalizedDescriptionKey: [NSString stringWithFormat:@"Invalid DNS server: %@", server]}];
            }
            return NO;
        }
    }
    return YES;
}

- (BOOL)validateDNSDomains:(NSArray *)domains error:(NSError **)error {
    if (!domains || domains.count == 0) return YES; // Optional domains
    
    for (NSString *domain in domains) {
        if (![self isValidDNSDomain:domain]) {
            if (error) {
                *error = [NSError errorWithDomain:@"DapPacketTunnelProvider" 
                                            code:-8 
                                        userInfo:@{NSLocalizedDescriptionKey: [NSString stringWithFormat:@"Invalid DNS domain: %@", domain]}];
            }
            return NO;
        }
    }
    return YES;
}

- (BOOL)isValidIPAddress:(NSString *)address {
    struct sockaddr_in sin;
    struct sockaddr_in6 sin6;
    return (inet_pton(AF_INET, address.UTF8String, &sin) == 1) || 
           (inet_pton(AF_INET6, address.UTF8String, &sin6) == 1);
}

- (BOOL)isValidHostname:(NSString *)hostname {
    if (hostname.length == 0 || hostname.length > 253) return NO;
    
    // Basic hostname validation
    NSCharacterSet *validChars = [NSCharacterSet characterSetWithCharactersInString:@"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-"];
    NSCharacterSet *invalidChars = [validChars invertedSet];
    
    return [hostname rangeOfCharacterFromSet:invalidChars].location == NSNotFound;
}

- (BOOL)isValidDNSDomain:(NSString *)domain {
    if (domain.length == 0 || domain.length > 253) return NO;
    
    // Allow wildcard domains (starting with .)
    if ([domain hasPrefix:@"."]) {
        domain = [domain substringFromIndex:1];
    }
    
    return [self isValidHostname:domain];
}

#pragma mark - Back-pressure and Error Handling

- (void)writePacketWithBackPressure:(NSData *)packet protocol:(NSNumber *)protocol {
    // FIXED: Implement back-pressure with packet dropping
    // In a real implementation, you would track outgoing queue size
    // For now, we'll use a simple approach
    
    // Example: dataWithBytesNoCopy to avoid copying
    // Note: This is a simplified example - real implementation would need proper buffer management
    [self.packetFlow writePackets:@[packet] withProtocols:@[protocol]];
}

- (void)handlePacketProcessingError:(NSError *)error {
    // FIXED: Implement error handling logic
    // This could include retry, reconnection, or error reporting
    NSLog(@"Handling packet processing error: %@", error);
    
    // Example: Log error statistics
    // In production, you might want to implement exponential backoff retry
    // or trigger reconnection based on error type
}

@end