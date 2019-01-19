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

#include <QSysInfo>
#include "DapConnectStream.h"


const quint8 daSig[] = {0xa0,0x95,0x96,0xa9,0x9e,0x5c,0xfb,0xfa};
QByteArray daSigQ((const char*) daSig, sizeof(daSig));
const size_t daPktSizeMaximum = 10241024;

int daSigDetect(const QByteArray& b) { return b.indexOf(daSigQ); }


DapConnectStream::DapConnectStream(DapSession * session, QObject* parent) : QObject(parent),
    pktOutLastSeqID(0), m_dapPktHdr(Q_NULLPTR), m_dapData(Q_NULLPTR), m_dataStream(Q_NULLPTR),
    m_streamState(SSS_NONE), m_isStreamOpened(false)
{
    qDebug() << "[DapConnectStream::DapConnectStream]";

    m_session = session;
    m_streamSocket = new QTcpSocket(this);
    m_streamSocket->setReadBufferSize(1000024);

    connect(m_streamSocket,&QIODevice::readyRead, this,&DapConnectStream::sltStreamProcess);
    //connect(m_streamSocket,&QAbstractSocket::hostFound, this,&DapConnectStream::sltStreamHostFound);
    connect(m_streamSocket,&QAbstractSocket::connected, this,&DapConnectStream::sltStreamConnected);
    connect(m_streamSocket,&QAbstractSocket::disconnected, this,&DapConnectStream::sltStreamDisconnected);
    connect(m_streamSocket,SIGNAL(bytesWritten(qint64)), this, SLOT(sltStreamBytesWritten(qint64)) );

   typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);

   connect(m_streamSocket,static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
           this, &DapConnectStream::sltStreamError);

}

DapConnectStream::~DapConnectStream()
{

}


void DapConnectStream::writeChannelPacket(DapChannelPacketHdr *chPkt, void *data, uint64_t *dest_addr)
{
    Q_UNUSED(dest_addr)
    size_t dOutSize = chPkt->size + sizeof(DapChannelPacketHdr);
    char * dOut = (char*) calloc(1, dOutSize);

    memcpy(dOut, chPkt, sizeof(DapChannelPacketHdr));
    memcpy(dOut + sizeof(DapChannelPacketHdr), data, chPkt->size);

    QByteArray dOutEnc, dOutRaw(dOut, dOutSize);

    DapCrypt::me()->encode(dOutRaw, dOutEnc, KeyRoleStream);

    size_t pktOutDataSize = sizeof(DapPacketHdr) + dOutEnc.size();
    uint8_t* pktOutData = (uint8_t*) calloc(1, pktOutDataSize);
    DapPacketHdr* pktOut = (DapPacketHdr* ) pktOutData;

    pktOut->type = DATA_PACKET;

    memcpy(pktOut->sig, daSig, sizeof(pktOut->sig));

    pktOut->size = dOutEnc.size();

    pktOutLastSeqID++;
    if(pktOutLastSeqID == 0xffffffff)
        pktOutLastSeqID=0;

    memcpy(pktOutData + sizeof(DapPacketHdr), dOutEnc, dOutEnc.size());

    writeStreamRaw(pktOutData, pktOutDataSize);

    if(pktOutData)
        free(pktOutData);

    if(dOut)
        free(dOut);

    free (data);
    free (chPkt);
}


void DapConnectStream::streamOpen(const QString& subUrl, const QString& query)
{
    if(m_streamSocket->isOpen()) {
        qWarning() << "Stream socket already open. "
                      "Closing current open socket";
        m_streamSocket->close();
    }

    qDebug() << "[DapConnectStream] Stream open SubUrl = " << subUrl;
    qDebug() << "[DapConnectStream] Stream open query =" << query;

    m_streamID.clear();

    network_reply = m_session->streamOpenRequest(subUrl, query);

    if(network_reply)
    {
        emit streamSessionRequested();
        connect(network_reply, &QNetworkReply::finished, this, &DapConnectStream::sltStreamOpenCallback);
    } else {
        qWarning() << "Network reply is NULL. Stream not will be open";
        emit errorNetwork("Can't init network connection");
    }
}

void DapConnectStream::streamClose()
{
    qDebug() <<"[SC] close the stream";
    emit streamDisconnecting();
    if(m_streamSocket->isOpen()){
        m_streamSocket->close();
    }
    m_isStreamOpened=false;
}

