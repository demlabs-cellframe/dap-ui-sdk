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

#ifndef DAPSESSION_H
#define DAPSESSION_H

#include <QCoreApplication>
#include <QString>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QMap>
#include <QList>
#include "DapConnectClient.h"
#include <DapCrypt.h>
#include "DapBaseDataLocal.h"

#include "dap_client_http.h"
#include "DapNetworkAccessManager.h"
#include "DapNetworkReply.h"


class DapSession : public QObject
{
    Q_OBJECT
private:
    static const int DEFAULT_REQUEST_TIMEOUT = 10000; // 10 sec
    const int m_requestTimeout;
    int m_enc_type;
    size_t m_enc_size;
    int m_pkey_exch_type;

public:
    static const QString URL_ENCRYPT;
    static const QString URL_STREAM;
    static const QString URL_DB;
    static const QString URL_CTL;
    static const QString URL_DB_FILE;
    static const QString URL_SERVER_LIST;
    static const QString URL_TX;
    static const QString URL_NEW_TX;
    static const QString URL_BUG_REPORT;
    static const QString URL_NEWS;
    static const QString URL_SIGN_UP;
    static const QString URL_BUG_REPORTS_STATUS;
#ifdef BUILD_VAR_GOOGLE
    static const QString URL_VERIFY_PURCHASE;
#endif

    DapSession(QObject * obj = Q_NULLPTR, int requestTimeout = DEFAULT_REQUEST_TIMEOUT);
    ~DapSession();

    void setUserAgent(const QString& userAgent);

    const QString& upstreamAddress()     { return m_upstreamAddress;       }
    quint16 upstreamPort()               { return m_upstreamPort;          }
    const QString& cookie()              { return m_cookie;                }
    const QString& sessionKeyID()        { return m_sessionKeyID;          }
    const QString& user()                { return m_user;                  }


    const QString& cdbAuthTxCond(){ return  m_cdbAuthTxCond; }
    const QString& cdbAuthNet(){ return  m_cdbAuthNet; }
    const QString& cdbAuthToken(){ return  m_cdbAuthToken; }
    const QString& cdbMaxPrice(){return m_cdbMaxPrice;}
    const QString& cdbAdress(){return m_CDBaddress;}

    QList<QString> usersNames()          { return m_userInform.keys();     }
    const QString userInfo
          (const QString & user_name)    { return m_userInform[user_name]; }
    void setDapUri(const QString& addr, const uint16_t port);
    void clearCredentials();
    void preserveCDBSession();
    void activateBySerial(bool flag) { isSerial = flag; }
    DapCrypt* getDapCrypt() { return m_dapCrypt; }
    DapCrypt* getDapCryptCDB() { return m_dapCryptCDB; }
public slots:
    DapNetworkReply * requestServerPublicKey();
    DapNetworkReply * authorizeRequest(const QString& a_user, const QString& a_password,
                                     const QString& a_domain = QString(), const QString& a_pkey = QString(), const QString& a_order_hash = QString());
    DapNetworkReply * authorizeByKeyRequest(const QString& a_serial = QString(),
                                     const QString& a_domain = QString(), const QString& a_pkey = QString() , const QString& a_order_hash = QString());
    DapNetworkReply * activateKeyRequest(const QString& a_serial = QString(), const QByteArray& a_signed = QByteArray(),
                                     const QString& a_domain = QString(), const QString& a_pkey = QString() );
    void resetKeyRequest(const QString& a_serial = QString(),
                                      const QString& a_domain = QString(), const QString& a_pkey = QString());
    DapNetworkReply * logoutRequest();
    DapNetworkReply *streamOpenRequest(const QString& subUrl, const QString& query, QObject* obj, const char *slot, const char *slot_err);

    void sendSignUpRequest(const QString &host, const QString &email, const QString &password);
    void sendBugReport(const QByteArray &data);
    void sendBugReportStatusRequest(const QByteArray &data);
    void getRemainLimits(const QString& net_id, const QString& a_pkey, const QString& address, quint16 port);
    void getNews();
    void sendTxOutRequest(const QString &tx);
    DapNetworkReply *  sendNewTxCondRequest(const QString& a_serial, const QString& a_domain, const QString& a_pkey, const QString& a_order_hash);

#ifdef BUILD_VAR_GOOGLE
    void requestPurchaseVerify(const QJsonObject *params);
#endif
protected:
    using HttpHeaders = QVector<HttpRequestHeader>;
    quint16 m_upstreamPort, m_CDBport;
    QString m_upstreamAddress, m_CDBaddress, m_user;
    // HTTP header fields
    QString m_cookie, m_sessionKeyID, m_userAgent, m_sessionKeyID_CDB;

    // Net service fields

    QString m_cdbAuthTxCond, m_cdbAuthNet, m_cdbAuthToken, m_cdbMaxPrice;

