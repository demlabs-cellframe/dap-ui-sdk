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
