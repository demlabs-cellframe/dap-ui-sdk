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

void DapConnectClient::request_GET(const QString& host,  quint16 port, const QString & urlPath,
                           DapNetworkReply &a_netReply, const QString& headers, bool /*ssl*/)
{
    m_httpClient->requestHttp_GET(host, port, urlPath, headers, a_netReply);
}

void DapConnectClient::request_GET_for_ping(const QString& host,  quint16 port, const QString & urlPath,
                                   DapNetworkReply &a_netReply, const QString& headers, bool /*ssl*/)
{
    m_httpClient->requestHttp_GET_for_ping(host, port, urlPath, headers, a_netReply);
}

void DapConnectClient::request_POST(const QString& host,  quint16 port,
                            const QString & urlPath, const QByteArray& data, DapNetworkReply &a_netReply, const QString& headers, bool /*ssl*/)
{
    m_httpClient->requestHttp_POST(host, port, urlPath, data, headers, a_netReply);
}
