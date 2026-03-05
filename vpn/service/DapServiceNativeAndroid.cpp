#include <QtDebug>
#include "DapAndroidCompat.h"

#include "DapServiceNativeAndroid.h"
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
    QtAndroid::androidActivity().callMethod<void> ("stopVPNService", "()V");
}

void DapServiceNativeAndroid::checkInstallation()
{
    if (!QtAndroid::androidService().isValid()) {
        QAndroidIntent serviceIntent(QtAndroid::androidActivity().object(),
                                     "com/" DAP_BRAND "/" DAP_BRAND "Service");

        const char *method_name = (QtAndroid::androidSdkVersion() >= 26)
                                  ? "startForegroundService"
                                  : "startService";

        QtAndroid::androidActivity().callObjectMethod(
                    method_name,
                    "(Landroid/content/Intent;)Landroid/content/ComponentName;",
                    serviceIntent.handle().object<jobject>());
    }
}
