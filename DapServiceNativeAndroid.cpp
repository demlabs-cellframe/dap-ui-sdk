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
    qWarning() << "[DapServiceNativeAndroid] checkInstallation() not implemented";
    QtAndroid::androidActivity().callMethod<void>( "startService"
                                           ,"()V" );
    /*QAndroidJniObject::callStaticMethod<void>("com/demlabs/dapchain/DapChainVpnService",
                                              "startService",
                                              "(Landroid/content/Context;)V",
                                              QtAndroid::androidActivity().object());*/
}
