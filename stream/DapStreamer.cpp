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

#include "DapStreamer.h"
#include <QTcpSocket>
#include <QSysInfo>
#include <QTimer>
#include <QNetworkProxy>
#include <stdlib.h>

constexpr quint8 daSig[] = {0xa0,0x95,0x96,0xa9,0x9e,0x5c,0xfb,0xfa};
QByteArray daSigQ((const char*) daSig, sizeof(daSig));

inline static int daSigDetect(const QByteArray& b) { return b.indexOf(daSigQ); }

QHash<char, DapChBase*> DapStreamer::m_dsb;


DapStreamer::DapStreamer(DapSession *session, QObject* parent) :
    QObject(parent),
    m_pktOutLastSeqID(0),
    m_streamSocket(this),
    m_streamState(SSS_NONE),
    m_isStreamOpened(false)
{
    qDebug() << "[DapStreamer::DapStreamer] Constructor called";

    // Reserve space for packet processing buffers
    m_procPktInData.reserve(DAP_PKT_SIZE_MAX);
    m_procPktInDecData.reserve(DAP_PKT_SIZE_MAX);

    connect(&m_timer, &QTimer::timeout, this, &DapStreamer::_printPacketLossStatistics);
    m_timer.start(5*60000);

    initStreamSocket();

    m_session = session;
}

void DapStreamer::initStreamSocket() {

    // Configure stream socket
    m_streamSocket.setReadBufferSize(DAP_PKT_SIZE_MAX);
    m_streamSocket.setSocketOption(QAbstractSocket::LowDelayOption, 1);
    m_streamSocket.setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    m_streamSocket.setProxy(QNetworkProxy::NoProxy);

    connect(&m_streamSocket, &QIODevice::readyRead, this, &DapStreamer::sltStreamProcess);
    //connect(m_streamSocket,&QAbstractSocket::hostFound, this,&DapConnectStream::sltStreamHostFound);
    connect(&m_streamSocket, &QAbstractSocket::connected, this, &DapStreamer::sltStreamConnected);
    connect(&m_streamSocket, &QAbstractSocket::disconnected, this, &DapStreamer::sltStreamDisconnected);

    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(&m_streamSocket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::errorOccurred),
            this, &DapStreamer::sltStreamError);

    connect(&m_streamSocket, &QAbstractSocket::stateChanged, this, &DapStreamer::sltStreamStateChanged);
}

DapStreamer::~DapStreamer()
{
}

void DapStreamer::writeChannelPacket(DapChannelPacketHdr *a_pktHdr, void *data, uint64_t *dest_addr)
{
    if (a_pktHdr->size + sizeof(DapChannelPacketHdr) > DAP_PKT_SIZE_MAX) {
        qWarning() << "Too large package size " << a_pktHdr->size;
        return;
    }

    Q_UNUSED(dest_addr)

    DapPacketHdr* pktOut = reinterpret_cast<DapPacketHdr*>(m_writeEncDataOut);
    size_t pktOutDataSize = sizeof(DapPacketHdr);

    if (a_pktHdr->type == CH_KEEPALIVE_PKT) {
        pktOut->type = KEEPALIVE_PACKET;
        pktOut->size = 0;
    } else {
        pktOut->type = DATA_PACKET;

        a_pktHdr->seq_id = m_pktOutLastSeqID++;

        size_t dOutSize = a_pktHdr->size + sizeof(DapChannelPacketHdr);

        memcpy(m_writeDataOut, a_pktHdr, sizeof(DapChannelPacketHdr));
        memcpy(m_writeDataOut + sizeof(DapChannelPacketHdr), data, a_pktHdr->size);

        QByteArray dOutEnc, dOutRaw(m_writeDataOut, dOutSize);

        m_session->getDapCrypt()->encode(dOutRaw, dOutEnc, KeyRoleStream);

        pktOutDataSize += dOutEnc.size();
        pktOut->size = dOutEnc.size();

        memcpy(m_writeEncDataOut + sizeof(DapPacketHdr), dOutEnc.constData(), dOutEnc.size());
    }

    memcpy(pktOut->sig, daSig, sizeof(pktOut->sig));

    writeStreamRaw(m_writeEncDataOut, pktOutDataSize);

    ::free(data);
    ::free(a_pktHdr);
}

