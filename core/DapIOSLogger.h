#ifndef DAPIOSLOGGER_H
#define DAPIOSLOGGER_H

#include "dap_common.h"

static const char *s_log_level_tag[ 16 ] = {
    " [DBG] ", // L_DEBUG     = 0
    " [INF] ", // L_INFO      = 1,
    " [ * ] ", // L_NOTICE    = 2,
    " [MSG] ", // L_MESSAGE   = 3,
    " [DAP] ", // L_DAP       = 4,
    " [WRN] ", // L_WARNING   = 5,
    " [ATT] ", // L_ATT       = 6,
    " [ERR] ", // L_ERROR     = 7,
    " [ ! ] ", // L_CRITICAL  = 8,
    " [---] ", //             = 9
    " [---] ", //             = 10
    " [---] ", //             = 11
    " [---] ", //             = 12
    " [---] ", //             = 13
    " [---] ", //             = 14
    " [---] ", //             = 15
};

void redirectLogToDocuments();

void DapIOSLog(dap_log_level log_lvl, const char* msg);
// class DapIOSLogger
// {
// public:
    // void DapSetIOSLogFile(const QString& fileName);

// }; 

#endif //DAPIOSLOGGER_H