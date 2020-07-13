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

class DapSession : public QObject
{
    Q_OBJECT
private:
    static const int DEFAULT_REQUEST_TIMEOUT = 10000; // 10 sec
    const int m_requestTimeout;
public:
    static const QString URL_ENCRYPT;
    static const QString URL_STREAM;
    static const QString URL_DB;
    static const QString URL_CTL;
    static const QString URL_DB_FILE;
    static const QString URL_SERVER_LIST;
    static const QString URL_TX;
    static const QString URL_BUG_REPORT;

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


    QList<QString> usersNames()          { return m_userInform.keys();     }
    const QString userInfo
          (const QString & user_name)    { return m_userInform[user_name]; }
    void setDapUri(const QString& addr, const uint16_t port);
    void clearCredentials();
    void preserveCDBSession();
    DapCrypt* getDapCrypt() { return m_dapCrypt; }
public slots:
    /* Request to server */
    /* QNetworkReply does not need to be cleared. It's do DapConnectClient */
    QNetworkReply * encryptInitRequest();
    QNetworkReply * authorizeRequest(const QString& a_user, const QString& a_password,
                                     const QString& a_domain = QString(), const QString& a_pkey = QString() );
    QNetworkReply * authorizeByKeyRequest(const QString& a_serial = QString(),
                                     const QString& a_domain = QString(), const QString& a_pkey = QString() );
    QNetworkReply * activateKeyRequest(const QString& a_serial = QString(), const QByteArray& a_signed = QByteArray(),
                                     const QString& a_domain = QString(), const QString& a_pkey = QString() );
    QNetworkReply * logoutRequest();
    QNetworkReply * streamOpenRequest(const QString& subUrl, const QString& query);

    void sendBugReport(const QByteArray &data);

    void abortEncryptionInitRequest() { m_netEncryptReply->abort(); }
    void abortAuthorizeRequest()      { m_netAuthorizeReply->abort(); }
    void abortLogoutRequest()         { m_netLogoutReply->abort();  }
    void sendTxBackRequest(const QString &tx);
protected:
    using HttpHeaders = QVector<HttpRequestHeader>;

    quint16 m_upstreamPort, m_CDBport;
    QString m_upstreamAddress, m_CDBaddress, m_user;
    // HTTP header fields
    QString m_cookie, m_sessionKeyID, m_userAgent, m_sessionKeyID_CDB;

    // Net service fields

    QString m_cdbAuthTxCond, m_cdbAuthNet, m_cdbAuthToken;

    QNetworkReply * m_netEncryptReply;
    QNetworkReply * m_netAuthorizeReply;
    QNetworkReply * m_netLogoutReply;
    QNetworkReply * m_netSendBugReportReply;

    QMap<QString,QString> m_userInform;

    QNetworkReply* encRequest(const QString& reqData,const QString& url,
                              const QString& subUrl,const QString& query, bool isCDB);

    QNetworkReply* encRequestRaw(const QByteArray& bData, const QString& url,
                                 const QString& subUrl, const QString& query);

    QNetworkReply* encRequest(const QString& reqData, const QString& url, const QString& subUrl,
                               const QString& query, QObject* obj, const char* slot, bool isCDB);

    QNetworkReply* encRequestRaw(const QByteArray& bData, const QString& url, const QString& subUrl,
                               const QString& query, QObject* obj, const char* slot);

    QNetworkReply* encRequest(const QString& reqData, const QString& url,
                    const QString& subUrl, const QString& query, const char* slot, bool isCDB = false)
    {
        return encRequest(reqData, url, subUrl, query, this, slot, isCDB);
    }

    QNetworkReply* encRequestRaw(const QByteArray& bData, const QString& url,
                    const QString& subUrl, const QString& query, const char* slot)
    {
        return encRequestRaw(bData, url, subUrl, query, this, slot);
    }

    void fillSessionHttpHeaders(HttpHeaders& headers, bool isCDBSession = false) const;
    QNetworkReply * requestServerPublicKey();
private:
    DapCrypt* m_dapCrypt, *m_dapCryptCDB;
    QNetworkReply* _buildNetworkReplyReq(const QString& urlPath,
                                         const QByteArray* data = Q_NULLPTR, bool isCDB = false);
private slots:
    void onEnc();
    void errorSlt(QNetworkReply::NetworkError);
    void onAuthorize();
    void onKeyActivated();
    void onLogout();
    void answerBugReport();
signals:
    void pubKeyRequested();
    void pubKeyServerRecived();

    void encryptInitialized();
    void errorEncryptInitialization(const QString& msg);

    void serverResponseError(const QString& msg);

    void errorAuthorization(const QString &);
    void activateKey();
    void errorNetwork(const QString&);

    void authRequested();
    void keyActRequested();
    void repeatAuth();
    void authorized(const QString &);
    void onAuthorized();
    void usrDataChanged(const QString &addr, ushort port);
    void logoutRequested();
    void logouted();

    void receivedBugReportNumber(const QString& bugReportNumber);
};




#endif // DAPSESSION_H
