#include "DapServiceNativeWindows.h"

DapServiceNativeWindows::DapServiceNativeWindows() {

}

void DapServiceNativeWindows::checkInstallation() {

}

/**
 * @brief DapServiceNativeWindows::restartService
 */
void DapServiceNativeWindows::restartService() {
/*
    ::system("Sc.exe stop \"" SAP_SERVICE_NAME "\"");
    ::system("Sc.exe start \"" SAP_SERVICE_NAME "\"");

    not enough: must unroll network changes aswell, if the ones occured
*/
}

