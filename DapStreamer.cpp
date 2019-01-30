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

DapStreamer::DapStreamer(DapSession* mainSession, QObject *obj) : DapConnectStream(mainSession, obj)
{

}

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
    if(m_dsb.contains(chId)) {
        qCritical() << "Proc with id" << chId << "already exists";
        return Q_NULLPTR;
    }

    DapChThread * dct = new DapChThread(obj);
    m_dsb.insert(chId, obj);
    m_dapChThead.insert(chId, dct);
//    obj->passSignals(this);
    connect(obj, &DapChBase::pktChOut, this, &DapStreamer::writeChannelPacket);
    connect (dct, &DapChThread::sigNewPkt, obj, &DapChBase::onPktIn);
    dct->start();
    return dct;
}

