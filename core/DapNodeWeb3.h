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

#ifndef DAPNODEWEB3_H
#define DAPNODEWEB3_H

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
    NetworkReplyConnectError,
    NetworkReplyStatusError,
    NetworkReplyWalletsError,
    NetworkReplyNetworksListError,
    NetworkWrongReplyError,
    TimerElapsed,
    JsonError,
    StatusError
};

int DapNodeErrorCode(DapNodeErrors, const bool httpFinished);

class DapNodeWeb3 : public QObject
{
    Q_OBJECT
private:
    const int m_requestTimeout;
    // http access manager
    DapNetworkAccessManager * m_httpClient;
    // cellframe dashboard connect id
    QString m_connectId;
    // reply data
    DapNetworkReply *m_networkReply;
    // network request string
    QString m_networkRequest;
    //
    bool m_parseJsonError;

public:
    static const int DEFAULT_REQUEST_TIMEOUT = 10000; // 10 sec
    static const QString WEB3_URL;
    static const quint16 WEB3_PORT;

    DapNodeWeb3(QObject * obj = Q_NULLPTR, int requestTimeout = DEFAULT_REQUEST_TIMEOUT);
    ~DapNodeWeb3();
    QString connectedId() { return m_connectId; }

private:
    void request_GET(const QString& host,  quint16 port,
                     const QString & urlPath, DapNetworkReply &a_netReply,
                     const QString &headers = "", bool ssl = false);
    bool jsonError() { return m_parseJsonError; }
    void responseProcessing(const int error, const QString errorString = "", const bool httpFinished = true);

    void responseParsing(const int error, DapNodeErrors errorType, const QString errorString, const bool httpFinished,
                DapNodeErrors getReplyDataError, QString messageReplyDataError,
                void(DapNodeWeb3::*parseMethod)(const QString&));

public slots:
    // requests
    void nodeDetectedRequest();
    void nodeConnectionRequest();
    void connectReply();
    void nodeStatusRequest();
    void walletsRequest();
    void networksRequest();
    void walletDataRequest(const QString& walletName);
    void getCertificates();
    void createCertificate(const QString& certType, const QString& certName);
    void condTxCreateRequest(QString walletName, QString networkName, QString sertificateName, QString tokenName, QString value, QString unit);
    void getMempoolTxHashRequest(QString transactionHash, QString networkName);
    void getLedgerTxHashRequest(QString transactionHash, QString networkName);


private slots:
    // send request string
    void sendRequest(QString request);
    // reply error
    void replyError(DapNodeErrors errorType, const QString errorString = QString(""), const bool httpFinished = true);
    // reply
    void nodeStatusOkReply(const QString& replyData);
    void parseReplyConnect(const QString& replyData);
    void parseReplyNetworks(const QString& replyData);
    void parseReplyWallets(const QString& replyData);
    void parseDataWallet(const QString& replyData);
    void parseCertificates(const QString& replyData);
    void parseCreateCertificate(const QString& replyData);
    void parseCondTxCreateReply(const QString& replyData);
    void parseLedgerReply(const QString& replyData);
    void parseMempoolReply(const QString& replyData);
    void parseJsonError(QString replyData);

signals:
    // -------- output signals --------
    void sigError(int errorCode, QString errorMessage);
    void sigReceivedWalletsList(QStringList);
    void sigReceivedNetworksList(QStringList);
    void sigWalletDataReady(QJsonArray);
    void sigReceivedCertificatestList(QStringList);
    void sigCreatedCertificate(QString);
    void sigCondTxCreateSuccess(QString hash);
    void sigMempoolContainHash();
    void sigLedgerContainHash();
    void connectionIdReceived(QString connectionId);
    void statusOk();
    void nodeDetected();
    void nodeNotDetected();
    void nodeNotConnected();
    void checkNodeStatus();
};



//{
//"data": [
//    {
//        "address": "mWNv7A43YnqRHCWVJG2zobGRUWepgMs5b2K2Hq4w7QePxDXoy1VArS2vhdyAxp5cSR1Q2qUYQDkYQs4uFxr7TP3WnJzzTWa2iGFdDDv7",
//        "network": "mileena",
//        "tokens": [
//            {
//                "balance": "0",
//                "datoshi": "0",
//                "tokenName": "0"
//            }
//        ]
//    },
//    {
//        "address": "mJUUJk6Yk2gBSTjcDvqqLvGTgcNFZeZbCmrD3Mb1QycVPZmZcUVmH9WW8aLixh6M7XfzLWYGbSGSGwMSV6PiVwcLnbjZvoAfmQEfqEsN",
//        "network": "subzero",
//        "tokens": [
//            {
//                "balance": "0",
//                "datoshi": "0",
//                "tokenName": "0"
//            }
//        ]
//    },
//    {
//        "address": "Rj7J7MiX2bWy8sNyaJdTGtjsUzwUbDHsfT3EnKN4Nhcmcf2bRxgx5GoXxgR6q3YDu8s64PGvbbHhh21jSgns6tjTXy6ADf4ga1XdnJnk",
//        "network": "Backbone",
//        "tokens": [
//            {
//                "balance": "0",
//                "datoshi": "0",
//                "tokenName": "0"
//            }
//        ]
//    },
//    {
//        "address": "iDAeSXFFiwH2LyhqupWKEHqfCbSQFvTXoPm4bU9VyboenWTxeviL5AjVo54dXCbGctryNgFiV91UcD8hmMaNChgyGuQusikS5W9dUYGA",
//        "network": "kelvpn-minkowski",
//        "tokens": [
//            {
//                "balance": "0",
//                "datoshi": "0",
//                "tokenName": "0"
//            }
//        ]
//    }
//],
//"errorMsg": "",
//"status": "ok"
//}


#endif // DAPNODEWEB3_H
