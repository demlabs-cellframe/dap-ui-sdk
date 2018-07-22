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
#include "DapKeyAes.h"
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>
#include "liboqs/crypto/aes/aes.h"
#include "dap_enc_key.h"
#include "dap_enc_aes.h"



void DapKeyAes::encode(QByteArray& dataIn, QByteArray& dataOut)
{
    int tail = 0;
    if(dataIn.size() < 16)
        tail = 16 - dataIn.size();
    else if(dataIn.size() % 16 > 0)
        tail = 16 - dataIn.size() % 16;
    void * a_in_new = (void*)malloc(dataIn.size() + tail);
    memcpy(a_in_new,dataIn.data(),dataIn.size());
    memset((char*)a_in_new+dataIn.size(),0,tail);
    void* a_out = malloc(dataIn.size() + tail);
    OQS_AES128_ECB_enc((uint8_t*)a_in_new,dataIn.size()+tail,m_keyStr,(uint8_t*)a_out);
    dataOut = QByteArray((char*)a_out,dataIn.size()+tail);
    //QByteArray::fromRawData((char*)a_out,dataIn.size()+tail);
    free(a_in_new);
    free(a_out);
}


void DapKeyAes::decode(QByteArray& dataIn, QByteArray& dataOut)
{
    void* a_out = malloc(dataIn.size());
    OQS_AES128_ECB_dec((uint8_t*)dataIn.data(),dataIn.size(),m_keyStr,(uint8_t*)a_out);
    int tail = 0;
    int end = dataIn.size()-16 > 0 ? dataIn.size()-16 : 0;
    for(int i =dataIn.size()-1; i >= end; i--)
    {
        if(*((char*)a_out + i) == (char)0)
            tail++;
        else
            break;
    }
    dataOut = QByteArray((char*)a_out,dataIn.size()-tail);
    free(a_out);
    //dataOut = QByteArray::fromRawData((char*)a_out,dataIn.size()-tail);
}


bool DapKeyAes::init(const QString& str_key)
{
    m_keyStr = new unsigned char[AES_KEY_LENGTH];
    memcpy(m_keyStr, str_key.toStdString().c_str(), AES_KEY_LENGTH);
    return true;
}


bool DapKeyAes::initKeyChar(const char* str_key)
{
    m_keyStr = new unsigned char[AES_KEY_LENGTH];
    memcpy(m_keyStr, str_key, AES_KEY_LENGTH);
    return true;
}
