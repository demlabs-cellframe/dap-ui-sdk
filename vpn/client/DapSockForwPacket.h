#ifndef DAPSOCKFORWPACKET_H
#define DAPSOCKFORWPACKET_H

#include <QtGlobal>

// CRITICAL: This structure MUST match the old DapStreamChChainVpnPacket.h::PacketHdr
// Server expects exactly this format with usage_id field!
struct DapSockForwPacketHdr{
    int socket_id;        // Socket ID (4 bytes)
    quint32 op_code;      // Operation code (4 bytes): 0xbc = send, 0xf8 = receive
    quint32 usage_id;     // Usage ID (4 bytes) - REQUIRED by server protocol!
    union
    {
        struct
        {
            quint32 addr_size;
            quint16 port;
            quint16 padding;
        } __attribute__((packed)) op_connect;
        struct
        {
            quint32 data_size;
            quint32 padding;
        } __attribute__((packed)) raw;   // Default access to raw OP data
        struct
        {
            quint32 data_size;
            quint32 padding;
        } __attribute__((packed)) op_data;
    };
} __attribute__((packed));

struct DapSockForwPacket
{
    DapSockForwPacketHdr header;
    quint8 data[];
} __attribute__((packed));

#endif // DAPSOCKFORWPACKET_H
