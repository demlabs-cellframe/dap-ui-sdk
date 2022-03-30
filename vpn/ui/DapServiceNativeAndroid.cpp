#include <QtDebug>
#include <QAndroidJniObject>
#include <QAndroidIntent>
#include <QtAndroid>

#include "DapServiceNativeAndroid.h"

DapServiceNativeAndroid::DapServiceNativeAndroid()
{

}

void DapServiceNativeAndroid::restartService()
{
}

void DapServiceNativeAndroid::checkInstallation()
{
    if (!QtAndroid::androidService().isValid()) {
        QAndroidIntent serviceIntent(QtAndroid::androidActivity().object(),
                                     "com/" DAP_BRAND "/" DAP_BRAND "Service");
        QtAndroid::androidActivity().callObjectMethod(
                    "startForegroundService",
                    "(Landroid/content/Intent;)Landroid/content/ComponentName;",
                    serviceIntent.handle().object<jobject>());
    }
}
