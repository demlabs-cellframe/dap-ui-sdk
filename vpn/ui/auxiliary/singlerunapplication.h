#ifndef SINGLERUNAPPLICATION_H
#define SINGLERUNAPPLICATION_H

#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>

//#ifdef Q_OS_WIN
//#include "dap_common.h"
//#include "registry.h"
//bool checkSingleInstance()
//{
//  HANDLE hEvent = CreateEventA (nullptr, FALSE, FALSE, "Local\\" DAP_BRAND);
//  if (!hEvent)
//    {
//      CloseHandle (hEvent);
//      return false;
//    }
//  if (GetLastError() == ERROR_ALREADY_EXISTS)
//    {
//      CloseHandle (hEvent);
//      hEvent = nullptr;
//      return false;
//    }
//  return true;
//}
//#endif

//#ifdef Q_OS_WIN
//  if (!checkSingleInstance())
//    return 0;
//#endif

class SingleRunApplication
{
public:
    SingleRunApplication();

public:
    SingleRunApplication( const QString& key );
    ~SingleRunApplication();

    bool isLocked();
    bool lock();
    void release();

private:
    const QString key;
    const QString memoryLockKey;
    const QString sharedMemoryKey;

    QSharedMemory sharedMemory;
    QSystemSemaphore memoryLock;

    Q_DISABLE_COPY( SingleRunApplication )
};

#endif // SINGLERUNAPPLICATION_H