qint64 DapConnectStream::writeStreamRaw(const void * data, size_t data_size)
{
    quint64 outWriteSize, wr, ret = 0;
    if(m_streamSocket->isWritable())
    {
        while (ret < (quint64) data_size)
        {
            outWriteSize = ((quint64) data_size) - ret;
            wr = m_streamSocket->write((const char*)data + ret, outWriteSize);

            if(wr > 0)
                ret += wr;
            else
            {
                if(m_isStreamOpened)
                    return 0;
                qDebug() << "[DapConnectStream] Wr = " << wr << " stream closed!";
                //emit streamClosed(); ///todo in pervois version DAP this string be uncommented
            }
        }
        return ret;
    }
    else
    {
        qWarning() << "Stream Socket is not writable";
        if(!m_streamSocket->isOpen()) {
            qWarning() << "Stream socket is closed";
            emit streamClosed();
        }
        return 0;
    }
}

void DapConnectStream::sltStreamOpenCallback()
{
    qDebug() << "Stream Open callback;";

    if(network_reply->error() != QNetworkReply::NetworkError::NoError)
        qWarning() << "Network Reply Error" << network_reply->error();

    if(network_reply->error() >= QNetworkReply::NetworkError::ConnectionRefusedError &&
            network_reply->error() <= QNetworkReply::NetworkError::UnknownNetworkError ) {
        qWarning() << "Can't open stream. Network error";
        emit sigStreamOpenNetworkError(network_reply->error());
        return;
    }

    QVariant statusCode = network_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (!statusCode.isValid()) {
        qWarning() << "Status code is not valid";
    } else if (statusCode.toInt() == 401 ) {
        qWarning() << "Server return unauthorized code. Need authorization";
        emit sigStreamOpenHttpError(statusCode.toInt());
        return;
    } else if (statusCode.toInt() == 400 ) {
        qWarning() << "Bad Reqeust! Maybe without cookie or keyID.";
        emit sigStreamOpenHttpError(statusCode.toInt());
    } else if (statusCode.toInt() >= 402 && statusCode.toInt() < 600) {
        emit sigStreamOpenHttpError(statusCode.toInt());
    }

    QByteArray baReply(network_reply->readAll());
    if(baReply.size() == 0) {
        qWarning() << "Reply is empty";
        emit sigStreamOpenBadResponseError();
        return;
    }

    QByteArray streamReplyDec;
    DapCrypt::me()->decode(baReply, streamReplyDec, KeyRoleSession);
    QString streamReplyStr(streamReplyDec);

    QStringList str_list = streamReplyStr.split(" ");

    if(str_list.length() != 2)
    {
        qWarning() << "Bad response. Wrong Reply Format!" << streamReplyStr;
        emit sigStreamOpenBadResponseError();
        return;
    }

    m_streamID = str_list.at(0);
    QString streamServKey = str_list.at(1);

    if(!m_streamID.isEmpty()) {
        qDebug() << "Stream id:" << m_streamID;
        qDebug()  << "[DapConnectStream] Stream server key for client requests: "
                  << streamServKey;
        emit streamServKeyRecieved();
        DapCrypt::me()->initAesKey(streamServKey, KeyRoleStream);

        if(!m_streamSocket->isOpen()) {
            m_streamSocket->connectToHost(m_session->upstreamAddress(),
                                          m_session->upstreamPort(),
                                          QIODevice::ReadWrite);
        } else {
            qCritical() << "Stream already open";
        }

    } else {
        qWarning() << "Can't open stream." << m_streamID;
        emit sigStreamOpenBadResponseError();
        m_streamID.clear();
    }

}

void DapConnectStream::sltStreamBytesWritten(qint64 bytes)
{
    (void) bytes;
}

