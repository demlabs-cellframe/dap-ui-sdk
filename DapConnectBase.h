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


#ifndef DAPCONNECTBASE_H
#define DAPCONNECTBASE_H

#include <QObject>
#include <QNetworkReply>
#include <QAbstractSocket>
#include <QString>
#include <QList>
#include <QBuffer>

class DapConnectBase : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void slotNetworkError(QNetworkReply::NetworkError);
    void slotReadPacketFinished();

public:
    DapConnectBase(QObject *parent = NULL);
    virtual ~DapConnectBase();

    QNetworkReply * request(const QString & url, QByteArray * rData = Q_NULLPTR);
    QNetworkReply * request(const QString & url, QString &rData);

protected:
    QNetworkAccessManager * http_client;
    QNetworkReply * network_reply;

    QByteArray byte_buffer;

    static QString httpAddress();

Q_SIGNALS:
    void finished();
    void errorText(const QString &e);
    void errorNetwork(int);
    void errorAuth(int);
    void notify(const QString&);


};

#endif // DAPCONNECTBASE_H
