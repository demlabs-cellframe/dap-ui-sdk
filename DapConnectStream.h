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

#include "DapConnectBase.h"
#include "DapSession.h"
#include "DapChannelPacket.h"
#include <DapCrypt.h>
#include <QAbstractSocket>

#define DATA_PACKET 0x00
#define SERVICE_PACKET 0xff
#define KEEPALIVE_PACKET 0x11

class DapConnectStream : public DapConnectBase
{
    Q_OBJECT
public:
    enum SafeartsStreamState { SSS_NONE, SSS_CONNECTING, SSS_HTTP_HEADER,
                               SSS_FRAME_SEARCH, SSS_FRAME_HEADER, SSS_FRAME_BODY};
    DapConnectStream(QObject* parent = Q_NULLPTR); //ok
    ~DapConnectStream(); //ok
    bool isConnected() { return m_streamSocket->isOpen(); } //ok
    int upstreamSocket() { return m_streamSocket->isOpen()?m_streamSocket->socketDescriptor(): -1; }
protected:
    quint32 pktOutLastSeqID;

    DapPacketHdr * m_dapPktHdr;

    quint8 * m_dapData;

    QAbstractSocket * m_streamSocket;
    QDataStream * m_dataStream;

    QByteArray m_buf;
    QByteArray m_streamCtlReply;

    int m_dapDataPosition;

    SafeartsStreamState m_streamState;
    bool m_isStreamOpened;

    QString m_streamID;

    //QString httpAddress(); //ok

    void procPktIn(DapPacketHdr * pkt, void * data);//ok

    qint64 writeStreamRaw(const void * data, size_t data_size);//ok

private slots:
    void sltStreamHostFound() { emit notify("Host found"); } //ok
    void sltStreamProcess();//ok
    void sltStreamConnected();//ok
    void sltStreamDisconnected();//ok
    void sltStreamError(QAbstractSocket::SocketError socketError); //ok
    void sltIdReadyRead() { m_streamCtlReply.append(network_reply->readAll()); }//ok
    void sltIdFinishedRead();//ok
    void sltStreamBytesWritten(qint64 bytes);//ok

public slots:
    void streamOpen(const QString& subUrl, const QString& query); //ok
    void streamClose(); //ok

    void writeChannelPacket(DapChannelPacketHdr *chPkt, void *data, uint64_t *dest_addr = NULL); //ok

signals:
    void recivedChannelPacket(DapChannelPacketHdr* pkt, void* data);
    void streamOpened();
    void streamClosed();

    //new 2017_06
    void streamConnecting();
};

#endif // DAPCONNECTSTREAM_H
