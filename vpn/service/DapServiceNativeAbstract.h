#ifndef DAPSERVICENATIVEABSTRACT_H
#define DAPSERVICENATIVEABSTRACT_H

#include <QString>

class DapServiceNativeAbstract
{
public:
    DapServiceNativeAbstract();
    virtual void checkInstallation() = 0;
    virtual void restartService() = 0;
    virtual void init();
};

#endif // DAPSERVICENATIVEABSTRACT_H