void DapStreamer::sltStreamStateChanged(QAbstractSocket::SocketState state)
{
    logSocketState(state);

    if (state == QAbstractSocket::UnconnectedState)
    {
        qDebug() << "Socket has become unconnected.";
        emit errorNetwork("Socket has become unconnected");
    }
    else if (state == QAbstractSocket::ConnectedState)
    {
        qDebug() << "Socket is connected.";
    }
}

void DapStreamer::logSocketState(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::UnconnectedState:
        qDebug() << "Socket state: UnconnectedState";
        break;
    case QAbstractSocket::HostLookupState:
        qDebug() << "Socket state: HostLookupState";
        break;
    case QAbstractSocket::ConnectingState:
        qDebug() << "Socket state: ConnectingState";
        break;
    case QAbstractSocket::ConnectedState:
        qDebug() << "Socket state: ConnectedState";
        break;
    case QAbstractSocket::BoundState:
        qDebug() << "Socket state: BoundState";
        break;
    case QAbstractSocket::ListeningState:
        qDebug() << "Socket state: ListeningState";
        break;
    case QAbstractSocket::ClosingState:
        qDebug() << "Socket state: ClosingState";
        break;
    default:
        qDebug() << "Socket state: Unknown";
        break;
    }
}

void DapStreamer::streamOpen(const QString& subUrl, const QString& query)
{
    if(m_streamSocket.isOpen())
    {
        qWarning() << "Stream socket already open. "
                      "Closing current open socket";
        //streamClose();
        m_streamSocket.close();
    }
    qDebug() << "Stream open SubUrl = " << subUrl;
    qDebug() << "Stream open query =" << query;
    m_streamID.clear();
    m_network_reply = m_session->streamOpenRequest(subUrl, query, this, SLOT(sltStreamOpenCallback()), QT_STRINGIFY(errorNetwork));
}

void DapStreamer::streamOpen(const QString& subUrl, const QString& query, const QString& address, quint16 port) {
    if (m_streamSocket.isOpen())
    {
        qWarning() << "Stream socket already open. "
                      "Closing current open socket";
        //streamClose();
        m_streamSocket.close();
    }
    qDebug() << "Stream open SubUrl = " << subUrl;
    qDebug() << "Stream open query =" << query;

    m_streamSocket.connectToHost(address, port);
    if (m_streamSocket.waitForConnected(15000)) {
        QByteArray data = QString("%1?%2").arg(subUrl).arg(query).toLatin1();
        m_streamSocket.write(data);
    }
    else
    {
        qWarning() << "Failed to connect to server" << address << ":" << port;
        emit errorNetwork (tr ("Socket connection timeout"));
    }
}

void DapStreamer::streamClose()
{
    qDebug() <<"[DapStreamer] streamClose()";
    emit streamDisconnecting();
    if(m_streamSocket.isOpen())
    {
        qDebug() <<"[SC] close the stream";
        m_streamSocket.close();
    }
    emit streamClosed();
    qDebug() << "STREAM CLOSED - DapStreamer::streamClose()";
    m_pktOutLastSeqID = 0;
    m_isStreamOpened=false;
}

qint64 DapStreamer::writeStreamRaw(const void *data, size_t data_size)
{
    quint64 totalWritten = 0;

    if (!m_streamSocket.isWritable())
    {
        qWarning() << "Stream Socket is not writable";
        if (!m_streamSocket.isOpen())
        {
            qWarning() << "Stream socket is closed";
            emit streamClosed();
        }
        return 0;
    }

    while (totalWritten < static_cast<quint64>(data_size))
    {
        quint64 bytesRemaining = static_cast<quint64>(data_size) - totalWritten;
        // qDebug() << "Attempting to write" << bytesRemaining << "bytes";

        qint64 bytesWritten = m_streamSocket.write(static_cast<const char*>(data) + totalWritten, bytesRemaining);

        if (bytesWritten > 0)
        {
            totalWritten += bytesWritten;
            // qDebug() << "Successfully written" << bytesWritten << "bytes, total written:" << totalWritten;
        }
        else
        {
            // qWarning() << "[DapConnectStream] Failed to write data, bytesWritten =" << bytesWritten;
            if (!m_isStreamOpened)
            {
                // qDebug() << "Stream is closed, stopping write operation";
                emit streamClosed();
            }
            return totalWritten;
        }
    }

    // qDebug() << "Finished writing all data, total bytes written:" << totalWritten;
    return totalWritten;
}

