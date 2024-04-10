#include "DapPluginsPathControll.h"
#ifdef Q_OS_ANDROID

#include <DapAndroidHelpers.h>
#endif
#ifdef Q_OS_WIN
#include "registry.h"
#endif

DapPluginsPathControll::DapPluginsPathControll(QObject *parent) : QObject(parent)
{

}

QString DapPluginsPathControll::defaultPluginPath(const QString a_brand)
{
#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    return QString("/opt/%1/dapps").arg(a_brand).toLower();
#elif defined(Q_OS_MACOS)
    return QString("/tmp/cellframe-dashboard_dapps");
#elif defined (Q_OS_WIN)
    return QString("%1/%2/dapps").arg(regWGetUsrPath()).arg(DAP_BRAND);
#elif defined Q_OS_ANDROID
    static DapQtJniObject l_pathObj = dapQtAndroidServiceContext().callObjectMethod(
                "getExternalFilesDir"
                , "(Ljava/lang/String;)Ljava/io/File;"
                , DapQtJniObject::fromString(QString("")).object());
    return QString("%1/dapps").arg(l_pathObj.toString());
#endif
    return {};
}
