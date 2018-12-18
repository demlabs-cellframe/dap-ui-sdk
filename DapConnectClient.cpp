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

DapConnectClient::DapConnectClient(QObject *parent) :
    QObject(parent)
{
    http_client = new QNetworkAccessManager(this);
    http_client->setProxy(QNetworkProxy::NoProxy);
    network_reply = Q_NULLPTR;
    defaultNetworkConfig = new QNetworkConfiguration(http_client->activeConfiguration());
}

QString DapConnectClient::httpAddress()
{
    return QString("http://%1:%2").arg(DapSession::getInstance()->upstreamAddress())
            .arg(DapSession::getInstance()->upstreamPort()) ;
}

void DapConnectClient::saveCurrentNetConf()
{
    if(defaultNetworkConfig != Q_NULLPTR)
        delete defaultNetworkConfig;
    qDebug() << "Save default configuration name:" << http_client->configuration().name();
    defaultNetworkConfig = new QNetworkConfiguration(http_client->configuration());
}

void DapConnectClient::restoreDefaultNetConf()
{
    qDebug() << "Restore default configuration name: " << defaultNetworkConfig->name();
    http_client->setConfiguration(*defaultNetworkConfig);
}

void DapConnectClient::rebuildNetworkManager()
{
    delete http_client;
    http_client = new QNetworkAccessManager(this);
    http_client->setProxy(QNetworkProxy::NoProxy);
}


QNetworkReply* DapConnectClient::request(const QString& domain, const QString & url, QByteArray * rData)
{
    if(http_client->networkAccessible() == QNetworkAccessManager::NotAccessible) {
        rebuildNetworkManager();
    }

    QNetworkReply * nReply;
    QNetworkRequest nRequest;
    nRequest.setUrl(QUrl(httpAddress().append(url)));
    qDebug()<< "[DapConnectClient] requests httpAddress + url " << httpAddress().append(url);

    if(!DapSession::getInstance()->cookie().isEmpty())
        nRequest.setRawHeader(QString("Cookie").toLatin1(), DapSession::getInstance()->cookie().toLatin1());

    if(!DapSession::getInstance()->sessionKeyID().isEmpty())
        nRequest.setRawHeader(QString("KeyID").toLatin1(), DapSession::getInstance()->sessionKeyID().toLatin1());

    if(rData)
    {
        nRequest.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");
        nReply = http_client->post(nRequest,*rData);
    }
    else
        nReply = http_client->get(nRequest);

    if(nReply)
    {
        connect(nReply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(slotNetworkError(QNetworkReply::NetworkError)));
        connect(nReply, SIGNAL (finished()),this, SLOT(slotReadPacketFinished()));
    }
    else
        qDebug()<< "[DapConnectClient] Reply object is NULL";

    return nReply;
}

void DapConnectClient::slotReadPacketFinished()
{
    qDebug() << "[DapConnectClient] Connection Read Packet finished " ;
    Q_EMIT finished();
}

void DapConnectClient::slotNetworkError(QNetworkReply::NetworkError err)
{
    qWarning() << err;
    switch(err) {
        case QNetworkReply::AuthenticationRequiredError:
            emit authenticationRequiredError();
        case QNetworkReply::ConnectionRefusedError:  Q_EMIT errorNetwork("Network error: ConnectionRefusedError");break;
        case QNetworkReply::HostNotFoundError: Q_EMIT errorNetwork("Network error: HostNotFoundError"); break;
        case QNetworkReply::TimeoutError: Q_EMIT errorNetwork("Network error: TimeoutError"); break;
        case QNetworkReply::TemporaryNetworkFailureError: Q_EMIT errorNetwork("Network error: TemporaryNetworkFailureError");break;
        case QNetworkReply::NetworkSessionFailedError: Q_EMIT errorNetwork("Network error: NetworkSessionFailedError"); break;
        case QNetworkReply::BackgroundRequestNotAllowedError: Q_EMIT errorNetwork("Network error: BackgroundRequestNotAllowedError"); break;
        case QNetworkReply::ProxyConnectionRefusedError: Q_EMIT errorNetwork("Network error: ProxyConnectionRefusedError"); break;
        case QNetworkReply::ProxyNotFoundError: Q_EMIT errorNetwork("Network error: ProxyNotFoundError");break;
        case QNetworkReply::ProxyTimeoutError: Q_EMIT errorNetwork("Network error: ProxyTimeoutError");break;
        case QNetworkReply::InternalServerError: emit errorNetwork("Network error: InternalServerError");break;
        case QNetworkReply::ProxyAuthenticationRequiredError: Q_EMIT errorNetwork("Network error: ProxyAuthenticationRequiredError");break;
        default: emit errorNetwork ("UnknownServerError"); break;
    }
}

DapConnectClient::~DapConnectClient()
{
    if(defaultNetworkConfig != Q_NULLPTR)
        delete defaultNetworkConfig;
    delete http_client;
}
