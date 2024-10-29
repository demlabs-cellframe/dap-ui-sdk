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

#ifndef DAPCHANNELPACKET_H
#define DAPCHANNELPACKET_H

#include <QDataStream>

#define DAP_PKT_SIZE_MAX 0xFFFF

struct DapChannelPacketHdr
{
    quint8 id; // channel_id
    quint8 enc_type; // Zero if not encrypted
    quint8 type;
    quint8 padding;
    quint64 seq_id;
    quint32 size;
}  __attribute__((packed));

struct DapPacketHdr
{
    uint8_t sig[8];
    uint32_t size;
    uint64_t timestamp;
    uint8_t type;
    uint64_t src_addr, dst_addr;
} __attribute__((packed));


class DapChannelPacket
{
    DapChannelPacketHdr* m_hdr;
    void* m_data;
public:
    DapChannelPacket(DapChannelPacketHdr* _hdr, void* _data): m_hdr(_hdr), m_data(_data) { }
    ~DapChannelPacket() {
        if (m_hdr) {
            free(m_hdr);
        }

        if (m_data) {
            free(m_data);
        }
    }

    const DapChannelPacketHdr* hdr() { return m_hdr; }
    const void *data() { return m_data; }

    void unleashData() { m_data = nullptr; } // Prevent double free action
};



#endif // DAPCHANNELPACKET_H
