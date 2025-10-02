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

#ifndef DAPCONNECTSTREAM_H
#define DAPCONNECTSTREAM_H

#include <QDateTime>
#include <QQueue>
#include <QPair>
#include <QTimer>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QObject>
#include <QString>
#include <QByteArray>
#include <DapCrypt.h>

#include "DapConnectClient.h"
#include "DapSession.h"
#include "DapChannelPacket.h"
#include "DapChBase.h"
#include "DapClientDefinitions.h"

#define DATA_PACKET 0x00
#define SERVICE_PACKET 0xff
#define KEEPALIVE_PACKET 0x11
#define CH_KEEPALIVE_PKT 0x88


class DapStreamer : public QObject
{
    Q_OBJECT
public:
    enum SafeartsStreamState { SSS_NONE, SSS_CONNECTING, SSS_HTTP_HEADER,
                               SSS_FRAME_SEARCH, SSS_FRAME_HEADER, SSS_FRAME_BODY};

    DapStreamer(DapSession * session, QObject* parent = Q_NULLPTR);

    ~DapStreamer();
    bool isConnected() { return m_streamSocket.isOpen(); }
    int upstreamSocket() { return m_streamSocket.isOpen() ? m_streamSocket.socketDescriptor(): -1; }
    void addChProc(char chId, DapChBase* obj);
    void setStreamOpened(bool b) { m_isStreamOpened = b; }
    void setStreamTimeoutCheck(bool b) { m_timeoutStreamCheck = b; }
    uint16_t m_reconnectAttempts;
    uint16_t m_aliveChecks;

    void sltStreamStateChanged(QAbstractSocket::SocketState state);
    void logSocketState(QAbstractSocket::SocketState state);

    void _detectPacketLoose(quint64 currentSeqId);

protected:
    static QHash<char, DapChBase*> m_dsb;
    DapSession *m_session;
    quint32 m_pktOutLastSeqID;
    DapPacketHdr m_dapPktHdr;

    quint8 m_dapData[DAP_PKT_SIZE_MAX] = {0};

    QTcpSocket m_streamSocket;

    QByteArray m_buf;

    int m_dapDataPosition;
    DapNetworkReply * m_network_reply = Q_NULLPTR;

    SafeartsStreamState m_streamState;
    volatile bool m_isStreamOpened;
    volatile bool m_timeoutStreamCheck;
    QString m_streamID;

    QByteArray m_procPktInDecData, m_procPktInData;
    void procPktIn(DapPacketHdr * pkt, void * data);

    // variables for DapStreamer::writeChannelPacket
    char m_writeDataOut[DAP_PKT_SIZE_MAX] = {0};
    char m_writeEncDataOut[DAP_PKT_SIZE_MAX] = {0};
    qint64 writeStreamRaw(const void * data, size_t data_size);
private:
    quint64 m_lastSeqId = quint64(-1);
    // emit sigStreamPacketLoosed if packet loose detected

    void initStreamSocket();
    QQueue<QPair<QDateTime, int>> m_packetLossQueue;
    QTimer m_timer;

    void _removeOldEntries();

private slots:
    void sltStreamProcess();
    void sltStreamConnected();
    void sltStreamDisconnected();

    void sltStreamError(QAbstractSocket::SocketError socketError);
    void sltStreamOpenCallback();

    void _printPacketLossStatistics();

public slots:
    void openDefault(   ) {
        qDebug() << "[DapStreamer] Open socket_forward media item sf=1";
        streamOpen("stream_ctl","channels=s");

    }

    void openChannels(const QString & a_channels)
    {
        streamOpen(QString("stream_ctl,channels=%1,enc_type=%2,enc_headers=%3")
                   .arg(a_channels)
                   .arg(DAP_ENC_KEY_TYPE_SALSA2012)
                   .arg(0)
                   ,"");
    }

    void sendRemainServiceRequest(const QString & a_channels, const QString & a_query,
                                  const QString& address, quint16 port) {
        streamOpen(QString("stream_ctl,channels=%1,enc_type=%2,enc_headers=%3")
                       .arg(a_channels)
                       .arg(DAP_ENC_KEY_TYPE_SALSA2012)
                       .arg(0)
                   , a_query, address, port);
    }

//    void abortStreamRequest() { m_network_reply->abort(); }

    void streamOpen(const QString& subUrl, const QString& query);
    void streamOpen(const QString& subUrl, const QString& query, const QString& address, quint16 port);
    void streamClose();

    void writeChannelPacket(DapChannelPacketHdr *chPkt, void *data, uint64_t *dest_addr = Q_NULLPTR);

    // virtual void readChPacket(DapChannelPacketHdr *pkt, void *data) = 0;
signals:
    Q_INVOKABLE void errorNetwork(const QString &e);
    void authenticationRequiredError();
    void notify(const QString&);
    void sigProcPacket(DapChannelPacket *pkt);
    void recivedChannelPacket(DapChannelPacketHdr* pkt, void* data);
    void isAlive(bool reset);
    void streamDisconnecting();
    void streamOpened();
    void streamClosed();

    /* StreamOpen error signals */
    void sigStreamOpenBadResponseError();

    // all another errors
    void streamOpenError(int errorCode);

    void streamSessionRequested();
    void streamServKeyRecieved();

    void sigStreamPacketLoosed(quint64 countLoosedPackets);
    void sigNetworkMonitor();

    void sigStreamPacketLoosed(int countLoosedPackets);
};

#endif // DAPCONNECTSTREAM_H
