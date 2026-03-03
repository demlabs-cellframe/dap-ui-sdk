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
        return QJniObject(QNativeInterface::QAndroidApplication::context());
    }

    inline QJniObject androidActivity()
    {
        return QJniObject(QNativeInterface::QAndroidApplication::context());
    }

    inline int androidSdkVersion()
    {
        return QNativeInterface::QAndroidApplication::sdkVersion();
    }
}

#else // Qt5

#include <QtAndroid>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QAndroidIntent>

#endif // QT_VERSION

#endif // Q_OS_ANDROID
