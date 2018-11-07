#ifndef DAPKEYMSRLN_H
#define DAPKEYMSRLN_H

#include "InterfaceDapKey.h"
#include "rand/dap_rand.h"
#include "dap_enc_key.h"
// #include "liboqs/kex/kex.h"
#include "DapKeyIaes.h"


class DapKeyMsrln : public DapKey
{
    dap_enc_key *key;

  //  OQS_RAND* rand;
    void* private_key;

    uint8_t* public_key;
    size_t public_length;
//    OQS_KEX* kex;
    DapKeyIaes* aes_key;

public:
    DapKeyMsrln();

    QByteArray generateAliceMessage();

    QByteArray generateBobMessage(QByteArray aliceMessage);

    void convertToAes();

    bool makePublicKey(QByteArray &bobMessage);

    void encode(QByteArray& in, QByteArray& out);
    void decode(QByteArray& in, QByteArray& out);
    bool init(const QString& key);
};

#endif // DAPKEYMSRLN_H
