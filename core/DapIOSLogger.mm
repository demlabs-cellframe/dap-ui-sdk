#import <Foundation/Foundation.h>
#import <os/log.h>
#include "DapIOSLogger.h"
#include "dap_common.h"

const char * dapIOSLogPath() {
    NSArray *allPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [allPaths objectAtIndex:0];
    NSString *pathForLog = [documentsDirectory stringByAppendingPathComponent:@"/log"];
    const char * path = [pathForLog UTF8String];
    return path;
}