void DapConnectStream::sltStreamError(QAbstractSocket::SocketError socketError)
{

    qWarning() << "Socket error: " <<m_streamSocket->errorString();
    switch (socketError) {
        case QAbstractSocket::NetworkError:
            emit errorNetwork( tr("Networking error, pls fix your network configuration"));
        break;
        case QAbstractSocket::SocketAccessError:
            m_isStreamOpened=false;
            emit errorNetwork( tr("Socket access error, has no privileges for socket operations"));
        break;
        case QAbstractSocket::DatagramTooLargeError:
            emit errorNetwork( tr("Too large datagram you're trying to send through the socket (more than 8192 bytes)"));
        break;
        case QAbstractSocket::UnsupportedSocketOperationError:
            emit errorNetwork( tr("The requested socket operation is not supported by the local operating system (e.g., lack of IPv6 support)"));
        break;
        case QAbstractSocket::ProxyAuthenticationRequiredError:
            m_isStreamOpened=false;
            emit errorNetwork( tr("The socket is using a proxy, and the proxy requires authentication"));
        break;
        case QAbstractSocket::UnfinishedSocketOperationError:
            emit errorNetwork( tr("The last operation attempted has not finished yet (still in progress in the background)"));
        break;
        case QAbstractSocket::ProxyConnectionRefusedError:
            emit errorNetwork( tr("Could not contact the proxy server because the connection to that server was denied"));
        break;
        case QAbstractSocket::HostNotFoundError:
            emit errorNetwork( tr("The host %1 was not found. Please check the "
                           "host name and port %2 settings.")
                 .arg(m_session->upstreamAddress())
                 .arg(m_session->upstreamPort()));
        break;
        case QAbstractSocket::ConnectionRefusedError:
            emit errorNetwork( tr("The connection was refused by the peer. "
                           "Make sure the %1 is running, "
                           "and check that the host name and port %2 "
                           "is open")
                 .arg(m_session->upstreamAddress())
                 .arg(m_session->upstreamPort()));

            break;
        default:{
            emit errorNetwork(tr("The following error occurred: %1")
                       .arg(m_streamSocket->errorString()));

        }
    }
}

void DapConnectStream::sltStreamDisconnected()
{
    qDebug() << "[DapConnectStream] sltStreamDisconnected.";
    emit finished();
    emit streamClosed();

    if(m_dataStream)
        delete m_dataStream;
    m_dataStream = Q_NULLPTR;
}

void DapConnectStream::sltStreamConnected()
{
    qDebug() << "[DapConnectStream] Connected to the server";
    m_isStreamOpened=true;
    m_buf.clear();
    if(m_dataStream)
        delete m_dataStream;

    m_dataStream = new QDataStream(m_streamSocket);
    QString str_url = QString("%1/fjskd9234j?fj913htmdgaq-d9hf=%2")
            .arg(DapSession::URL_STREAM).arg(m_streamID);

    qDebug() << "[DapConnectStream] Stream URL: " << str_url;

    // Use QNetworkRequest or DapConnectClient::request
    QString str_request = QString(
                          "POST %2 HTTP/1.1\r\n"
                          "Connection: Keep-Alive\r\n"
                          "Cookie: %1\r\n"
                          "User-Agent: DapClient %4 \r\n"
                          "Host: %3\r\n"
                          "\r\n")
                            .arg(m_session->cookie())
                            .arg(str_url).arg(m_session->upstreamAddress())
                            .arg(2); // Do something with versioning

    qDebug() << "[DapConnectStream] Request on out : " << str_request;

    QByteArray baReq( str_request.toLatin1() );
    quint64 ret = m_streamSocket->write(baReq.constData(), baReq.size());
    qDebug() << "[DapConnectStream] HTTP stream request sent "<< ret<< " bytes";

    if( !m_streamSocket->waitForBytesWritten())
        qDebug() << "[DapConnectStream] Can't wait until all bytes are sent: "
                 << m_streamSocket->errorString();

    m_streamState = SSS_FRAME_SEARCH;
    emit streamOpened();
}


