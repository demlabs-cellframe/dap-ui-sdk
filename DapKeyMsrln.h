#ifndef DAPKEYMSRLN_H
#define DAPKEYMSRLN_H

#include "InterfaceDapKey.h"
#include "liboqs/crypto/rand/rand.h"
#include "liboqs/kex/kex.h"
#include "DapKeyAes.h"


class DapKeyMsrln : public DapKey
{
    OQS_RAND* rand;
    void* private_key;
    uint8_t* public_key;
    size_t public_length;
    OQS_KEX* kex;
    DapKeyAes* aes_key;

public:
    DapKeyMsrln();

    QByteArray generateAliceMessage();

    QByteArray generateBobMessage(QByteArray aliceMessage);

    void convertToAes();

    bool makePublicKey(QByteArray &bobMessage);

    void prepare();

    void encode(QByteArray& in, QByteArray& out);
    void decode(QByteArray& in, QByteArray& out);
    bool init(const QString& key);





};

#endif // DAPKEYMSRLN_H
