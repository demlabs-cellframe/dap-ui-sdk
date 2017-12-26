/*
 Copyright (c) 2017-2018 (c) Project "DeM Labs Inc" https://github.com/demlabsinc
  All rights reserved.

 This file is part of DAP (Deus Applications Prototypes) the open source project

    DAP (Deus Applicaions Prototypes) is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DAP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with any DAP based project.  If not, see <http://www.gnu.org/licenses/>.
*/


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