void DapStreamer::sltStreamOpenCallback()
{
    qDebug() << "Opening stream";

    // Check if m_session or DapCrypt is null
    if (!m_session || !m_session->getDapCrypt())
    {
        qCritical() << "Session or DapCrypt is null";
        emit sigStreamOpenBadResponseError();
        return;
    }

    // Check if m_network_reply is valid
    if (!m_network_reply)
    {
        qCritical() << "Network reply is null";
        emit sigStreamOpenBadResponseError();
        return;
    }

    if (m_network_reply->error() != QNetworkReply::NoError)
    {
        if (!m_network_reply)
        {
            qCritical() << "Network reply became null before accessing errorString";
            emit sigStreamOpenBadResponseError();
            return;
        }

        QString errorMsg = m_network_reply->errorString();
        qCritical() << "Network error occurred:" << errorMsg;
        emit sigStreamOpenBadResponseError();
        return;
    }

    // Get network reply data and check if it is empty
    QByteArray replyData = m_network_reply->getReplyData();
    if (replyData.isEmpty())
    {
        qWarning() << "Network reply data is empty";
        emit sigStreamOpenBadResponseError();
        return;
    }

    // Debugging the reply data size before conversion to hex
    qDebug() << "Reply data size:" << replyData.size();

    // Safely convert the data to hex with a size limit to avoid excessive output
    const int maxHexOutputSize = 1024; // Limit hex output size to avoid large data handling
    QByteArray truncatedData = replyData.left(maxHexOutputSize);
    qDebug() << "Reply data (hex, truncated):" << truncatedData.toHex();

    // Decode the reply data
    QByteArray streamReplyDec;
    m_session->getDapCrypt()->decode(replyData, streamReplyDec, KeyRoleSession);

    if (streamReplyDec.isEmpty())
    {
        qCritical() << "Decoded reply data is empty";
        emit sigStreamOpenBadResponseError();
        return;
    }

    QString streamReplyStr(streamReplyDec);
    QStringList str_list = streamReplyStr.split(" ");

    if (str_list.length() < 2)
    {
        qWarning() << "Bad response. Wrong Reply Format!" << streamReplyStr;
        emit sigStreamOpenBadResponseError();
        return;
    }

    m_streamID = str_list.at(0);
    QString streamServKey = str_list.at(1);

    if (!m_streamID.isEmpty())
    {
        qDebug() << "Stream id:" << m_streamID;
        qDebug() << "[DapConnectStream] Stream server key for client requests: " << streamServKey;

        // Check if m_session or DapCrypt is null
        if (!m_session || !m_session->getDapCrypt())
        {
            qCritical() << "Session or cryptography module is null";
            emit sigStreamOpenBadResponseError();
            return;
        }

        // Check if streamServKey is empty or has incorrect length
        if (streamServKey.isEmpty())
        {
            qCritical() << "Stream server key is empty";
            emit sigStreamOpenBadResponseError();
            return;
        }

        // Attempt to initialize the key
        m_session->getDapCrypt()->initKey(DAP_ENC_KEY_TYPE_SALSA2012, streamServKey, KeyRoleStream);

        if (!m_streamSocket.isOpen())
        {
            m_streamSocket.connectToHost(m_session->upstreamAddress(), m_session->upstreamPort(), QIODevice::ReadWrite);

            if (m_streamSocket.waitForConnected(15000))
            {
                return;
            }
            else
            {
                qCritical() << "Socket connection timeout:" << m_streamSocket.errorString();
                emit errorNetwork(tr("Socket connection timeout"));
            }
        }
        else
        {
            qCritical() << "Stream already open";
        }
    }
    else
    {
        qWarning() << "Can't open stream " << m_streamID;
        emit sigStreamOpenBadResponseError();
        m_streamID.clear();
    }
}

