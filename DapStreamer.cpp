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
#include <QDebug>

// sc == m_dapConStream
// chs == m_dsb
// chThreads == m_dapChThead

DapStreamer::DapStreamer(QObject *obj) : BaseObject(obj)
{
   qDebug() <<"[SSB] init";
   m_streamThread = new QThread;
   m_dapConStream = new DapConnectStream(nullptr);

   connect(m_dapConStream, &DapConnectStream::streamOpened, this, &DapStreamer::onStreamOpened);
   connect(m_dapConStream, &DapConnectStream::streamClosed, this, &DapStreamer::onStreamClosed);

   connect(m_dapConStream, &DapConnectStream::streamConnecting, this, &DapStreamer::streamConnecting);
   connect(m_dapConStream, &DapConnectStream::errorNetwork, this, &DapStreamer::error);

   connect(m_dapConStream, &DapConnectStream::errorNetwork,    this, &DapStreamer::errorText);
   connect(m_dapConStream, &DapConnectStream::errorAuth,    this, &DapStreamer::errorAuth);

   connect(m_dapConStream, &DapConnectStream::recivedChannelPacket, this, &DapStreamer::readChPacket);
   connect(this, &DapStreamer::sendChPacket, m_dapConStream, &DapConnectStream::writeChannelPacket);
}

void DapStreamer::close()
{
    qDebug() << "[DapStreamer] Stream closed.";
    m_dapConStream->streamClose();
}

void DapStreamer::onStreamOpened()
{
    qDebug() << "[DapStreamer] Stream Opened";
    emit streamOpened();
}//ok

void DapStreamer::readChPacket(DapChannelPacketHdr *pkt, void *data)
{
    DapChThread *dct = m_dapChThead[pkt->id];
    if (dct) {
        dct->sltProcPacket(new DapChannelPacket(pkt, data));
    } else {
        qDebug() << "[DapStreamer] Unknow channel id " << (char) pkt->id;
    }
}

DapChThread* DapStreamer::addChProc(char chId, DapChBase* obj)
{
    DapChThread * dct = new DapChThread(obj);
    m_dsb.insert(chId, obj);
    m_dapChThead.insert(chId, dct);
//    obj->passSignals(this);
    connect(obj, &DapChBase::pktChOut, this, &DapStreamer::writeChPacket);
    connect (dct, &DapChThread::sigNewPkt, obj, &DapChBase::onPktIn);
    dct->start();
    return dct;
}

