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


#ifndef DapConnectClient_H
#define DapConnectClient_H

#include <QNetworkConfigurationManager>
#include <QNetworkReply>

using HttpRequestHeader = QPair<const QString, const QString>;

class DapConnectClient : public QObject
{
    Q_OBJECT
private:
    bool _buildRequest(QNetworkRequest& req, const QString& host,  quint16 port, const QString & urlPath,
                       const QVector<HttpRequestHeader>* headers);

    void _rebuildNetworkManager();

    DapConnectClient(QObject *parent = Q_NULLPTR);
    ~DapConnectClient() = default;
    DapConnectClient(const DapConnectClient&) = delete;
    DapConnectClient& operator=(const DapConnectClient&) = delete;
public:
    static DapConnectClient* instance()  {static DapConnectClient client; return &client;}

    /* Example call: request_GET("google.com", &{
     *                                           HttpRequestHeader("User-Agent", "Mozilla/5.0"),
     *                                           HttpRequestHeader("Accept", "text/html")
     *                                         }); */

    // if headers nullptr sets Content-Type Header by default "text/plain"
    QNetworkReply* request_GET(const QString& host,  quint16 port,const QString & urlPath,
                               const QVector<HttpRequestHeader>* headers = Q_NULLPTR);

    QNetworkReply* request_POST(const QString& host,  quint16 port,
                                const QString & urlPath, const QByteArray& data,
                                const QVector<HttpRequestHeader>* headers = Q_NULLPTR);

private:
    QNetworkAccessManager * m_httpClient;
    QNetworkConfigurationManager* m_netConfManager;
private slots:
    // For clear all network reply objects
    void finished(QNetworkReply *reply) { reply->deleteLater(); }
signals:
    void sigNetworkManagerRebuild();
};

#endif // DapConnectClient_H
