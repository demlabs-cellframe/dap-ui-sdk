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

#include "DapChBase.h"
#include <QByteArray>
#include <QString>
#include <cstring>

void DapChBase::write_str(char ch, const QString& str)
{
    DapChannelPacketHdr *pkt = (DapChannelPacketHdr*) calloc(1, sizeof(DapChannelPacketHdr));
    QByteArray b = str.toLatin1();
    void *data = malloc(b.length());
    memcpy(data,b.constData(),b.size());
    pkt->id = m_id;
    pkt->type = ch;
    pkt->size = b.size();
    emit pktChOut(pkt, data);
}
