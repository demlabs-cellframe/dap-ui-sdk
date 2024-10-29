#ifndef DAPTUNNATIVE_H
#define DAPTUNNATIVE_H

#include <QtGlobal>

#if defined(Q_OS_WIN)
#include "DapTunWindows.h"
typedef class DapTunWindows DapTunNative;
#elif defined(Q_OS_ANDROID)
#include "DapTunAndroid.h"
typedef class DapTunAndroid DapTunNative;
#elif defined (Q_OS_DARWIN)
#include "darwin/DapUtun.h"
typedef class DapUtun DapTunNative;
#elif defined(Q_OS_LINUX)
#include "DapTunLinux.h"
typedef class DapTunLinux DapTunNative;
#endif

#endif // DAPTUNNATIVE_H
