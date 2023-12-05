#pragma once

#include <QtGlobal>

#ifdef Q_OS_ANDROID

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

#include <QtAndroid>
#include <QAndroidJniObject>
using DapQtJniObject = QAndroidJniObject;
#else

#include <QCoreApplication>
#include <QJniObject>
using DapQtJniObject = QJniObject;
#endif


inline DapQtJniObject dapQtAndroidServiceContext()
{
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

     return QtAndroid::androidService();
    
    #else
        return QJniObject(QCoreApplication::instance()->nativeInterface<QNativeInterface::QAndroidApplication>()->context());
    #endif
}

inline int dapQtAndroidSdkVersion()
{

    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
         return QtAndroid::androidSdkVersion();
    #else
         return QCoreApplication::instance()
->nativeInterface<QNativeInterface::QAndroidApplication>()
             ->sdkVersion();
    #endif
}

inline void dapQtAndroidRunOnMainThread(const const std::function<void()> &runnable)
{
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QtAndroid::runOnAndroidThread(runnable);
    #else
        QCoreApplication::instance()->nativeInterface<QNativeInterface::QAndroidApplication>()
->runOnAndroidMainThread([runnable]() {runnable(); return QVariant();});
    #endif
}



#endif // ANDROID