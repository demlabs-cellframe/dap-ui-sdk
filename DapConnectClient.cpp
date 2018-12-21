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
#include "DapSession.h"
#include <QNetworkProxy>
#include <QNetworkConfiguration>


#define HTTP_ADDRESS_URL_TEMPLATE(host, port, url) \
    QString("http://%1:%2%3").arg(host).arg(port).arg(url)

DapConnectClient::DapConnectClient(QObject *parent) :
    QObject(parent)
{
    m_httpClient = new QNetworkAccessManager(this);
    m_httpClient->setProxy(QNetworkProxy::NoProxy);
    m_defaultNetworkConfig = new QNetworkConfiguration(m_httpClient->activeConfiguration());

    connect(m_httpClient, &QNetworkAccessManager::finished, this, &DapConnectClient::finished);
}

void DapConnectClient::saveCurrentNetConf()
{
    if(m_defaultNetworkConfig != Q_NULLPTR)
        delete m_defaultNetworkConfig;
    qDebug() << "Save default configuration name:" << m_httpClient->configuration().name();
    m_defaultNetworkConfig = new QNetworkConfiguration(m_httpClient->configuration());
}

void DapConnectClient::restoreDefaultNetConf()
{
    qDebug() << "Restore default configuration name: " << m_defaultNetworkConfig->name();
    m_httpClient->setConfiguration(*m_defaultNetworkConfig);
}

void DapConnectClient::_rebuildNetworkManager()
{
    delete m_httpClient;
    m_httpClient = new QNetworkAccessManager(this);
    m_httpClient->setProxy(QNetworkProxy::NoProxy);
}

bool DapConnectClient::_buildRequest(QNetworkRequest& req, const QString& host,
                                     quint16 port, const QString & urlPath,
                                     const QVector<HttpRequestHeader>& headers)
{
    if(m_httpClient->networkAccessible() == QNetworkAccessManager::NotAccessible) {
        _rebuildNetworkManager();
    }

    QString httpAddress = HTTP_ADDRESS_URL_TEMPLATE(host, port, urlPath);
    qDebug()<< "Requests httpAddress + url " << httpAddress;

    req.setUrl(httpAddress);
    if(!req.url().isValid()) {
        qCritical() << "Bad URL";
        return false;
    }

    for(const auto& header : headers) {
        req.setRawHeader(header.first.toLatin1(), header.second.toLatin1());
    }

    return true;
}

QNetworkReply* DapConnectClient::request_GET(const QString& host,  quint16 port, const QString & urlPath)
{
    return request_GET(host, port, urlPath, {
                            HttpRequestHeader("Content-Type","text/plain")
                        });
}

QNetworkReply* DapConnectClient::request_GET(const QString& host,  quint16 port, const QString & urlPath,
                           const QVector<HttpRequestHeader>& headers)
{
    QNetworkRequest req;
    if(!_buildRequest(req, host, port, urlPath, headers)) {
        return Q_NULLPTR;
    }
    return m_httpClient->get(req);
}

QNetworkReply* DapConnectClient::request_POST(const QString& host,  quint16 port,
                                              const QString & urlPath, const QByteArray& data)
{
    return request_POST(host, port, urlPath, data, {
                            HttpRequestHeader("Content-Type","text/plain")
                        });
}

QNetworkReply* DapConnectClient::request_POST(const QString& host,  quint16 port,
                            const QString & urlPath, const QByteArray& data,
                            const QVector<HttpRequestHeader>& headers)
{
    QNetworkRequest req;
    if(!_buildRequest(req, host, port, urlPath, headers)) {
        return Q_NULLPTR;
    }
    return m_httpClient->post(req, data);
}

//void DapConnectClient::slotNetworkError(QNetworkReply::NetworkError err)
//{
//    qWarning() << err;
//    switch(err) {
//        case QNetworkReply::AuthenticationRequiredError:
//            emit authenticationRequiredError();
//        case QNetworkReply::ConnectionRefusedError:  Q_EMIT errorNetwork("Network error: ConnectionRefusedError");break;
//        case QNetworkReply::HostNotFoundError: Q_EMIT errorNetwork("Network error: HostNotFoundError"); break;
//        case QNetworkReply::TimeoutError: Q_EMIT errorNetwork("Network error: TimeoutError"); break;
//        case QNetworkReply::TemporaryNetworkFailureError: Q_EMIT errorNetwork("Network error: TemporaryNetworkFailureError");break;
//        case QNetworkReply::NetworkSessionFailedError: Q_EMIT errorNetwork("Network error: NetworkSessionFailedError"); break;
//        case QNetworkReply::BackgroundRequestNotAllowedError: Q_EMIT errorNetwork("Network error: BackgroundRequestNotAllowedError"); break;
//        case QNetworkReply::ProxyConnectionRefusedError: Q_EMIT errorNetwork("Network error: ProxyConnectionRefusedError"); break;
//        case QNetworkReply::ProxyNotFoundError: Q_EMIT errorNetwork("Network error: ProxyNotFoundError");break;
//        case QNetworkReply::ProxyTimeoutError: Q_EMIT errorNetwork("Network error: ProxyTimeoutError");break;
//        case QNetworkReply::InternalServerError: emit errorNetwork("Network error: InternalServerError");break;
//        case QNetworkReply::ProxyAuthenticationRequiredError: Q_EMIT errorNetwork("Network error: ProxyAuthenticationRequiredError");break;
//        default: emit errorNetwork ("UnknownServerError"); break;
//    }
//}