    DapNetworkReply * m_netEncryptReply;
    DapNetworkReply * m_netAuthorizeReply;
    DapNetworkReply * m_netNewTxReply;
    DapNetworkReply * m_netKeyActivateReply;
    DapNetworkReply * m_netLogoutReply;
    DapNetworkReply * m_netSendBugReportReply;
    DapNetworkReply * m_netBugReportsStatusReply;
    DapNetworkReply * m_netSignUpReply;
#ifdef BUILD_VAR_GOOGLE
    DapNetworkReply * m_netPurchaseReply;
#endif


    QMap<QString,QString> m_userInform;

    DapNetworkReply* encRequestRaw(const QByteArray& bData, const QString& url,
                                 const QString& subUrl, const QString& query);

    DapNetworkReply* encRequest(const QString& reqData, const QString& url, const QString& subUrl,
                               const QString& query, QObject* obj, const char* slot, const char* slot_err, bool isCDB, bool isCriticalReq = true);

    DapNetworkReply* encRequestRaw(const QByteArray& bData, const QString& url, const QString& subUrl,
                               const QString& query, QObject* obj, const char* slot, const char* slot_err);

    DapNetworkReply* encRequest(const QString& reqData,const QString& url,
                                const QString& subUrl,const QString& query, bool isCDB, bool isCriticalReq = true) {
        return encRequest(reqData, url, subUrl, query, this, NULL, NULL, isCDB, isCriticalReq);
    }

    DapNetworkReply* encRequest(const QString& reqData, const QString& url,
                    const QString& subUrl, const QString& query, const char* slot, const char* slot_err, bool isCDB = false, bool isCriticalReq = true)
    {
        return encRequest(reqData, url, subUrl, query, this, slot, slot_err, isCDB, isCriticalReq);
    }

    DapNetworkReply* encRequestRaw(const QByteArray& bData, const QString& url,
                    const QString& subUrl, const QString& query, const char* slot, const char* slot_err)
    {
        return encRequestRaw(bData, url, subUrl, query, this, slot, slot_err);
    }

    DapNetworkReply* requestRawToSite(const QString& dnsName, const QString& url, const QByteArray& bData, const char * slot, bool ssl, const QString& headers);

private:
    DapNetworkAccessManager * m_httpClient;
    DapCrypt* m_dapCrypt, *m_dapCryptCDB;
    bool isSerial = false;
    DapNetworkReply* _buildNetworkReplyReq(const QString& urlPath, QObject *obj, const char *slot, const char *slot_err,
                                         const QByteArray* data = Q_NULLPTR, bool isCDB = false, bool isCriticalReq = true/*, DapNetworkReply *netReply = nullptr*/);

//    void requestDapClientHttp(const QString& host,  quint16 port, const QByteArray& data, const QString & urlPath, bool isCDB = false);

//    void requestDapClientHttp(const QString & urlPath, const QByteArray & body, bool isCDB = false);

//    static void responseCallback(void * a_response, size_t a_response_size, void * a_obj);
//    static void responseCallbackError(int a_err_code, void * a_obj);

private slots:
    void onEnc();
    void onAuthorize();
    void onKeyActivated();
#ifdef BUILD_VAR_GOOGLE
    void onPurchaseVerified();
#endif
    void onLogout();
    void answerBugReport();
    void answerBugReportsStatus();
    Q_INVOKABLE void answerBugReportsStatusError(const QString& msg);
    void answerSignUp();
    void onNewTxCond();
    void onResetSerialKey();
    void errorResetSerialKey(const QString&);

signals:
    void encryptInitialized();
    void errorEncryptInitialization(const QString& msg);

    void serverResponseError(const QString& msg);

    Q_INVOKABLE void errorAuthorization(const QString&);
    void activateKey();
    Q_INVOKABLE void errorActivation(const QString&);
    Q_INVOKABLE void errorNetwork(const QString&);
    Q_INVOKABLE void errorNetwork(const int, const QString&);

    void authRequested();
    void keyActRequested();
    void authorized(const QString &);
    /// TODO: this signal KelVPN not use
    void onAuthorized();
    void onConnectNoCDB();
    void usrDataChanged(const QString &addr, ushort port);
    
    void logoutRequested();
    void logouted();
    void newTxCondReceived(QString &);

    Q_INVOKABLE void receivedBugReportAnswer(const QString&);
    Q_INVOKABLE void receivedBugReportStatusAnswer(const QString&);
    void sigSignUpAnswer(const QString& signUpAnswer);
    void sigReceivedNewsMessage(const QJsonDocument& news);

    void sigSerialKeyReseted(const QString&);
    void sigResetSerialKeyError(const int, const QString&);
    void sigNewTxReceived();
    void sigNewTxError();
#ifdef BUILD_VAR_GOOGLE
    Q_INVOKABLE void purchaseResponseReceived(const QJsonDocument& response);
    void purchaseError(const QString&);
#endif
};

#endif // DAPSESSION_H
