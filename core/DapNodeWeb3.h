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


//int DapNodeErrorCode(DapNodeErrors, const bool httpFinished);
class DapNodeWeb3;

struct ReplyMethod
{
    QString request;
    void(DapNodeWeb3::*parseMethod)(const QString&, int baseErrorCode);
    void(DapNodeWeb3::*replyError)(int error);
    QString wrongReplyText;
    int baseErrorCode;
};


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

    void responseParsing(const int error, const QString wrongReplyerrorString, const bool httpFinished,
                         int errorCode, QString messageReplyDataError,
                         void(DapNodeWeb3::*parseMethod)(const QString&, int baseErrorCode),
                         void(DapNodeWeb3::*responceError)(int code));

    static const QList<ReplyMethod> replyItems;

public slots:
    // requests
    void nodeDetectedRequest();
    void nodeConnectionRequest();
    void nodeStatusRequest();
    void walletsRequest();
    void networksRequest();
    void walletDataRequest(const QString& walletName);
    void getCertificates();
    void createCertificate(const QString& certType, const QString& certName);
    void condTxCreateRequest(QString walletName, QString networkName, QString sertificateName, QString tokenName, QString value, QString unit, QString fee);
    void getMempoolTxHashRequest(QString transactionHash, QString networkName);
    void getLedgerTxHashRequest(QString transactionHash, QString networkName);
    void getOrdersListRequest(QString networkName, QString tokenName, QString minPrice, QString maxPrice, QString unit);
    void nodeDumpRequest(QString networkName);
    void getNodeIPRequest(QString networkName, QString nodeAddr);
    void getFeeRequest(QString networkName);
    void getListKeysRequest(QString networkName);
    void getNetIdRequest(QString networkName);


private slots:
    // send request string
    void sendRequest(QString request);
    // reply error
    void replyError(int errorCode, const QString errorString);
    // response processing
    void parseReplyStatus(const QString& replyData, int baseErrorCode);
    void parseReplyConnect(const QString& replyData, int baseErrorCode);
    void parseReplyNetworks(const QString& replyData, int baseErrorCode);
    void parseReplyWallets(const QString& replyData, int baseErrorCode);
    void parseDataWallet(const QString& replyData, int baseErrorCode);
    void parseCertificates(const QString& replyData, int baseErrorCode);
    void parseCreateCertificate(const QString& replyData, int baseErrorCode);
    void parseCondTxCreateReply(const QString& replyData, int baseErrorCode);
    void parseLedgerReply(const QString& replyData, int baseErrorCode);
    void parseMempoolReply(const QString& replyData, int baseErrorCode);
    void parseOrderList(const QString& replyData, int baseErrorCode);
    void parseJsonError(const QString& replyData, int baseErrorCode);
    void parseNodeIp(const QString& replyData, int baseErrorCode);
    void parseFee(const QString& replyData, int baseErrorCode);
    void parseNodeDump(const QString& replyData, int baseErrorCode);
    void parseListKeys(const QString& replyData, int baseErrorCode);
    void parseNetId(const QString& replyData, int baseErrorCode);
    //
    void replyConnectError(int code);

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
    void sigOrderList(QJsonArray);
    void sigNodeIp(QString nodeIp);
    void sigNetId(QString netId);
    void sigFee(QString fee);
    void connectionIdReceived(QString connectionId);
    void sigNodeDump(QList<QMap<QString, QString>> nodeDump);
    void sigListKeys(QList<QString> listKeys);
    void statusOk();
    void nodeDetected();
    void nodeNotDetected();
    void nodeNotConnected();
    void checkNodeStatus();
    void sigIncorrectId();
};

#endif // DAPNODEWEB3_H
