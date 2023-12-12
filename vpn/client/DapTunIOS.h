#ifndef DAPTUNIOS_H
#define DAPTUNIOS_H

#import <Foundation/Foundation.h>
#import "DapTunAbstract.h"

@class DapTunWorkerIOS;

@interface DapTunIOS : DapTunAbstract

@property (nonatomic, strong) NSThread *tunThread;

- (instancetype)init;
- (void)workerStart;
- (void)addNewUpstreamRoute:(NSString *)destination;
- (void)onWorkerStarted;
- (void)onWorkerStopped;

@end

#endif // DAPTUNIOS_H
