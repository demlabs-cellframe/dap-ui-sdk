#pragma once

#include <QtGlobal>

#ifdef Q_OS_ANDROID


#include <QCoreApplication>
#include <QJniObject>
using DapQtJniObject = QJniObject;

inline DapQtJniObject dapQtAndroidActivityContext()
{
    return QJniObject(QCoreApplication::instance()->nativeInterface<QNativeInterface::QAndroidApplication>()->context());
}

inline DapQtJniObject dapQtAndroidServiceContext()
{
    return QJniObject(QCoreApplication::instance()->nativeInterface<QNativeInterface::QAndroidApplication>()->context());
}

inline int dapQtAndroidSdkVersion()
{
        return QCoreApplication::instance()
->nativeInterface<QNativeInterface::QAndroidApplication>()
             ->sdkVersion();
 
}

inline void dapQtAndroidRunOnMainThread(const const std::function<void()> &runnable)
{
        QCoreApplication::instance()->nativeInterface<QNativeInterface::QAndroidApplication>()
->runOnAndroidMainThread([runnable]() {runnable(); return QVariant();});
 
}



#endif // ANDROID