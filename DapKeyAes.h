#ifndef DAPKEYAES_H
#define DAPKEYAES_H

#include <openssl/aes.h>
#include <openssl/hmac.h>
#include <QByteArray>
#include "InterfaceDapKey.h"

typedef unsigned char byte;

#define AES_BLOCKSIZE 16
#define AES_KEY_LENGTH 256

class DapKeyAes : public DapKey
{
    unsigned char* m_keyStr;
public:
    DapKeyAes() { }

    void encode(QByteArray& in, QByteArray& out);
    void decode(QByteArray& in, QByteArray& out);
    bool init(const QString& str_key);
};

#endif // DAPKEYAES_H
