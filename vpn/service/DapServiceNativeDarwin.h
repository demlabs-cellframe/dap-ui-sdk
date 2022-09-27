#pragma once

#include <qglobal.h>
#ifdef Q_OS_MACOS
#include <Security/Authorization.h>
#include <Security/AuthorizationTags.h>
#endif

#include "DapServiceNativeAbstract.h"

class DapServiceNativeDarwin : public DapServiceNativeAbstract
{
public:
    DapServiceNativeDarwin();
    void checkInstallation() override;
    void restartService() override;
protected:
    void installService();
private:
    bool isNeedInstallService = false;
};

