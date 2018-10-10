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

#ifndef DAPSTREAMER_H
#define DAPSTREAMER_H

#include "DapConnectStream.h"
#include "DapChannelPacket.h"
#include "DapChBase.h"
#include <QThread>
#include <QHash>

class DapChThread : public QThread //ok
{
    Q_OBJECT
public:
    DapChThread(QObject *obj) : QThread(obj) { }
    ~DapChThread() {  }
signals:
    void sigNewPkt(DapChannelPacket*);
public slots:
    void sltProcPacket(DapChannelPacket *pkt) {
        emit sigNewPkt(pkt);
    }
};


class DapStreamer : public QObject
{
    Q_OBJECT
public:
    DapStreamer(QObject *obj); //ok
    virtual ~DapStreamer() { m_streamThread->quit(); m_streamThread->wait(); } //ok
    DapChThread* addChProc(char chId, DapChBase* obj);//ok
    bool isConnected(){ return m_dapConStream->isConnected();}//ok
    int upstreamSocket() { return m_dapConStream->upstreamSocket(); }
protected:
    DapConnectStream* m_dapConStream;
    QHash<char, DapChBase*> m_dsb;
    QHash<char, DapChThread*> m_dapChThead;
    QThread* m_streamThread;

protected slots:
    void onStreamClosed()
    {
        qDebug() << "[DapStreamer] Stream closed";
        emit streamClosed();
    }

    void readChPacket(DapChannelPacketHdr* pkt, void* data); //ok

public slots:
    void writeChPacket(
        DapChannelPacketHdr* pkt,
        void* data,
        uint64_t *dest_addr = Q_NULLPTR)
    {
        emit sendChPacket(pkt,data,dest_addr);
    }

    void open(const QString& subUrl, const QString query) {
        qDebug() << "[DapStreamer] Open " << subUrl
            << "media item " << query;
        m_dapConStream->streamOpen(subUrl, query);
    }
    void openDefault(){
        open("socket_forward","sf=1");
    }

    void close();

signals:
    void errorText(QString);
    void errorNetwork(int);
    void errorAuth(int);

    void streamOpened();
    void streamClosed();

    void streamSessionRequested();
    void streamConnecting();
    void streamReconnecting();
    void streamDisconnecting();

    void streamServKeyRecieved();

    void sendChPacket(DapChannelPacketHdr* pkt, void* data, uint64_t *dest_addr = Q_NULLPTR);
};

#endif // DAPSTREAMER_H
