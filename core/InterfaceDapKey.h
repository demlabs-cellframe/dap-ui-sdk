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


#ifndef INTERFACEDAPKEY_H
#define INTERFACEDAPKEY_H

#include <QByteArray>


class DapKeyAbstract
{
public:
    DapKeyAbstract() { }
    virtual ~DapKeyAbstract() { }

    virtual void encode(const QByteArray& in, QByteArray& out) = 0;
    virtual void decode(const QByteArray& in, QByteArray& out) = 0;
    virtual bool init(const QString& key) {(void)key; return false;}
};

#endif
