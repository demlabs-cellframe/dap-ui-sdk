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

#include "DapConnectClient.h"
#include "DapSession.h"
#include "DapChannelPacket.h"
#include <DapCrypt.h>
#include <QAbstractSocket>

#define DATA_PACKET 0x00
#define SERVICE_PACKET 0xff
#define KEEPALIVE_PACKET 0x11

class DapConnectStream : public QObject
{
    Q_OBJECT
public:
    enum SafeartsStreamState { SSS_NONE, SSS_CONNECTING, SSS_HTTP_HEADER,
                               SSS_FRAME_SEARCH, SSS_FRAME_HEADER, SSS_FRAME_BODY};

    DapConnectStream(DapSession * session, QObject* parent = Q_NULLPTR);

    ~DapConnectStream();
    bool isConnected() { return m_streamSocket->isOpen(); }
    int upstreamSocket() { return m_streamSocket->isOpen()?m_streamSocket->socketDescriptor(): -1; }
protected:
    DapSession *m_session;
    quint32 pktOutLastSeqID;

    DapPacketHdr * m_dapPktHdr;

    quint8 * m_dapData;

    QAbstractSocket * m_streamSocket;
    QDataStream * m_dataStream;

    QByteArray m_buf;

    int m_dapDataPosition;

    QNetworkReply * network_reply = Q_NULLPTR;

    SafeartsStreamState m_streamState;
    bool m_isStreamOpened;

    QString m_streamID;

    void procPktIn(DapPacketHdr * pkt, void * data);

    qint64 writeStreamRaw(const void * data, size_t data_size);

private slots:
    void sltStreamProcess();
    void sltStreamConnected();
    void sltStreamDisconnected();

    void sltStreamError(QAbstractSocket::SocketError socketError);
    void sltStreamOpenCallback();

    void sltStreamBytesWritten(qint64 bytes);

public slots:
    void openDefault() {
        qDebug() << "[DapStreamer] Open socket_forward media item sf=1";
        streamOpen("socket_forward","sf=1");
    }

    void abortStreamRequest() { network_reply->abort(); }

    void streamOpen(const QString& subUrl, const QString& query);
    void streamClose();

    void writeChannelPacket(DapChannelPacketHdr *chPkt, void *data, uint64_t *dest_addr = Q_NULLPTR);

    virtual void readChPacket(DapChannelPacketHdr *pkt, void *data) = 0;
signals:
    void finished();
    void errorNetwork(const QString &e);
    void authenticationRequiredError();
    void notify(const QString&);

    void recivedChannelPacket(DapChannelPacketHdr* pkt, void* data);

    void streamDisconnecting();
    void streamOpened();
    void streamClosed();

    /* StreamOpen error signals */
    void sigStreamOpenHttpError(int httpCode);
    void sigStreamOpenNetworkError(QNetworkReply::NetworkError);
    void sigStreamOpenBadResponseError();
    // all another errors
    void streamOpenError(int errorCode);

    void streamSessionRequested();
    void streamServKeyRecieved();
};

#endif // DAPCONNECTSTREAM_H
