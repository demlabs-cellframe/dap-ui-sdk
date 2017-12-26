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
#include "BaseObject.h"
#include "DapConnectBase.h"
#include "DapConnSession.h"

#define SERVER_LIST_FILE "DiveVPNServers.xml"

struct BadServers
{
    QString m_name;
    QString m_port;
    QString m_addr;
    QString m_user;

    BadServers(QString serverName = "", QString address = "", QString port = "", QString user = "")
    {
        m_name = serverName;
        m_port = port;
        m_addr = address;
        m_user = user;
    }

    bool operator==(const BadServers &rhs) {
        if (m_name == rhs.m_name &&
                m_port == rhs.m_port &&
                m_addr == rhs.m_addr &&
                m_user == rhs.m_user) {
            return true;
        }
        return false;
    }
};

class DapSession : public BaseObject
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
    const QString upstreamPort()         { return m_upstreamPort;          }
    const QString& cookie()              { return m_cookie;                }
    const QString& sessionKeyID()        { return m_sessionKeyID;          }
    const QString& user()                { return m_user;                  }
    const QString& domain()              { return m_domain;                }
    const QString upstreamIp()           { return m_upstreamIp;            }

    QList<QString> usersNames()          { return m_userInform.keys();     }
    const QString userInfo
          (const QString & user_name)    { return m_userInform[user_name]; }


    void setDapUri(const QString& a_str) { m_upstreamAddress = a_str.section(":",0,0); m_upstreamPort = a_str.section(":",1,1);}
    void setUser(const QString& a_str) {m_user = a_str;}
    void setIP(const QString& a_str) {m_upstreamIp = a_str;}

    QNetworkReply* encRequest2(DapConnectBase *dcb, const QString& reqData,const QString& url,
                              const QString& subUrl,const QString& query);

    bool isAuthorized() { return cookie().length()>0; }

    void setSaUri(const QString& saUri); //ok

public slots:
    void encryptInit();
    void authorize(const QString& user, const QString& password, const QString& domain);
    void logout();

protected:

    QString m_upstreamAddress, m_upstreamPort, m_cookie, m_sessionKeyID;
    DapSession();
    ~DapSession();
    DapConnectBase * m_dapConnectBase;


    QXmlStreamReader m_xmlStreamReader;
    QNetworkReply * netReply;

    QByteArray arrData;
    QByteArray *baData;

    QMap<QString,QString> m_userInform;

    QString m_user; 
    QString m_domain;
    QString m_upstreamIp;

    QString m_sessionServerPublicKey;

    bool critError = false;

    DapConnSession *sc;

    void encRequest(const QString& reqData, const QString& url, const QString& subUrl,
                               const QString& query, QObject* obj, const char* slot);
    void encRequest(const QString& reqData, const QString& url,
                    const QString& subUrl, const QString& query, const char* slot)
    {
        encRequest(reqData, url, subUrl, query, this,slot);
    }
public:

    void serverListRequester(bool);
    void requestServerPublicKey();
    void requestTestKey()
    {
        encRequest("user2 pass2 domain.com",
                   URL_DB, "TestRsaKey", "login", SLOT(testRsaReplacementSlot()));
    }

private:
    QList<BadServers> _badSevers;
private slots:
    void onDownloading();
    void onEnc();
    void errorSlt(QNetworkReply::NetworkError);
    void onAuthorize();
    void onLogout();
    void testRsaReplacementSlot();
signals:
    void pubKeyServerRecived();
    void encryptInitialized();

    void errorEncryption();
    void errorAuthorization(const QString &);// эта штука отработает и при недоступном сервере
    void errorNetwork(const QString&);
    void errorConnection();
    void errorOfPubKeyServerReciving();
    void authorizing();

    void authorized(const QString &);
    void logouted();
};




#endif // DAPSESSION_H
