#include "singlerunapplication.h"
#include <QCryptographicHash>
#include <QSharedMemory>
QString generateKeyHash( const QString& key, const QString& salt )
{
    QByteArray data;
    data.append( key.toUtf8() );
    data.append( salt.toUtf8() );
    data = QCryptographicHash::hash( data, QCryptographicHash::Sha1 ).toHex();
    return data;
}


SingleRunApplication::SingleRunApplication( const QString& key )
    : key( key )
    , memoryLockKey( generateKeyHash( key, "memLockKey") )
    , sharedMemoryKey( generateKeyHash( key, "sharedmemKey") )
    , sharedMemory( sharedMemoryKey )
    , memoryLock( memoryLockKey, 1 )
{
    memoryLock.acquire();
    {
        QSharedMemory a_sharedMemory( sharedMemoryKey );
        a_sharedMemory.attach();
    }
    memoryLock.release();
}

SingleRunApplication::~SingleRunApplication()
{
    release();
}

bool SingleRunApplication::isLocked()
{
    if ( sharedMemory.isAttached() )
        return false;

    memoryLock.acquire();
    const bool isAttach = sharedMemory.attach();
    if ( isAttach )
        sharedMemory.detach();
    memoryLock.release();

    return isAttach;
}

bool SingleRunApplication::lock()
{
    if ( isLocked() )
        return false;

    memoryLock.acquire();
    const bool result = sharedMemory.create( sizeof( quint64 ) );
    memoryLock.release();
    if ( !result )
    {
        release();
        return false;
    }
    return true;
}

void SingleRunApplication::release()
{
    memoryLock.acquire();
    if ( sharedMemory.isAttached() )
        sharedMemory.detach();
    memoryLock.release();
}
