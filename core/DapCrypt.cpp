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
#include "DapKeyCommon.h"
#include <QDebug>
#include "DapSession.h"
#include <QRandomGenerator>

/**
 * @brief DapCrypt::DapCrypt
 * keyStream - AES key generated after handshake
 */
DapCrypt::DapCrypt()
{
    keySession = new DapKeyMsrln;
    keyStream = Q_NULLPTR;
}

DapCrypt::DapCrypt(const DapCrypt &rhs) : keyStream(Q_NULLPTR) {
    if (keySession) {
        if (keySession->_sharedSessionKey) {
            delete keySession->_sharedSessionKey;
            keySession->_sharedSessionKey = nullptr;
        }
        delete keySession;
        keySession = nullptr;
    }
    keySession = new DapKeyMsrln;
    keySession->_key = dap_enc_key_dup(rhs.keySession->_key);
    keySession->_sharedSessionKey = new DapKey(rhs.keySession->_sharedSessionKey->m_key);
}

DapCrypt::~DapCrypt()
{
    delete keySession;
}

DapKeyAbstract* DapCrypt::roleToKey(KeyRole kRole) const
{
    switch(kRole) {
    case KeyRoleSession:    return keySession;
    case KeyRoleStream:     return keyStream;
    }
}



QString DapCrypt::getRandomString(int lengthStr)
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

    QString randomString;

    for(int i = 0; i < lengthStr; ++i)
    {
        const int index = QRandomGenerator::global()->bounded(possibleCharacters.length());
        randomString.append(possibleCharacters.at(index));
    }
    return randomString;
}

void DapCrypt::encode(QByteArray& in, QByteArray& out, KeyRole kRole) const
{
    DapKeyAbstract * key = roleToKey(kRole);
    if( key == Q_NULLPTR )
    {
        qCritical() << "[DapCrypt] Error! No key for role! [" << kRole << "]";
        return;
    }
    key->encode(in,out);
}

void DapCrypt::decode(QByteArray& in, QByteArray& out, KeyRole kRole) const
{
    DapKeyAbstract * key = roleToKey(kRole);
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

void DapCrypt::initKey(dap_enc_key_type_t a_type, QString &a_keyStr, KeyRole a_keyRole) {
    DapKeyAbstract **dapKey;
    switch(a_keyRole)
    {
    case KeyRoleStream:
        dapKey = &keyStream;
        break;
    default:
        qCritical() <<"Wrong key role " << a_keyRole;
        return;
    }

    if(*dapKey)
        delete *dapKey;

    *dapKey = new DapKey(a_type, a_keyStr);
}
