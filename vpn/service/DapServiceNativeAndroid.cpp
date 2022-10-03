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

        QString method_name = "startForegrouService";
        
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
