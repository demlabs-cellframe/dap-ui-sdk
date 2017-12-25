#ifndef DAPCRYPT_H
#define DAPCRYPT_H

#include <QString>
#include "BaseObject.h"
#include "InterfaceDapKey.h"
#include "DapKeyRsa.h"

enum KeyRole { KeyRoleSession = 1, KeyRoleStream = 2, KeyRoleSessionServer = 3, KeyServerPublic = 4};


class DapCrypt : public BaseObject
{
    DapCrypt();
    ~DapCrypt() { }
    DapKey * keyStream;
    DapKeyRsa * keyClientSessionRsa;
    DapKeyRsa * keySesionServer;
    DapKeyRsa * keyServerPublic;

    DapKey * roleToKey(KeyRole kRole);
    QString readKeyFromBio(BIO *bio);

    static unsigned char b64_byte_decode(unsigned char b);
    static size_t b64_get_decodet_size(size_t in_size);
    static void Base64Decode(const char* source, size_t srcLen, unsigned char* out);
public:

    DapKeyRsa* getKeyServerPublic() { return keyServerPublic; }
    static DapCrypt * me() { static DapCrypt dapCrypt; return &dapCrypt; }

    static size_t fromBase64(QByteArray in, size_t in_size, unsigned char* out);
    QString getRandomString(int length);

    void initAesKey(QString &keyStr, KeyRole kRole);
    QString createRSAKey();
    bool setRsaSessionServerKey(const QString & a_keyStr);
    void setRsaPubKeyServer(const QString & a_keyStr){ keyServerPublic->init(a_keyStr); }

    void encode(QByteArray& in, QByteArray& out, KeyRole kRole);
    void encodeB64(const QString& in, QByteArray& out, KeyRole kRole);
    void decode(QByteArray& in, QByteArray& out, KeyRole kRole);
    void decodeB1k(QByteArray& in, QByteArray& out, int sizeBlock, KeyRole kRole);

};

#endif // DAPCRYPT_H
