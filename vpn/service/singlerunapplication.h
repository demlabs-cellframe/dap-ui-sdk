#ifndef SINGLERUNAPPLICATION_H
#define SINGLERUNAPPLICATION_H

#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>

#if !QT_CONFIG(sharedmemory)
//  for android code build. 
//  Should not be used anyway, and will rise a compiller error in this case.

class QSharedMemoryStub
{
    public:
        QSharedMemoryStub(const QString &key);
        bool attach();
        void detach();
        bool create(int);
        bool isAttached();
};

using QSharedMemory = QSharedMemoryStub;


#endif 

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
