#ifndef INTERFACEDAPKEY_H
#define INTERFACEDAPKEY_H

#include <QByteArray>


class DapKey
{
public:
    DapKey() { }
    virtual ~DapKey() { }

    virtual void encode(QByteArray& in, QByteArray& out) = 0;
    virtual void decode(QByteArray& in, QByteArray& out) = 0;
    virtual bool init(const QString& key) = 0;
};

#endif