void DapConnectStream::sltStreamProcess()
{
   // qDebug()<< "[DapConnectStream::sltStreamProcess]";
    if( m_streamSocket->bytesAvailable() == 0 )
    {
        qWarning() << "[DapConnectStream] No data after wait!";
        return;
    }

    if( m_streamSocket->bytesAvailable() < 0 )
    {
        qWarning() << "[DapConnectStream] No data";
        return;
    }

    QByteArray btAr = m_streamSocket->readAll();
    int bufferLengthBefore = m_buf.length();
    m_buf.append(btAr);
    bool cycle_flag = true;

    while (cycle_flag) {

     switch (m_streamState) {

        case SSS_HTTP_HEADER:
        {
            cycle_flag = false;
            qDebug() << "[DapConnectStream] case HTTP_HEADER";
            char str_hdr_end[] = { 0x0D,0x0A,0x0D,0x0A };
            int header_end = m_buf.indexOf(str_hdr_end);
            if(header_end != -1)
            {
                header_end += strlen(str_hdr_end);
                header_end -= bufferLengthBefore;
                qDebug() << "[DapConnectStream] HTTP header end = " << header_end;
                m_streamState = SSS_FRAME_SEARCH;
            }
            else
                qDebug() << "[DapConnectStream] HTTP header ends in the next frame";
        } break;

        case SSS_FRAME_SEARCH:
        {
            int sigPos = daSigDetect(m_buf);
            if(sigPos < 0)
            {
               // qWarning() << "[DapConnectStream] SigPos < 0 !";
                return;
            }
            else
            {
                m_buf = m_buf.mid(sigPos);
                m_streamState = SSS_FRAME_HEADER;
            }
        } break;

        case SSS_FRAME_HEADER:
        {
            if(m_buf.length() >= (int) sizeof(DapPacketHdr))
            {
                const DapPacketHdr* dapPktConstHdr = (DapPacketHdr* ) m_buf.constData();
                size_t dapPktSize = dapPktConstHdr->size;

                if(dapPktSize > daPktSizeMaximum)
                {
                    qWarning() << "[DapConnectStream] Package size too big = " << dapPktSize;
                    m_buf.clear();
                    m_streamState = SSS_FRAME_SEARCH;
                    m_dapDataPosition = 0;
                    return;
                }

             /*   if(m_dapPktHdr)
                    free(m_dapPktHdr);      ???????
                if(m_dapData)
                    free(m_dapData); */

                m_dapPktHdr = (DapPacketHdr*) calloc(1,sizeof(DapPacketHdr));
                m_dapData = (quint8*) calloc(1,dapPktSize + 16); // +16 это попытка вылечить sig segf.


                memcpy(m_dapPktHdr, m_buf.constData(), sizeof(DapPacketHdr));

                m_buf = m_buf.mid(sizeof(DapPacketHdr));

                if( (int)dapPktSize <= m_buf.length() )
                {
                    memcpy(m_dapData, m_buf.constData(), dapPktSize);
                    procPktIn(m_dapPktHdr, m_dapData);
                    m_streamState = SSS_FRAME_SEARCH;
                    m_buf = m_buf.mid(dapPktSize);
                    m_dapDataPosition = 0;
                }
                else
                {
                    m_dapDataPosition = m_buf.length();
                    memcpy(m_dapData, m_buf.constData(), m_dapDataPosition);
                    m_buf.clear();
                    m_streamState = SSS_FRAME_BODY;
                    cycle_flag = false;
                }
            } else {
                //qDebug() << "[DapConnectStream] Collecting data for DA frame header";
                cycle_flag = false;
            }
        } break;

        case SSS_FRAME_BODY:
        {

          if(m_buf.length() < ( (int)m_dapPktHdr->size) - m_dapDataPosition)
          {
              memcpy(m_dapData + m_dapDataPosition, m_buf.constData(), (size_t) m_buf.length());
              m_dapDataPosition += m_buf.length();
              m_buf.clear();
              cycle_flag = false;
          }
          else
          {
              size_t bytesLeft = m_dapPktHdr->size - ( (size_t)m_dapDataPosition );
              memcpy(m_dapData + m_dapDataPosition, m_buf.constData(), bytesLeft);

              procPktIn(m_dapPktHdr, m_dapData);
              m_streamState = SSS_FRAME_SEARCH;
              m_buf = m_buf.mid(bytesLeft);
          }

        } break;

        default: qWarning() << "[DapConnectStream] Unexcepted state";
            break;
     }
   }
}

void DapConnectStream::procPktIn(DapPacketHdr * pkt, void * data)
{
    QByteArray decData, inData;
    inData.append((const char*) data, pkt->size);

    DapCrypt::me()->decode(inData, decData, KeyRoleStream);

    if(decData.size() == 0) {
        qWarning() << "Error decode. Packet loosed";
        return;
    }

    DapChannelPacketHdr* channelPkt = (DapChannelPacketHdr*) calloc (1,sizeof(DapChannelPacketHdr));
    memcpy(channelPkt, decData.constData(), sizeof(DapChannelPacketHdr));

    void* channelData = calloc(1, decData.size() - sizeof(DapChannelPacketHdr));
    memcpy(channelData, decData.constData() + sizeof(DapChannelPacketHdr),
           decData.size() - sizeof(DapChannelPacketHdr));

    readChPacket(channelPkt, channelData);
    if(pkt)
        free(pkt);
    else
        qCritical() << "[DapConnectStream] DapPacketHdr is NULL in procPktIn!";

    if(data)
        free(data);
    else
        qCritical() << "[DapConnectStream] (void * data) is NULL in procPktIn!";
}



