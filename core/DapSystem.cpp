#include "DapSystem.h"

DapSystem::DapSystem()
{

}

QString DapSystem::dirAppFiles(const QString a_brand)
{
#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    return QString("/opt/%1").arg(a_brand).toLower();
#elif defined(Q_OS_MACOS)
    return QString("/tmp/%1").arg(a_brand);
#elif defined (Q_OS_WIN)
    return QString("%1/%2").arg(regWGetUsrPath()).arg(DAP_BRAND);
#elif defined Q_OS_ANDROID
    return QString("/sdcard/%1").arg(a_brand);
#endif
    return {};
}
