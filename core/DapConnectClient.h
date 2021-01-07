/*
* Authors:
* Dmitriy Gerasimov <naeper@demlabs.net>
* Cellframe       https://cellframe.net
* DeM Labs Inc.   https://demlabs.net
* Copyright  (c) 2017-2019
* All rights reserved.

This file is part of CellFrame SDK the open source project

CellFrame SDK is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

CellFrame SDK is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with any CellFrame SDK based project.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>

#include "DapNetworkAccessManager.h"
#include "DapNetworkReply.h"
using HttpRequestHeader = QPair<const QString, const QString>;

class DapConnectClient : public QObject
{
    Q_OBJECT
private:
    bool _buildRequest(QNetworkRequest& req, const QString& host,
                       quint16 port, const QString & urlPath, bool ssl,
                       const QVector<HttpRequestHeader>* headers);

    DapConnectClient(QObject *parent = Q_NULLPTR);
    ~DapConnectClient() = default;
    DapConnectClient(const DapConnectClient&) = delete;
    DapConnectClient& operator=(const DapConnectClient&) = delete;
public:
    DapNetworkAccessManager * m_httpClient;
    static DapConnectClient* instance()  {static DapConnectClient client; return &client;}

    /* Example call: request_GET("google.com", &{
     *                                           HttpRequestHeader("User-Agent", "Mozilla/5.0"),
     *                                           HttpRequestHeader("Accept", "text/html")
     *                                         }); */

    // if headers nullptr sets Content-Type Header by default "text/plain"
    void request_GET(const QString& host,  quint16 port,
                               const QString & urlPath, DapNetworkReply &a_netReply, const QString &headers = "", bool ssl = false);

    void request_POST(const QString& host,  quint16 port,
                                const QString & urlPath, const QByteArray& data, DapNetworkReply &a_netReply, const QString &headers = "", bool ssl = false);

    void abortRequests() {
        for (auto &l_rpl : m_httpClient->findChildren<QNetworkReply*>()) {
            if (!l_rpl->isFinished()) {
                qDebug() << "Abort request " << l_rpl->url().toString();
                l_rpl->abort();
            }
        }
    }
    void _rebuildNetworkManager();
};
