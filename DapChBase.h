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

#ifndef DAPSACHBASE_H
#define DAPSACHBASE_H

#include "DapChannelPacket.h"

class DapChBase : public QObject
{
    Q_OBJECT
public:
    DapChBase(QObject *obj, char id) : QObject(obj), m_id(id) { }
    char get_id() { return m_id; }

protected:
    char m_id;

protected slots:
    void write_str(char, const QString&);

signals:
    void pktChOut(DapChannelPacketHdr* pkt, void* data, uint64_t* dest_addr = NULL);
    void error(const QString& msg);
public slots:
    virtual void onPktIn(DapChannelPacket* pkt) = 0;
};

#endif // DAPSACHBASE_H