void DapStreamer::sltStreamError(QAbstractSocket::SocketError socketError)
{
    qWarning() << "Socket error: " <<m_streamSocket.errorString();
    switch (socketError)
    {
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
                       .arg(m_streamSocket.errorString()));
        }
    }
}

void DapStreamer::sltStreamDisconnected()
{
    qDebug() << "[DapConnectStream] sltStreamDisconnected";
    emit streamClosed();
}

void DapStreamer::sltStreamConnected()
{
    qDebug() << "[DapConnectStream] Connected to the server";
    m_buf.clear();

    QString str_url = QString("/%1/fjskd9234j?fj913htmdgaq-d9hf=%2")
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
    m_isStreamOpened = false;
    QByteArray baReq( str_request.toLatin1() );
    qint64 ret = m_streamSocket.write(baReq.constData(), baReq.size());
    m_streamState = SSS_FRAME_SEARCH;
    if (m_streamSocket.flush())
    {
        qDebug() << "[DapConnectStream] HTTP stream request sent " << ret<< " bytes";
        qDebug() << "STREAM OPENED";
        emit streamOpened();
    }
    else
    {
        qCritical() << "Stream not opened";
        emit errorNetwork(m_streamSocket.errorString());
    }
}

void DapStreamer::sltStreamProcess()
{
   // qDebug()<< "[DapConnectStream::sltStreamProcess]";
    if( m_streamSocket.bytesAvailable() == 0 )
    {
        qWarning() << "[DapConnectStream] No data after wait!";
        return;
    }

    if( m_streamSocket.bytesAvailable() < 0 )
    {
        qWarning() << "[DapConnectStream] No data";
        return;
    }

    QByteArray btAr = m_streamSocket.readAll();
    int bufferLengthBefore = m_buf.length();
    m_buf.append(btAr);
    bool cycle_flag = true;

    while (cycle_flag)
    {
        switch (m_streamState)
        {
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
            }
            break;

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
        }
        break;

        case SSS_FRAME_HEADER:
        {
            if(m_buf.length() >= (int) sizeof(DapPacketHdr))
            {
                const DapPacketHdr* dapPktConstHdr = (DapPacketHdr* ) m_buf.constData();
                size_t dapPktSize = dapPktConstHdr->size;

                if(dapPktSize > DAP_PKT_SIZE_MAX)
                {
                    qWarning() << "[DapConnectStream] Package size too big = " << dapPktSize;
                    m_buf.clear();
                    m_streamState = SSS_FRAME_SEARCH;
                    m_dapDataPosition = 0;
                    return;
                }
                else if (dapPktSize == 0)
                {
                    switch (dapPktConstHdr->type)
                    {
                    case 0x11: //Keep-alive pkt
                    case 0x12: //Keep-alive pkt too...
                        emit isAlive(true);
                        break;
                    default:
                        //qDebug() << "Pkt type " << dapPktConstHdr->type << " unrecognized, len " << m_buf.length();
                        break;
                    }
                    m_buf.clear();
                    m_streamState = SSS_FRAME_SEARCH;
                    m_dapDataPosition = 0;
                    return;
                }

                memcpy(&m_dapPktHdr, m_buf.constData(), sizeof(DapPacketHdr));

                m_buf = m_buf.mid(sizeof(DapPacketHdr));

                if( (int)dapPktSize <= m_buf.length() )
                {
                    memcpy(m_dapData, m_buf.constData(), dapPktSize);
                    procPktIn(&m_dapPktHdr, m_dapData);
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
            }
            else
            {
                //qDebug() << "[DapConnectStream] Collecting data for DA frame header";
                cycle_flag = false;
            }
        }
        break;

        case SSS_FRAME_BODY:
        {
          if(m_buf.length() < ( (int)m_dapPktHdr.size) - m_dapDataPosition)
          {
              memcpy(m_dapData + m_dapDataPosition, m_buf.constData(), (size_t) m_buf.length());
              m_dapDataPosition += m_buf.length();
              m_buf.clear();
              cycle_flag = false;
          }
          else
          {
              size_t bytesLeft = m_dapPktHdr.size - ( (size_t)m_dapDataPosition );
              memcpy(m_dapData + m_dapDataPosition, m_buf.constData(), bytesLeft);

              procPktIn(&m_dapPktHdr, m_dapData);
              m_streamState = SSS_FRAME_SEARCH;
              m_buf = m_buf.mid(bytesLeft);
          }

        }
        break;

        default:
            qWarning() << "[DapConnectStream] Unexcepted state";
            break;
     }
   }
}

