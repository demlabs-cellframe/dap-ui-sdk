#import <Foundation/Foundation.h>
#import <os/log.h>
#include "DapIOSLogger.h"
#include "dap_common.h"

void redirectLogToDocuments() {
     NSArray *allPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
     NSString *documentsDirectory = [allPaths objectAtIndex:0];
     NSString *pathForLog = [documentsDirectory stringByAppendingPathComponent:@"KelVNP.log"];

     freopen([pathForLog cStringUsingEncoding:NSASCIIStringEncoding],"a+",stderr);
}

void DapIOSLog(dap_log_level log_lvl, const char* msg) {
    NSString *outputData = [[NSString alloc] initWithUTF8String: msg ];
    NSString *nslog_lvl = [[NSString alloc] initWithUTF8String: s_log_level_tag[log_lvl]];
    NSLog(@"%@: %@",nslog_lvl, outputData);
    os_log(OS_LOG_DEFAULT, "%@: %@", nslog_lvl, outputData);
    [outputData release];
    [nslog_lvl release];
}


