#ifndef DAPCHSOCKFORW_H
#define DAPCHSOCKFORW_H

#include "DapChBase.h"
#include "DapSession.h"

#include "DapSockForwPacket.h"
#include "DapTunNative.h"
#include "DapStreamer.h"

#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>

#define STREAM_SF_PACKET_OP_CODE_CONNECTED 0x000000a9
#define STREAM_SF_PACKET_OP_CODE_CONNECT 0x000000aa
#define STREAM_SF_PACKET_OP_CODE_DISCONNECT 0x000000ab
#define STREAM_SF_PACKET_OP_CODE_SEND 0x000000ac
#define STREAM_SF_PACKET_OP_CODE_RECV 0x000000ad
#define STREAM_SF_PACKET_OP_CODE_PROBLEM 0x000000ae

#define STREAM_SF_PROBLEM_CODE_NO_FREE_ADDR 0x00000001
#define STREAM_SF_PROBLEM_CODE_TUNNEL_DOWN  0x00000002
#define STREAM_SF_PROBLEM_CODE_PACKET_LOST  0x00000003

#define STREAM_SF_PACKET_OP_CODE_RAW_L3 0x000000b0
#define STREAM_SF_PACKET_OP_CODE_RAW_L2 0x000000b1
#define STREAM_SF_PACKET_OP_CODE_RAW_L3_ADDR_REQUEST 0x000000b2
#define STREAM_SF_PACKET_OP_CODE_RAW_L3_ADDR_REPLY 0x000000b3
#define STREAM_SF_PACKET_OP_CODE_RAW_L3_DEST_REPLY 0x000000b4

#define STREAM_SF_PACKET_OP_CODE_RAW_SEND 0x000000bc
#define STREAM_SF_PACKET_OP_CODE_RAW_RECV 0x000000bd

struct DapSockForwPacket;

class DapChSockForw : public DapChBase
{
    Q_OBJECT
public:
    DapChSockForw(DapStreamer * a_streamer, DapSession * mainDapSession);

    bool isTunCreated(){return tun->isCreated();}

    void tunCreate (const QString& a_addr, const QString& a_gw);
    void workerStart(int a_tunSocket);

    quint16 addForwarding(const QString remoteAddr, quint16 remotePort, quint16 localPort);
    void delForwarding(int sockId);
    void delForwardingAll();

    DapStreamer * streamer(){ return m_streamer; }
public slots:
    void onPktIn(DapChannelPacket *pkt) override;
    void packetOut(DapSockForwPacket *pkt);

    void requestIP();
    void netConfigClear();

    void tunCreate(); // create with all predefined before values
    void tunDestroy();
private slots:
    void onFwServerConnected();
    void onFwClientReadyRead();
    void onFwClientDisconnected();

signals:
    void bytesRead(int);
    void bytesWrite(int);

    void fwdDisconnected(int sockServ, int sockClient);
    void usrMsg(QString);
    void sigPacketRead();
    void sigPacketWrite();

    void sigTunNativeCreate();
    void sigNativeDestroy();

    void netConfigReceived(QString,QString);
    void netConfigRequested();
    void netConfigReceivedSame();
    void netConfigCleared();
    void tunCreated();
    void tunDestroyed();
    void tunError(const QString&);
    void tunWriteData();

    void ipRequested();

    void sendCmdAll(const QString&);

private:
    // DATAS
    DapStreamer * m_streamer;
    DapSession * m_mainDapSession;

    QMap<int, QTcpSocket* > m_socketsIn;
    QMap<int, QTcpServer* > m_servs;
    QMap<int, QString > m_servsRemoteAddr;
    QMap<int, quint16 > m_servsRemotePort;

    QMap<int, QTcpSocket* > m_socksIn;
    QMap<int, QTcpSocket* > m_socksOut;

    DapTunNative * tun;
    // METHODS
    QString m_addr, m_gw;
    QTcpServer *m_fdListener;
};

#endif // DAPCHSOCKFORW_H
