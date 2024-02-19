#ifndef DAPSERVICENATIVEIOS_H
#define DAPSERVICENATIVEIOS_H


#include "DapServiceNativeAbstract.h"

class DapServiceNativeIOS : public DapServiceNativeAbstract
{
public:
    DapServiceNativeIOS();
    void checkInstallation() override;
    void restartService() override;
protected:
    void installService();
private:
    bool isNeedInstallService = false;
};

#endif // DAPSERVICENATIVEIOS_H
