#include <QtDebug>

#include <DapAndroidHelpers.h>



#include "DapServiceNativeAndroid.h"
#include <QtCore/private/qandroidextras_p.h>

#include <QTimer>

DapServiceNativeAndroid::DapServiceNativeAndroid()
{

}

void DapServiceNativeAndroid::restartService()
{
    stopService();
    QTimer::singleShot(500, [ = ] {
        checkInstallation();
    });
}

void DapServiceNativeAndroid::stopService()
{
    dapQtAndroidServiceContext().callMethod<void> ("stopVPNService", "()V");
}

void DapServiceNativeAndroid::checkInstallation()
{
    if (!dapQtAndroidServiceContext().isValid()) {
        QAndroidIntent serviceIntent(dapQtAndroidServiceContext().object(),
                                     "com/" DAP_BRAND "/" DAP_BRAND "Service");

        QString method_name = "startService";
        
        if (dapQtAndroidSdkVersion() < 26)
        {
            method_name = "startService";
        }

        dapQtAndroidServiceContext().callObjectMethod(
                    method_name.toStdString().c_str(),
                    "(Landroid/content/Intent;)Landroid/content/ComponentName;",
                    serviceIntent.handle().object<jobject>());
    }
}
