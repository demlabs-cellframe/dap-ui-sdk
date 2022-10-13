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

#ifndef DAPNODE_H
#define DAPNODE_H

#include <QCoreApplication>
#include <QString>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QMap>
#include <QList>
#include <QThread>
#include "DapConnectClient.h"
#include <DapCrypt.h>
#include "DapDataLocal.h"

#include "dap_client_http.h"
#include "DapNetworkAccessManager.h"
#include "DapNetworkReply.h"

enum class DapNodeErrors
{
    NetworkReplyFinishedConnectError,
    NetworkReplyConnectError,
    NetworkReplyFinishedWalletsError,
    NetworkReplyWalletsError,
    NetworkWrongReplyError,
    TimerElapsed,
    JsonError,
    StatusError
};

int DapNodeErrorCode(DapNodeErrors);

class DapNode : public QObject
{
    Q_OBJECT
private:
    static const int DEFAULT_REQUEST_TIMEOUT = 10000; // 10 sec
    const int m_requestTimeout;
    DapNetworkAccessManager * m_httpClient;
    QString m_connectId;
    QStringList m_walletsList;
    bool m_parseJsonError;

public:
    static const QString WEB3_URL;
    static const quint16 WEB3_PORT;

    DapNode(QObject * obj = Q_NULLPTR, int requestTimeout = DEFAULT_REQUEST_TIMEOUT);
    ~DapNode();

private:
    void request_GET(const QString& host,  quint16 port,
                     const QString & urlPath, DapNetworkReply &a_netReply,
                     const QString &headers = "", bool ssl = false);
    bool jsonError() { return m_parseJsonError = true; }


public slots:
    void requestWalletsList();
    void requestWalletsListStop();

protected:
    DapNetworkReply *m_connectReply;
    DapNetworkReply *m_walletsListReply;

private slots:
    void connectRequest();
    void connectReply();
    void walletsListReply();
    void replyError(DapNodeErrors errorType, const QString errorString = QString(""));
    void parseReplyConnect(const QString replyData);
    void parseReplyWallets(const QString replyData);
    void parseJsonError(QString replyData);
signals:
    void sigReceivedWalletsList(QStringList);
    void sigError(int errorCode, QString errorMessage);

};

#endif // DAPNODE_H
