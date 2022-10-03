#ifndef SINGLERUNAPPLICATION_H
#define SINGLERUNAPPLICATION_H

#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>

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
