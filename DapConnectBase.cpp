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


#include "DapConnectBase.h"
#include "DapSession.h"
#include <QNetworkProxy>

DapConnectBase::DapConnectBase(QObject *parent) :
    QObject(parent)
{
    http_client = new QNetworkAccessManager(this);
    http_client->setProxy(QNetworkProxy::NoProxy);
    network_reply = Q_NULLPTR;
}

QString DapConnectBase::httpAddress()
{
    return QString("http://%1:%2").arg(DapSession::getInstance()->upstreamAddress())
            .arg(DapSession::getInstance()->upstreamPort()) ;
}

QNetworkReply* DapConnectBase::request(const QString & url, QByteArray * rData)
{
    QNetworkReply * nReply;
    QNetworkRequest nRequest;
    nRequest.setUrl(QUrl(httpAddress().append(url)));

    qDebug()<< "[DapConnectBase] requests httpAddress + url " << httpAddress().append(url);

    if(!DapSession::getInstance()->cookie().isEmpty())
        nRequest.setRawHeader(QString("Cookie").toLatin1(), DapSession::getInstance()->cookie().toLatin1());

    if(!DapSession::getInstance()->sessionKeyID().isEmpty())
        nRequest.setRawHeader(QString("KeyID").toLatin1(), DapSession::getInstance()->sessionKeyID().toLatin1());

    if(rData)
    {
        nRequest.setHeader(QNetworkRequest::ContentTypeHeader,"text/plain");
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
        qDebug()<< "[DapConnectBase] Reply object is NULL";

    return nReply;
}

QNetworkReply *DapConnectBase::request(const QString &url, QString &rData)
{
    auto tmp = QByteArray(rData.toStdString().c_str());
    return request(url, &tmp);
}

void DapConnectBase::slotReadPacketFinished()
{
    qDebug() << "[DapConnectBase] Connection Read Packet finished " ;
    Q_EMIT finished();
}

void DapConnectBase::slotNetworkError(QNetworkReply::NetworkError err)
{


    qDebug() << "[DapConnectBase] Network error++: " << err;
    switch(err){
        case QNetworkReply::ConnectionRefusedError:  Q_EMIT errorText("Network error: ConnectionRefusedError");break;
        case QNetworkReply::HostNotFoundError: Q_EMIT errorText("Network error: HostNotFoundError"); break;
        case QNetworkReply::TimeoutError: Q_EMIT errorText("Network error: TimeoutError"); break;
        case QNetworkReply::TemporaryNetworkFailureError: Q_EMIT errorText("Network error: TemporaryNetworkFailureError");break;
        case QNetworkReply::NetworkSessionFailedError: Q_EMIT errorText("Network error: NetworkSessionFailedError"); break;
        case QNetworkReply::BackgroundRequestNotAllowedError: Q_EMIT errorText("Network error: BackgroundRequestNotAllowedError"); break;
        case QNetworkReply::ProxyConnectionRefusedError: Q_EMIT errorText("Network error: ProxyConnectionRefusedError"); break;
        case QNetworkReply::ProxyNotFoundError: Q_EMIT errorText("Network error: ProxyNotFoundError");break;
        case QNetworkReply::ProxyTimeoutError: Q_EMIT errorText("Network error: ProxyTimeoutError");break;
        case QNetworkReply::InternalServerError: emit errorText("Network error: InternalServerError");break;
        case QNetworkReply::ProxyAuthenticationRequiredError: Q_EMIT errorText("Network error: ProxyAuthenticationRequiredError");break;
        default: emit errorText ("UnknownServerError"); break;
    }
    if (err == QNetworkReply::InternalServerError){
        qWarning() << "Auth error";
        emit errorAuth(0);
    } else {
        qWarning() << "Network error";
        emit errorNetwork(err);
    }
}

DapConnectBase::~DapConnectBase()
{
    delete http_client;
}
