#include <QtDebug>

#include <QAndroidJniObject>

#include <QtAndroid>

#include "DapServiceNativeAndroid.h"

DapServiceNativeAndroid::DapServiceNativeAndroid()
{

}

void DapServiceNativeAndroid::restartService()
{

    /*qWarning() << "[DapServiceNativeAndroid] restartService()";
    // Calling Service
    QAndroidJniObject::callStaticMethod<void>("com/demlabs/dap/DapVpnService",
                                              "startService",
                                              "(Landroid/content/Context;)V",
                                              QtAndroid::androidActivity().object());*/

}

void DapServiceNativeAndroid::checkInstallation()
{
    if (!QtAndroid::androidService().isValid()) {
        qInfo() << "Starting Android service";
        QtAndroid::androidActivity().callMethod<void>( "startService", "()V" );
    }
}
