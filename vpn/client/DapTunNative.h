#ifndef DAPTUNNATIVE_H
#define DAPTUNNATIVE_H

#include <QtGlobal>

#if defined(Q_OS_WIN)
#include "DapTunWindows.h"
typedef class DapTunWindows DapTunNative;
#elif defined(Q_OS_ANDROID)
#include "DapTunAndroid.h"
typedef class DapTunAndroid DapTunNative;
#elif defined (Q_OS_MACOS)
#include "daptun/macos/DapUtun.h"
typedef class DapUtun DapTunNative;
#elif defined(Q_OS_LINUX)
#include "DapTunLinux.h"
typedef class DapTunLinux DapTunNative;
#elif defined(Q_OS_IOS)
#include "daptun/ios/DaptunIOS.h"
typedef class DapUtun DapTunNative;
//typedef class DapTunIOS DapTunNative;
#endif

#endif // DAPTUNNATIVE_H
