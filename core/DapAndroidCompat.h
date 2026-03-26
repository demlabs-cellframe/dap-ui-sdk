#pragma once

/*
 * Qt5/Qt6 Android compatibility layer.
 * In Qt6, QtAndroid module (androidextras) was removed.
 * QAndroidJniObject -> QJniObject
 * QAndroidJniEnvironment -> QJniEnvironment
 * QtAndroid::androidContext() -> QNativeInterface::QAndroidApplication::context()
 */

#ifdef Q_OS_ANDROID

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

#include <QJniObject>
#include <QJniEnvironment>
#include <QCoreApplication>
#include <QtCore/private/qandroidextras_p.h>

using QAndroidJniObject = QJniObject;
using QAndroidJniEnvironment = QJniEnvironment;

namespace QtAndroid
{
    inline QJniObject androidContext()
    {
        return QJniObject(QNativeInterface::QAndroidApplication::context());
    }

    inline QJniObject androidService()
    {
        QJniObject ctx(QNativeInterface::QAndroidApplication::context());
        if (!ctx.isValid())
            return QJniObject();
        QJniEnvironment env;
        jclass serviceClass = env.findClass("android/app/Service");
        if (!serviceClass)
            return QJniObject();
        if (env->IsInstanceOf(ctx.object(), serviceClass))
            return ctx;
        return QJniObject();
    }

    inline QJniObject androidActivity()
    {
        return QJniObject(QNativeInterface::QAndroidApplication::context());
    }

    inline int androidSdkVersion()
    {
        return QNativeInterface::QAndroidApplication::sdkVersion();
    }

    template<typename Func>
    inline auto runOnAndroidThread(Func &&func)
    {
        return QNativeInterface::QAndroidApplication::runOnAndroidMainThread(std::forward<Func>(func));
    }
}

#else // Qt5

#include <QtAndroid>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QAndroidIntent>

#endif // QT_VERSION

#endif // Q_OS_ANDROID
