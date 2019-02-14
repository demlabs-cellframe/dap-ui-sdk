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

#include "DapCrypt.h"
#include "DapKeyAes.h"
#include <QDebug>
#include "DapSession.h"

/**
 * @brief DapCrypt::DapCrypt
 * keyStream - AES key generated after handshake
 */
DapCrypt::DapCrypt()
{
    keySession = new DapKeyMsrln;
    keyStream = Q_NULLPTR;
}

DapCrypt::~DapCrypt()
{
    delete keySession;
}

DapKey* DapCrypt::roleToKey(KeyRole kRole) const
{
    switch(kRole)
    {
    case KeyRoleSession: return keySession;
    case KeyRoleStream:   return keyStream;
    default: return Q_NULLPTR;
    }
}



QString DapCrypt::getRandomString(int lengthStr)
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

    QString randomString;

    for(int i = 0; i < lengthStr; ++i)
    {
        int index = qrand() % possibleCharacters.length();
        randomString.append(possibleCharacters.at(index));
    }
    return randomString;
}

void DapCrypt::encode(QByteArray& in, QByteArray& out, KeyRole kRole) const
{
    DapKey * key = roleToKey(kRole);
    if( key == Q_NULLPTR )
    {
        qCritical() << "[DapCrypt] Error! No key for role! [" << kRole << "]";
        return;
    }
    key->encode(in,out);
}

void DapCrypt::decode(QByteArray& in, QByteArray& out, KeyRole kRole) const
{
    DapKey * key = roleToKey(kRole);
    if( key == Q_NULLPTR )
    {
        qCritical() << "[DapCrypt] Error! No key for role! [" << kRole << "]";
        return;
    }
    key->decode(in,out);
}

QByteArray DapCrypt::generateAliceMessage()
{
    return keySession->generateAliceMessage();
}

bool DapCrypt::generateSharedSessionKey(const QByteArray& bobMsg,
                                        const QByteArray& sessionID)
{
    return keySession->generateSessionKey(bobMsg, sessionID);
}

void DapCrypt::initAesKey(QString &keyStr, KeyRole kRole)
{
    QString keyEnd;
    DapKey ** dapKey;
    switch(kRole)
    {
    case KeyRoleStream:
        dapKey = &keyStream;
        keyEnd = keyStr;
        break;
    default:
        qCritical() <<"[DapCrypt] initAESKEY: Wrong key role " << kRole;
        return;
    }

    if(*dapKey)
        delete *dapKey;

    *dapKey = new DapKeyAes;
    (*dapKey)->init(QString(keyStr));
}
