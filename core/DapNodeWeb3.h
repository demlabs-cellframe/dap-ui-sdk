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

/* INCLUDES */

#include <QCoreApplication>
#include <QString>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QMap>
#include <QList>
#include <QThread>
#include "DapConnectClient.h"
#include <DapCrypt.h>
#include "DapBaseDataLocal.h"

#include "dap_client_http.h"
#include "DapNetworkAccessManager.h"
#include "DapNetworkReply.h"


//int DapNodeErrorCode(DapNodeErrors, const bool httpFinished);

class DapNodeWeb3 : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum class ReplyMethodID : quint16
  {
    Invalid,
    ParseReplyConnect,
    ParseReplyStatus,
    ParseReplyWallets,
    ParseReplyNetworks,
    ParseDataWallet,
    ParseCertificates,
    ParseCreateCertificate,
    ParseCondTxCreateReply,
    ParseMempoolReply,
    ParseLedgerReply,
    ParseOrderList,
    ParseNodeIp,
    ParseFee,
    ParseNodeList,
    ParseListKeys,
    ParseNetId
  };
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  const int m_requestTimeout;
  // http access manager
  DapNetworkAccessManager *m_httpClient;
  // cellframe dashboard connect id
  QString m_connectId;

  QString m_networkName;
  // reply data
  DapNetworkReply *m_networkReply;
  // network request string
  QString m_networkRequest;
  //
  bool m_parseJsonError = false;
  // reconnection attempts counter to prevent infinite loops
  int m_reconnectionAttempts = 0;
  static const int MAX_RECONNECTION_ATTEMPTS = 3;

public:
  static const int DEFAULT_REQUEST_TIMEOUT = 10000; // 10 sec
  static const QString WEB3_URL;
  static const quint16 WEB3_PORT;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapNodeWeb3 (QObject *obj = Q_NULLPTR, int requestTimeout = DEFAULT_REQUEST_TIMEOUT);
  ~DapNodeWeb3();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  QString connectedId() { return m_connectId; }
  QString networkName() { return m_networkName; }

private:
  void request_GET (const QString &host,  quint16 port,
                    const QString &urlPath, DapNetworkReply &a_netReply,
                    const QString &headers = "", bool ssl = false);
  void request_GET_long_timeout (const QString &host,  quint16 port,
                    const QString &urlPath, DapNetworkReply &a_netReply,
                    const QString &headers = "", bool ssl = false);
  bool jsonError() { return m_parseJsonError; }
  void responseProcessing (const int error, const QString errorString = "", const bool httpFinished = true);

  void responseParsing (
    const int error,
    const QString wrongReplyerrorString,
    const bool httpFinished,
    int baseErrorCode,
    QString messageReplyDataError,
    ReplyMethodID parseMethod,
    bool responceError);
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  // requests
  void nodeDetectRequest();
  void nodeDetectedRequest();
  void nodeVersionsRequest();
  void nodeConnectionRequest();
  void nodeStatusRequest();
  void walletsRequest();
  void networksRequest();
  void walletDataRequest (const QString &walletName);
  void getCertificates();
  void createCertificate (const QString &certType, const QString &certName);
      void condTxCreateRequest (QString walletName, QString networkName, QString sertificateName, QString tokenName, QString value, QString unit);
  void getMempoolTxHashRequest (QString transactionHash, QString networkName);
  void getLedgerTxHashRequest (QString transactionHash, QString networkName);
  void getOrdersListRequest (QString networkName, QString tokenName, QString minPrice, QString maxPrice, QString unit);
  void nodeListRequest (QString networkName);
  void getNodeIPRequest (const QString &networkName, const QJsonArray &orderList);
  void getFeeRequest (QString networkName);
  void getNetIdRequest (QString networkName);
  void getListKeysRequest (QString networkName);

private slots:
  // send request string
  void sendRequest (QString request);

  // reply error
  void replyError (int errorCode, const QString &errorString, const QString &errorGuiMessage);
  void parseJson (const QString &replyData, int baseErrorCode, const QString &a_replyName, QJsonDocument *a_destDoc = nullptr);

  // response processing
  void parseReplyStatus (const QString &replyData, int baseErrorCode);
  void parseReplyConnect (const QString &replyData, int baseErrorCode);
  void parseReplyNetworks (const QString &replyData, int baseErrorCode);
  void parseReplyWallets (const QString &replyData, int baseErrorCode);
  void parseDataWallet (const QString &replyData, int baseErrorCode);
  void parseCertificates (const QString &replyData, int baseErrorCode);
  void parseCreateCertificate (const QString &replyData, int baseErrorCode);
  void parseCondTxCreateReply (const QString &replyData, int baseErrorCode);
  void parseLedgerReply (const QString &replyData, int baseErrorCode);
  void parseMempoolReply (const QString &replyData, int baseErrorCode);
  void parseOrderList (const QString &replyData, int baseErrorCode);
  void parseNodeIp (const QString &replyData, int baseErrorCode);
  void parseFee (const QString &replyData, int baseErrorCode);
  void parseNodeList (const QString &replyData, int baseErrorCode);
  void parseListKeys (const QString &replyData, int baseErrorCode);
  void parseNetId (const QString &replyData, int baseErrorCode);

  void replyConnectError (int code);

  // Validation helpers
  bool isValidIPAddress(const QString& ip) const;

  // Dashboard diagnostics
  void requestNodeListForDiagnostics(const QString &networkName);
  void diagnoseDashboardNodeAvailability(const QString &networkName, const QJsonArray &failedNodes);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  // -------- output signals --------
  void sigError (int errorCode, QString errorMessage);
  void sigReceivedWalletsList (QStringList);
  void sigReceivedNetworksList (QStringList);
  void sigWalletDataReady (QJsonArray);
  void sigReceivedCertificatestList (QStringList);
  void sigCreatedCertificate (QString);
  void sigCondTxCreateSuccess (QString hash);
  void sigMempoolContainHash();
  void sigLedgerContainHash();
  void sigOrderList (QJsonArray);
  void sigNodeIp (QJsonObject);
  void sigNetId (QString netId);
  void sigFee (QString fee);
  void sigFeeData (QJsonObject);
  void connectionIdReceived (QString connectionId);
  void sigNodeList (QList<QMap<QString, QString>> nodeList);
  void sigListKeys (QList<QString> listKeys);
  void statusOk();
  void nodeDetected();
  void nodeNotDetected();
  void nodeNotConnected();
  void checkNodeStatus();
  void sigIncorrectId();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPNODEWEB3_H
