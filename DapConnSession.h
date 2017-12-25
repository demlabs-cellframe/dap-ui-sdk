#ifndef DAPCONNSESSION_H
#define DAPCONNSESSION_H

#include "DapConnectBase.h"

class DapConnSession : public DapConnectBase
{
    Q_OBJECT

public:
    DapConnSession(QObject *parent = 0);
    ~DapConnSession();

};
#endif // DAPCONNSESSION_H
