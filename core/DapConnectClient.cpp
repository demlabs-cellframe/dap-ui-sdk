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

#define HTTP_ADDRESS_URL_TEMPLATE(ssl, host, port, url) \
    QString("https://%1:%2%3").arg(host).arg(port).arg(url)

DapConnectClient::DapConnectClient(QObject *parent) :
    QObject(parent)
{
    m_httpClient = new QNetworkAccessManager(this);
    m_httpClient->setProxy(QNetworkProxy::NoProxy);
    connect(m_httpClient, &QNetworkAccessManager::finished, this, &DapConnectClient::finished);
}

void DapConnectClient::_rebuildNetworkManager()
{
    qDebug() << "Restarting NAM";
    emit sigNetworkManagerRebuild();
    delete m_httpClient;
    m_httpClient = new QNetworkAccessManager(this);
    m_httpClient->setProxy(QNetworkProxy::NoProxy);
    connect(m_httpClient, &QNetworkAccessManager::finished, this, &DapConnectClient::finished);
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

QNetworkReply* DapConnectClient::request_GET(const QString& host,  quint16 port, const QString & urlPath,
                           bool ssl, const QVector<HttpRequestHeader>* headers)
{
    QNetworkRequest req;
    if(!_buildRequest(req, host, port, urlPath, ssl, headers)) {
        return Q_NULLPTR;
    }
    auto netReply = m_httpClient->get(req);
    connect(this, &DapConnectClient::sigNetworkManagerRebuild, netReply, &QNetworkReply::abort);
    return netReply;
}

QNetworkReply* DapConnectClient::request_POST(const QString& host,  quint16 port,
                            const QString & urlPath, const QByteArray& data, bool ssl,
                            const QVector<HttpRequestHeader>* headers)
{
    QNetworkRequest req;
    if(!_buildRequest(req, host, port, urlPath, ssl, headers)) {
        return Q_NULLPTR;
    }
    auto netReply = m_httpClient->post(req, data);
    connect(this, &DapConnectClient::sigNetworkManagerRebuild, netReply, &QNetworkReply::abort);
    return netReply;
}
