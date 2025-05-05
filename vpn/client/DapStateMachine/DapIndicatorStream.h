#ifndef DAPINDICATORSTREAM_H
#define DAPINDICATORSTREAM_H

#include "DapIndicatorStateAbstract.h"

class DapIndicatorStream : public DapIndicatorStateAbstract {
    Q_OBJECT
public:
    explicit DapIndicatorStream(QObject *parent = nullptr): DapIndicatorStateAbstract(parent) { }
    void init(DapState *s, const QString& stateName) override;
protected:
    void initAllowedSubstatesTransitions() override;
public:
    DapState *upsDisconnected;
    DapState *upsHandshakeRequest;
    DapState *upsHandshakeReply;
    DapState *upsChannelsOpen;
    DapState *networkErr;
    DapState *srvProvided;
    DapState *configProvided;
    DapState *streamReconnecting;
};

#endif // DAPINDICATORSTREAM_H
