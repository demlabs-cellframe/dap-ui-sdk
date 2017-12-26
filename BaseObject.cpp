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


#include "BaseObject.h"

BaseObject::BaseObject(QObject *parent) : QObject(parent)
{

}

void BaseObject::connectSignals(QObject * rcv)
{
    connect(this,SIGNAL(error(QString)),rcv, SLOT(raiseErrorDialog(QString)));
    connect(this,SIGNAL(notify(QString)),rcv, SLOT(raiseNotifyDialog(QString)));
    connect(this,SIGNAL(status(QString,int)),rcv,SLOT(onStatusUpdate(QString,int)));
    connect(this,SIGNAL(progressOn(QString)),rcv,SLOT(raiseProgressDialog(QString)));
    connect(this,SIGNAL(progressOff()),rcv,SLOT(hideProgressDialog()));
    connect(this,SIGNAL(progressUpdate(int)),rcv,SLOT(updateProgress(int)));
}


void BaseObject::passSignals(BaseObject * rcv)
{
    connect(this,SIGNAL(error(QString)),rcv, SIGNAL(error(QString)));
    connect(this,SIGNAL(notify(QString)),rcv, SIGNAL(notify(QString)));
    connect(this,SIGNAL(status(QString,int)),rcv,SIGNAL(status(QString,int)));
    connect(this,SIGNAL(progressOn(QString)),rcv,SIGNAL(progressOn(QString)));
    connect(this,SIGNAL(progressOff()),rcv,SIGNAL(progressOff()));
    connect(this,SIGNAL(progressUpdate(int)),rcv,SIGNAL(progressUpdate(int)));
}
