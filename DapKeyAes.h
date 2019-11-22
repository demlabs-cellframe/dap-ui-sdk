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


#ifndef DAPKEYAES_H
#define DAPKEYAES_H

#include <QByteArray>
#include "InterfaceDapKey.h"
#include "dap_enc_key.h"

typedef unsigned char byte;

#define BUFF_ENC_OP_SIZE 100000

class DapKeyAes : public DapKeyAbstract
{
private:
    dap_enc_key* _key;
    char decode_buff[BUFF_ENC_OP_SIZE];
    char encode_buff[BUFF_ENC_OP_SIZE];
public:
    DapKeyAes();
    ~DapKeyAes();
    void encode(QByteArray& in, QByteArray& out);
    void decode(QByteArray& in, QByteArray& out);

    bool init(const QString& kex_buf);
    bool init(const QByteArray& seed, const QByteArray& kex_buf);
};

#endif // DAPKEYAES_H
