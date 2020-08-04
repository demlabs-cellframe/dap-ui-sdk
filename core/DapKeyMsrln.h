#ifndef DAPKEYMSRLN_H
#define DAPKEYMSRLN_H

#include "InterfaceDapKey.h"
#include "rand/dap_rand.h"
#include "dap_enc_key.h"
// #include "liboqs/kex/kex.h"
#include "DapKeyAes.h"


class DapKeyMsrln : public DapKeyAbstract
{
    dap_enc_key *_key = Q_NULLPTR;
    DapKeyAes *_sharedSessionKey = Q_NULLPTR;

    friend class DapCrypt;
public:
    DapKeyMsrln();
    ~DapKeyMsrln();

    QByteArray generateAliceMessage();

    // Generate shared key and bob message
    QByteArray generateBobMessage(QByteArray aliceMessage);

    // Sets shared key
    bool generateSessionKey(const QByteArray& bobMessage,
                                          const QByteArray& seed);

    bool generateAliceSharedKey(const QByteArray& bobMessage);

    void encode(QByteArray& in, QByteArray& out);
    void decode(QByteArray& in, QByteArray& out);
    bool init(const QString& key);
};

#endif // DAPKEYMSRLN_H
