#import "DapTunIOS.h"
#import <Foundation/Foundation.h>

@class DapTunWorkerIOS;

@interface DapTunIOS ()

@property (nonatomic, strong) DapTunWorkerIOS *tunWorkerIOS;

@end

@implementation DapTunIOS

- (instancetype)init {
    self = [super init];
    if (self) {
        // Initialization code
    }
    return self;
}

- (void)workerStart {

}

- (void)addNewUpstreamRoute:(NSString *)destination {

}

- (void)onWorkerStarted {

}

- (void)onWorkerStopped {

}

@end
