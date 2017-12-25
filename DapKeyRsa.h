#ifndef DAPKEYRSA_H
#define DAPKEYRSA_H

#define RSA_KEY_LENGTH 4096
#define PUB_EXP     3

#include "InterfaceDapKey.h"
#include <openssl/bn.h>
#include <openssl/pem.h>
#include <QObject>
#include <QDebug>

class DapKeyRsa : public DapKey
{
    enum {RSA_CHIPER_BLOCK_SIZE=512};
    RSA* keyPair;
    RSA* pubKey;
    QString strPubKey;
    inline bool setStrPubKey();
public:

    DapKeyRsa();
    ~DapKeyRsa();

    void encode(QByteArray& in, QByteArray& out) override;
    void decode(QByteArray& in, QByteArray& out) override;
    bool init(const QString& key) override;
    void generateRsaLocalKeyPair();
    RSA* getPubKey() { return pubKey; }

    const QString& getStringPubKey() { return strPubKey; }
};

#endif // DAPKEYRSA_H
