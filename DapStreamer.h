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
#include "DapChThread.h"
#include "DapChBase.h"

#define DATA_PACKET 0x00
#define SERVICE_PACKET 0xff
#define KEEPALIVE_PACKET 0x11


class DapStreamer : public QObject
{
    Q_OBJECT
public:
    enum SafeartsStreamState { SSS_NONE, SSS_CONNECTING, SSS_HTTP_HEADER,
                               SSS_FRAME_SEARCH, SSS_FRAME_HEADER, SSS_FRAME_BODY};

    DapStreamer(DapSession * session, QObject* parent = Q_NULLPTR);

    ~DapStreamer();
    bool isConnected() { return m_streamSocket->isOpen(); }
    int upstreamSocket() { return m_streamSocket->isOpen()?m_streamSocket->socketDescriptor(): -1; }

    DapChThread* addChProc(char chId, DapChBase* obj);

protected:
    static QHash<char, DapChBase*> m_dsb;
    DapChThread* m_dapChThead = Q_NULLPTR;
    DapSession *m_session;
    quint32 m_pktOutLastSeqID;
    DapPacketHdr m_dapPktHdr;

    quint8 m_dapData[DAP_PKT_SIZE_MAX] = {0};

    QAbstractSocket * m_streamSocket;

    QByteArray m_buf;

    int m_dapDataPosition;

    QNetworkReply * m_network_reply = Q_NULLPTR;

    SafeartsStreamState m_streamState;
    bool m_isStreamOpened;

    QString m_streamID;

    QByteArray m_procPktInDecData, m_procPktInData;
    void procPktIn(DapPacketHdr * pkt, void * data);

    // variables for DapStreamer::writeChannelPacket
    char m_writeDataOut[DAP_PKT_SIZE_MAX] = {0};
    char m_writeEncDataOut[DAP_PKT_SIZE_MAX] = {0};
    qint64 writeStreamRaw(const void * data, size_t data_size);

private slots:
    void sltStreamProcess();
    void sltStreamConnected();
    void sltStreamDisconnected();

    void sltStreamError(QAbstractSocket::SocketError socketError);
    void sltStreamOpenCallback();

    void sltStreamBytesWritten(qint64 bytes);

protected slots:
    void readChPacket(DapChannelPacketHdr* pkt, void* data) {
        m_dapChThead->sltProcPacket(new DapChannelPacket(pkt, data));
    }

public slots:
    void openDefault() {
        qDebug() << "[DapStreamer] Open socket_forward media item sf=1";
        streamOpen("socket_forward","sf=1");
    }

    void abortStreamRequest() { m_network_reply->abort(); }

    void streamOpen(const QString& subUrl, const QString& query);
    void streamClose();

    void writeChannelPacket(DapChannelPacketHdr *chPkt, void *data, uint64_t *dest_addr = Q_NULLPTR);

    // virtual void readChPacket(DapChannelPacketHdr *pkt, void *data) = 0;
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
