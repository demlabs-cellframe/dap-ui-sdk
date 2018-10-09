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
#include "InterfaceDapKey.h"
#include "DapKeyMsrln.h"

enum KeyRole { KeyRoleSession = 1, KeyRoleStream = 2};


class DapCrypt
{
    DapCrypt();
    ~DapCrypt() { }
    DapKey * keyStream;
    DapKeyMsrln * keySession;

    DapKey * roleToKey(KeyRole kRole);
public:

    DapKeyMsrln* getKeyServerPublic() { return keySession; }
    static DapCrypt * me() { static DapCrypt dapCrypt; return &dapCrypt; }

    QString getRandomString(int length);

    void initAesKey(QString &keyStr, KeyRole kRole);
    void setRsaPubKeyServer(const QString & a_keyStr){ keySession->init(a_keyStr); }

    void encode(QByteArray& in, QByteArray& out, KeyRole kRole);
    void decode(QByteArray& in, QByteArray& out, KeyRole kRole);
    void decodeB1k(QByteArray& in, QByteArray& out, int sizeBlock, KeyRole kRole);
    QByteArray generateAliceMessage();
    bool makePublicKey(QByteArray& data);
};

#endif // DAPCRYPT_H
