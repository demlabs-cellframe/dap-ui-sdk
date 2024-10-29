#include <QtDebug>
#include <QAndroidJniObject>
#include <QAndroidIntent>
#include <QtAndroid>

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

        QString method_name = "startService";
        
        if (QtAndroid::androidSdkVersion() < 26)
        {
            method_name = "startService";
        }

        QtAndroid::androidActivity().callObjectMethod(
                    method_name.toStdString().c_str(),
                    "(Landroid/content/Intent;)Landroid/content/ComponentName;",
                    serviceIntent.handle().object<jobject>());
    }
}
