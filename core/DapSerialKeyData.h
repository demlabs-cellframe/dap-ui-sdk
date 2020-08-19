#ifndef DAPSERIALKEYDATA_H
#define DAPSERIALKEYDATA_H

#include <QObject>

#include "DapServerInfo.h"

class DapSerialKeyData: public QObject
{
    Q_OBJECT
public:
    DapSerialKeyData(QObject* a_parent = nullptr);

    DapSerialKeyData(const DapSerialKeyData& another):QObject(){};

    QString serialKey() const;
    void setSerialKey(const QString &a_serialKey);

    bool isActivated() const;
    void setActivated(bool isActivated);

signals:
    void serialKeyChanged(const QString& serialKey);
    void activationChanged(bool activation);

private:
    QString m_serialKey;
    bool m_isActivated;
};

Q_DECLARE_METATYPE(DapSerialKeyData)

#endif // DAPSERIALKEYDATA_H
