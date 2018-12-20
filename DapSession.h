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

#define SERVER_LIST_FILE "DiveVPNServers.xml"

class DapSession : public QObject
{
    Q_OBJECT
public:
    static const QString URL_ENCRYPT;
    static const QString URL_STREAM;
    static const QString URL_DB;
    static const QString URL_CTL;
    static const QString URL_DB_FILE;
    static const QString URL_SERVER_LIST;

    static DapSession * getInstance()    { static DapSession session; return &session; }

    const QString& upstreamAddress()     { return m_upstreamAddress;       }
    quint16 upstreamPort()               { return m_upstreamPort;          }
    const QString& cookie()              { return m_cookie;                }
    const QString& sessionKeyID()        { return m_sessionKeyID;          }
    const QString& user()                { return m_user;                  }

    QList<QString> usersNames()          { return m_userInform.keys();     }
    const QString userInfo
          (const QString & user_name)    { return m_userInform[user_name]; }


    void setUser(const QString& a_str) {m_user = a_str;}

    QNetworkReply* encRequest2(DapConnectClient *dcb, const QString& reqData,const QString& url,
                              const QString& subUrl,const QString& query);

    void setDapUri(const QString& addr, const uint16_t port);

    void clearCredentials();

public slots:
    /* Requests */
    void encryptInit();
    void authorize(const QString& user, const QString& password, const QString& domain);
    void logout();

    // QNetworkReply must be cleaned
    QNetworkReply * request(const QString &url, const QString& queryString, const QByteArray *body = Q_NULLPTR);

    // url can be with query params
    QNetworkReply * request(const QString &url, const QByteArray *body = Q_NULLPTR);

    void restoreNetworkConf();
    void saveNetworkConf();
    void abortRequest() { netReply->abort(); }

protected:
    using HttpHeaders = QVector<HttpRequestHeader>;

    quint16 m_upstreamPort;
    QString m_upstreamAddress, m_cookie, m_sessionKeyID;
    DapSession();
    ~DapSession();
    DapConnectClient * m_DapConnectClient;


    QXmlStreamReader m_xmlStreamReader;
    QNetworkReply * netReply;

    QByteArray arrData;
    QByteArray *baData;

    QMap<QString,QString> m_userInform;

    QString m_user; 

    QString m_sessionServerBobMessage;

    void encRequest(const QString& reqData, const QString& url, const QString& subUrl,
                               const QString& query, QObject* obj, const char* slot);
    void encRequest(const QString& reqData, const QString& url,
                    const QString& subUrl, const QString& query, const char* slot)
    {
        encRequest(reqData, url, subUrl, query, this,slot);
    }

    void fillSessionHttpHeaders(HttpHeaders& headers);
public:

    void requestServerPublicKey();

private slots:
    void onDownloading();
    void onEnc();
    void errorSlt(QNetworkReply::NetworkError);
    void onAuthorize();
    void onLogout();
signals:
    void pubKeyRequested();
    void pubKeyServerRecived();
    void encryptInitialized();

    void errorEncryption();
    void errorAuthorization(const QString &);
    void errorNetwork(const QString&);
    void errorConnection();
    void errorOfPubKeyServerReciving();

    void authRequested();

    void authorized(const QString &);

    void logoutRequested();
    void logouted();
};




#endif // DAPSESSION_H
