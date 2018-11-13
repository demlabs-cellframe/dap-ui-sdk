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


#include <QString>
#include "DapKeyIaes.h"
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>
//#include "liboqs/crypto/aes/aes.h"
#include "dap_enc_key.h"
#include "dap_enc_iaes.h"



DapKeyIaes::DapKeyIaes()
{

}

DapKeyIaes::~DapKeyIaes()
{
    dap_enc_key_delete(_key);
}

// seed == session id
// kex_buf == shared key
bool DapKeyIaes::init(const QByteArray& seed, const QByteArray& kex_buf)
{
    //
    _key = dap_enc_key_new_generate(DAP_ENC_KEY_TYPE_IAES, kex_buf.data(),
                                    size_t(kex_buf.size()), seed.data(), size_t(seed.size()), 0);
}

void DapKeyIaes::encode(QByteArray& dataIn, QByteArray& dataOut)
{
    if(_key == Q_NULLPTR) {
        qWarning() << "Error encode. _key is null";
        return;
    }
    char* data_out;
    size_t enc_size = _key->enc(_key, dataIn, dataIn.size(), (void**)&data_out);
    dataOut = QByteArray(data_out, enc_size);
    delete data_out;
}


void DapKeyIaes::decode(QByteArray& dataIn, QByteArray& dataOut)
{
    char* data_out;
    size_t dec_size = _key->dec(_key, dataIn, (size_t)dataIn.size(), (void**)&data_out);
    dataOut = QByteArray(data_out, dec_size);
    delete data_out;
}


bool DapKeyIaes::init(const QString& kex_buf)
{

    _key = dap_enc_key_new_generate(DAP_ENC_KEY_TYPE_IAES, kex_buf.toLatin1().data(),
                                    size_t(kex_buf.length()), NULL, 0, 0);
    return true;
}


//bool DapKeyIaes::initKeyChar(const char* str_key)
//{
//  //  m_keyStr = new unsigned char[AES_KEY_LENGTH];
// //   memcpy(m_keyStr, str_key, AES_KEY_LENGTH);
//    return true;
//}