void DapStreamer::_detectPacketLoose(quint64 currentSeqId)
{
    int countLoosedPackets = currentSeqId - (m_lastSeqId + 1);
    if (countLoosedPackets > 0)
    {
        // qWarning() << "Packet Loosed count:" << countLoosedPackets;
        emit sigStreamPacketLoosed(countLoosedPackets);
        m_packetLossQueue.enqueue(qMakePair(QDateTime::currentDateTime(), countLoosedPackets));
    }
    else if((countLoosedPackets < 0) && (currentSeqId > 0))
    {
        qWarning() << "Something wrong. countLoosedPackets is " << countLoosedPackets
                   << "can't be less than zero. Current seq id:" << currentSeqId
                   << "last seq id: " << m_lastSeqId;
    }
    m_lastSeqId = currentSeqId;
    _removeOldEntries();
}

void DapStreamer::_removeOldEntries()
{
    QDateTime tenMinutesAgo = QDateTime::currentDateTime().addSecs(-600); // 10 minutes ago
    while (!m_packetLossQueue.isEmpty() && m_packetLossQueue.head().first < tenMinutesAgo)
    {
        m_packetLossQueue.dequeue();
    }
}

void DapStreamer::_printPacketLossStatistics()
{
    _removeOldEntries();
    int totalLostPackets = 0;
    for (const auto &entry : m_packetLossQueue)
    {
        totalLostPackets += entry.second;
    }
    qInfo() << "Total packet loss in the last 5 minutes:" << totalLostPackets;
}

void DapStreamer::procPktIn(DapPacketHdr * pkt, void * data)
{
    m_procPktInData.append((const char*) data, pkt->size);
    m_session->getDapCrypt()->decode(m_procPktInData, m_procPktInDecData, KeyRoleStream);

    if(m_procPktInDecData.size() > sizeof(DapChannelPacketHdr))
    {
        DapChannelPacketHdr* channelPkt = (DapChannelPacketHdr*) malloc(sizeof(DapChannelPacketHdr));
        memcpy(channelPkt, m_procPktInDecData.constData(), sizeof(DapChannelPacketHdr));
        _detectPacketLoose(channelPkt->seq_id);
        void* channelData = malloc(m_procPktInDecData.size() - sizeof(DapChannelPacketHdr) + 1);
        memcpy(channelData, m_procPktInDecData.constData() + sizeof(DapChannelPacketHdr),
               m_procPktInDecData.size() - sizeof(DapChannelPacketHdr));
        emit sigProcPacket(new DapChannelPacket(channelPkt, channelData));
    }
    else
    {
        qWarning() << "Error decode. Packet loosed";
    }

    m_procPktInData.clear();
    m_procPktInDecData.clear();
}

void DapStreamer::addChProc(char chId, DapChBase* obj)
{
    if(m_dsb.contains(chId))
    {
        qCritical() << "Proc with id" << chId << "already exists";
        return;
    }
    m_dsb.insert(chId, obj);
    connect(obj, &DapChBase::pktChOut, this, &DapStreamer::writeChannelPacket);
    connect (this, &DapStreamer::sigProcPacket, obj, &DapChBase::onPktIn);
}

