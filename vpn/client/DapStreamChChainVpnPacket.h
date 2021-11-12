#ifndef DAPSTREAMCHCHAINVPNPACKET_H
#define DAPSTREAMCHCHAINVPNPACKET_H

#include <QtGlobal>

namespace Dap {
namespace Stream {
struct PacketHdr{
    int socket_id;
    quint32 op_code;
    quint32 usage_id;
    union
    {
        struct
        {
            quint32 addr_size;
            quint16 port;
            quint16 padding;
        } __attribute__((packed)) op_connect ;
        struct
        {
            quint32 data_size;
            quint32 padding;
        } __attribute__((packed)) raw ;   // Default access to raw OP data
        struct
        {
            quint32 data_size;
            quint32 padding;
        } __attribute__((packed)) op_data ;
    };
} __attribute__((packed));

struct Packet
{
    PacketHdr header;
    quint8 data[];
} __attribute__((packed));

}
}
#endif // DAPSTREAMCHCHAINVPNPACKET_H
