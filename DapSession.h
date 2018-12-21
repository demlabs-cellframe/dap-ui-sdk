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

    void setDapUri(const QString& addr, const uint16_t port);

    void clearCredentials();

public slots:
    /* Request to server */
    /* QNetworkReply does not need to be cleared. It's do DapConnectClient */
    QNetworkReply * encryptInitRequest();
    QNetworkReply * authorizeRequest(const QString& user, const QString& password,
                                     const QString& domain);
    QNetworkReply * logoutRequest();
    QNetworkReply * streamOpenRequest(const QString& subUrl, const QString& query);

    void abortEncryptionInitRequest() { m_netEncryptReply->abort(); }
    void abortAuthorizeRequest()      { m_netEncryptReply->abort(); }
    void abortLogoutRequest()         { m_netLogoutReply->abort();  }

protected:
    using HttpHeaders = QVector<HttpRequestHeader>;

    quint16 m_upstreamPort;
    QString m_upstreamAddress, m_cookie,
            m_sessionKeyID, m_user;

    DapSession();
    ~DapSession();

    QNetworkReply * m_netEncryptReply;
    QNetworkReply * m_netAuthorizeReply;
    QNetworkReply * m_netLogoutReply;

    QMap<QString,QString> m_userInform;

    QNetworkReply* encRequest(const QString& reqData,const QString& url,
                              const QString& subUrl,const QString& query);

    QNetworkReply* encRequest(const QString& reqData, const QString& url, const QString& subUrl,
                               const QString& query, QObject* obj, const char* slot);

    QNetworkReply* encRequest(const QString& reqData, const QString& url,
                    const QString& subUrl, const QString& query, const char* slot)
    {
        return encRequest(reqData, url, subUrl, query, this, slot);
    }

    void fillSessionHttpHeaders(HttpHeaders& headers);
    QNetworkReply * requestServerPublicKey();

private slots:
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

    void authRequested();
    void authorized(const QString &);

    void logoutRequested();
    void logouted();
};




#endif // DAPSESSION_H
