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


#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <QObject>

class BaseObject : public QObject
{
    Q_OBJECT
public:
    explicit BaseObject(QObject *parent = 0);
public Q_SLOTS:
    void connectSignals(QObject * rcv);
    void passSignals(BaseObject * rcv);
signals:
    void status(const QString& str, int timeout = 10000);
    void error(const QString& msg);
    void notify(const QString& msg);
    void progressUpdate(int value);
    void progressOn(const QString & msg = "Progress");
    void progressOff();
};

#endif // BASEOBJECT_H
