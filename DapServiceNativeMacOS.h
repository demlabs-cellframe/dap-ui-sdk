#ifndef DAPSERVICENATIVEMACOS_H
#define DAPSERVICENATIVEMACOS_H

#ifdef Q_OS_MACOS
#include <Security/Authorization.h>
#include <Security/AuthorizationTags.h>
#endif

#include "DapServiceNativeAbstract.h"

class DapServiceNativeMacOS : public DapServiceNativeAbstract
{
public:
    DapServiceNativeMacOS();
    void checkInstallation() override;
    void restartService() override;
protected:
    void installService();
    void installTun();
    void installTunAndService();
    void deepTunCheck();
private:
    bool isNeedInstallOnlyService = false;
    bool isNeedInstallOnlyTun = false;
};

#endif // DAPSERVICENATIVEMACOS_H
