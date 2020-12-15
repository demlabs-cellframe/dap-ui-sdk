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


#include "DapConnectClient.h"
#include <QNetworkProxy>
#include "dap_client_http.h"

#define HTTP_ADDRESS_URL_TEMPLATE(ssl, host, port, url) \
    QString("https://%1:%2%3").arg(host).arg(port).arg(url)

DapConnectClient::DapConnectClient(QObject *parent) :
    QObject(parent)
{
    m_httpClient = new DapNetworkAccessManager();
//    m_httpClient->setProxy(QNetworkProxy::NoProxy);
  //  connect(m_httpClient, &DapNetworkAccessManager::finished, this, &DapConnectClient::finished); //todo_m: связать сигнал завершения менеджера с коллбеком
}

void DapConnectClient::_rebuildNetworkManager()
{
    qDebug() << "Restarting NAM";
    abortRequests();
    m_httpClient->deleteLater();
    m_httpClient = new DapNetworkAccessManager();
//    m_httpClient->setProxy(QNetworkProxy::NoProxy);
//    connect(m_httpClient, &DapNetworkAccessManager::finished, this, &DapConnectClient::finished); //todo_m: связать сигнал завершения менеджера с коллбеком
}

bool DapConnectClient::_buildRequest(QNetworkRequest& req, const QString& host,
                                     quint16 port, const QString & urlPath, bool ssl,
                                     const QVector<HttpRequestHeader>* headers)
{
    QString httpAddress = QString("%1://%2:%3%4").arg(ssl ? "https" : "http")
            .arg(host).arg(port).arg(urlPath);

    qDebug()<< "Requests httpAddress + url " << httpAddress;

    req.setUrl(httpAddress);
    if(!req.url().isValid()) {
        qCritical() << "Bad URL";
        return false;
    }

    if(headers != Q_NULLPTR) {
        for(const auto& header : *headers) {
            req.setRawHeader(header.first.toLatin1(), header.second.toLatin1());
        }
    } else {
        req.setRawHeader("Content-Type","text/plain");
    }

    return true;
}

void DapConnectClient::request_GET(const QString& host,  quint16 port, const QString & urlPath,
                           DapNetworkReply &a_netReply, bool ssl)
{
    m_httpClient->requestHttp_GET(host, port, urlPath, &a_netReply);
}

void DapConnectClient::request_POST(const QString& host,  quint16 port,
                            const QString & urlPath, const QByteArray& data, DapNetworkReply &a_netReply, bool ssl)
{
    m_httpClient->requestHttp_POST(host, port, urlPath, data, &a_netReply);
}
