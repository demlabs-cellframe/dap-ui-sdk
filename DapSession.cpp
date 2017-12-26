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


#define OP_CODE_LOGIN_INCORRECT_PSWD  "0xf2"
#define OP_CODE_NOT_FOUND_LOGIN_IN_DB "0xf3"
#define OP_CODE_SUBSCRIBE_EXPIRIED    "0xf4"
#define OP_CODE_CANT_CONNECTION_TO_DB "0xf5"

#include "DapSession.h"
#include "DapCrypt.h"
#include <QDir>
#include <QFile>

#include <QEventLoop>
#include <QThread>

const QString DapSession::URL_ENCRYPT("/1901248124123459");
const QString DapSession::URL_STREAM("/874751843144");
const QString DapSession::URL_DB("/01094787531354");
const QString DapSession::URL_CTL("/091348758013553");
const QString DapSession::URL_DB_FILE("/98971341937495431398");
const QString DapSession::URL_SERVER_LIST("/slist");


/**
 * @brief DapSession::DapSession
 */
DapSession::DapSession()
{
    m_upstreamAddress = "any.divevpn.com";
    m_upstreamPort = "80";
    m_sessionKeyID = "sessionKeyID";

    m_dapConnectBase = new DapConnectBase(this);
    baData = nullptr;
    connect (m_dapConnectBase, &DapConnectBase::errorText, this, [=](const QString &str){
        _badSevers.append(BadServers(QString("NameNotNeed"), m_upstreamAddress, m_upstreamPort, m_user));
        emit errorAuthorization(str);
    });

    connect(this, &DapSession::pubKeyServerRecived, [&]{
        if(baData)
            free(baData);
        baData=nullptr;


        m_xmlStreamReader.clear();
        arrData.clear();

        QString reqDataRSA = DapCrypt::me()->createRSAKey();
        QByteArray reqData = reqDataRSA.toLatin1();
        qDebug() << "[DapSession] Public Client Key = " << reqDataRSA;
        netReply = m_dapConnectBase->request(URL_ENCRYPT + "/hsd9jslagd92abgjalp9h" , &reqData);

        connect(netReply, &QNetworkReply::readyRead, this, &DapSession::onDownloading);
        connect(netReply, &QNetworkReply::readChannelFinished, this,  &DapSession::onEnc);
        connect(netReply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(errorSlt(QNetworkReply::NetworkError)));
    });

}

/**
 * @brief DapSession::~DapSession
 */
DapSession::~DapSession()
{
    delete m_dapConnectBase;
}

/**
 * @brief DapSession::requestServerPublicKey
 */
void DapSession::requestServerPublicKey()
{
    QNetworkReply *networkReply;
    QString str;
    baData = new QByteArray();
    networkReply = m_dapConnectBase->request(URL_ENCRYPT + "/gd4y5yh78w42aaagh", str);

    connect(networkReply, &QNetworkReply::readyRead, [=] {
        baData->append(networkReply->readAll());
    });

    connect(networkReply, &QNetworkReply::readChannelFinished, [=]{
        if(networkReply->readBufferSize())
            baData->append(networkReply->readAll());
        if (baData->length() != 0) {
            DapCrypt::me()->setRsaPubKeyServer(QString::fromLatin1(*baData));
            emit pubKeyServerRecived();
        } else {
            emit errorOfPubKeyServerReciving();
        }
    });

    connect(networkReply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(errorSlt(QNetworkReply::NetworkError)));

}

void DapSession::encryptInit()
{
    requestServerPublicKey();
}

/**
 * @brief DapSession::onEnc
 */
void DapSession::onEnc()
{
    qDebug() << "On Enc()";

    /// RSA for DAP
    if(netReply->readBufferSize())
        arrData.append(netReply->readAll());

    QStringList buf = QString::fromLatin1(arrData).split(" ");
    if (buf.size()<=1) {
        return;
    }
    
    m_sessionKeyID = QByteArray::fromBase64(buf[0].toLatin1());
    m_sessionServerPublicKey = QByteArray::fromBase64( buf[1].toLatin1() );
    qDebug() << "Session key Id = " << m_sessionKeyID;
    qDebug() << " Server PublicRsaKey = " << m_sessionServerPublicKey;

    if ( m_sessionKeyID.isEmpty() || m_sessionServerPublicKey.isEmpty()) {
        qDebug() << "ERROR encryption not inited";
        emit errorEncryption();
        return;
  }
  
    int sig_len = QByteArray::fromBase64(buf[3].toLatin1()).toInt();

    unsigned char* sig = new unsigned char[1024];

    DapCrypt::fromBase64(buf[2].toLatin1(), buf[2].toLatin1().size(), sig);


    if (m_sessionServerPublicKey.length() < 10) {
        qCritical() << "Length of RSA key is" << m_sessionServerPublicKey.length();
        ::exit(0);
    }

    if ( RSA_verify(NID_sha256, (unsigned char*) m_sessionServerPublicKey.toLatin1().data(),
            200, sig, sig_len, DapCrypt::me()->getKeyServerPublic()->getPubKey()) != 1)
    {
        qCritical() << "Failed verifycation session key!";
        ::exit(0);
    }
    qInfo() << "Verification Session key it's Ok";
    if (!DapCrypt::me()->setRsaSessionServerKey(m_sessionServerPublicKey)) {
        emit errorNetwork(tr("Server doesn't respond"));
        return;
    }
    delete []sig;
    emit encryptInitialized();

    _badSevers.removeAll(BadServers(QString("NameNotNeed"), m_upstreamAddress, m_upstreamPort, m_user));
    //_badSevers.append(BadServers(QString("NameNotNeed"), QString("127.0.0.1"), QString("80"), QString("User1")));
    //_badSevers.append(BadServers(QString("NameNotNeed"), QString("127.0.0.2"), QString("90"), QString("User2")));

}

/**
 * @brief DapSession::onDownloading
 */
void DapSession::onDownloading()
{
    arrData.append(netReply->readAll());
    qDebug() << " total: " << arrData.length() << " length";
}


/**
 * @brief DapSession::encRequest2
 * @param dcb
 * @param reqData
 * @param url
 * @param subUrl
 * @param query
 * @return
 */
QNetworkReply* DapSession::encRequest2(DapConnectBase *dcb, const QString& reqData, const QString& url,
                          const QString& subUrl, const QString& query)
{
    QNetworkReply * nr;
    QByteArray BAreqData = reqData.toLatin1();
    QByteArray BAreqDataEnc;
    QByteArray BAsubUrlEncB64;
    QByteArray BAqueryEncB64;

    qDebug() << "Request Data = " << BAreqData;

    DapCrypt::me()->encode(BAreqData, BAreqDataEnc, KeyRoleSessionServer);


    if(subUrl.length())
        DapCrypt::me()->encodeB64(subUrl, BAsubUrlEncB64, KeyRoleSessionServer);
    if(query.length())
        DapCrypt::me()->encodeB64(query, BAqueryEncB64, KeyRoleSessionServer);

  //  qDebug() << "Query size = " << BAqueryEncB64.length();
  //  qDebug() << "Query Encode : " << BAqueryEncB64;

    if(subUrl.length())
    {
        if(query.length())
            nr = dcb->request(url + "/" + BAsubUrlEncB64 + "?" + BAqueryEncB64, &BAreqDataEnc);
        else
            nr = dcb->request(url + "/" + BAsubUrlEncB64, &BAreqDataEnc);
    }
    else {
        nr = dcb->request(url + "/", &BAreqDataEnc);
    }

    return nr;
}

/**
 * @brief DapSession::setSaUri
 * @param saUri
 */
void DapSession::setSaUri(const QString &saUri)
{
    m_upstreamAddress=saUri.section(":",0,0);
    m_upstreamPort=saUri.section(":",1,1);
}

/**
 * @brief DapSession::onAuthorize
 */
void DapSession::onAuthorize()
{
    if ( critError ) {
        qDebug() << "Critical Error = True!";
        return;
    }

    arrData.append(netReply->readAll());
    
    QByteArray dByteArr;
    DapCrypt::me()->decode(arrData, dByteArr, KeyRoleSession);

    qDebug() << "Decoded data size = " << dByteArr.size();
    m_xmlStreamReader.addData(dByteArr);
    qDebug() << "[DapSession] Decoded data: " << QString::fromLatin1(dByteArr);

    // Вывод сообщений о ошибке

    if (QString::fromLatin1(dByteArr) == OP_CODE_NOT_FOUND_LOGIN_IN_DB) {
        emit errorAuthorization ("not_found_login_in_db");
        return;
    } else if (QString::fromLatin1(dByteArr) == OP_CODE_LOGIN_INCORRECT_PSWD) {
        emit errorAuthorization ("login_incorrect_pswd");
        return;
    } else if (QString::fromLatin1(dByteArr) == OP_CODE_SUBSCRIBE_EXPIRIED) {
        emit errorAuthorization ("subscribe_expired");
        return;
    } else if (QString::fromLatin1(dByteArr) == OP_CODE_CANT_CONNECTION_TO_DB) {
        emit errorAuthorization ("cant_connect_to_db");
        return;
    }


    bool isCookie = false;
    QString SRname;
    while(m_xmlStreamReader.readNextStartElement())
    {
        qDebug() << " name = " << m_xmlStreamReader.name();
        if(SRname == "err_str") {
            QString error_text = m_xmlStreamReader.readElementText();
            qDebug() << " Error str = " << error_text;
            emit errorAuthorization(QString("Server replied error string: '%1'").arg(error_text));
            return;
        }

        if (m_xmlStreamReader.name() == "auth_info") {
            while(m_xmlStreamReader.readNextStartElement()) {
                qDebug() << " auth_info = " << m_xmlStreamReader.name();
                if (m_xmlStreamReader.name() == "cookie") {
                    m_cookie = m_xmlStreamReader.readElementText();
                    isCookie = true;
                    //requestServerList();
                    emit authorized(m_cookie);
                } else {
                   m_userInform[m_xmlStreamReader.name().toString()] = m_xmlStreamReader.readElementText();

                }
            }
        } else {
            m_xmlStreamReader.skipCurrentElement();
        }
    }

    if(!isCookie) {
        critError = false;
        m_cookie.clear();
        emit errorAuthorization("No authorization cookie in server's reply");
    }
    //else
    //    emit authorizationProblem("No Auth. cookie in server's reply");
//    qDebug() << "SLEEP";
//    QThread::sleep(3); // for test TTL Session key
//    encRequest("user2 pass2 domain.com",
//               dataBaseUrl, "TestRsaKey", "login", SLOT(testRsaReplacementSlot()));
}

/**
 * @brief DapSession::testRsaReplacementSlot
 */
void DapSession::testRsaReplacementSlot()
{
    arrData.append(netReply->readAll());

    QByteArray dByteArr;
    DapCrypt::me()->decode(arrData, dByteArr, KeyRoleSession);

    qDebug() << "[DapSession] Decoded data size = " << dByteArr.size();
    m_xmlStreamReader.addData(dByteArr);
    qDebug() << "[DapSession] Decoded data: " << QString::fromLatin1(dByteArr);
}


/**
 * @brief DapSession::onLogout
 */
void DapSession::onLogout()
{
    qInfo() << "Logouted on the remote server";
}

/**
 * @brief DapSession::logout
 */
void DapSession::logout()
{
    qDebug() << "Request for logout";
    critError = false;
    m_cookie.clear();
    encRequest("", DapSession::getInstance()->URL_DB, "auth", "logout", SLOT(onLogout()));
    emit logouted();
}

/**
 * @brief DapSession::encRequest
 * @param reqData
 * @param url
 * @param subUrl
 * @param query
 * @param obj
 * @param slot
 */
void DapSession::encRequest(const QString& reqData, const QString& url, const QString& subUrl,
                           const QString& query, QObject * obj, const char * slot)
{
    arrData.clear();
    netReply = encRequest2(m_dapConnectBase, reqData, url, subUrl, query);


    connect(netReply, SIGNAL(readyRead()), obj,SLOT(onDownloading()));
    connect(netReply, SIGNAL(readChannelFinished()), obj, slot);
    connect(netReply, SIGNAL(error(QNetworkReply::NetworkError)),
            this,SLOT(errorSlt(QNetworkReply::NetworkError)));

}

/**
 * @brief DapSession::requestServerList
 */
void DapSession::serverListRequester(bool first_run)
{
    QNetworkReply *nReply;
    static QByteArray ans;
    int internal_state = 0;
    ans.clear();

    auto serverListSerialize = [](const QList<BadServers> &badlist) {
        QString out("");
        foreach (const BadServers &bs, badlist) {
            out += QString("&N%1&%2&%3").arg(bs.m_addr).arg(bs.m_port).arg(bs.m_user);
        }
        out.append('&');
        return out;
    };

    if(first_run) {
        qInfo() << "DapSession request run in update ServerList mode";
        nReply = encRequest2(m_dapConnectBase, "none",
                             URL_SERVER_LIST, "update", "getall");
        internal_state = 1;
    }
    else /*if (_badSevers.size())*/{
        qInfo() << "DapSession run in send bad server list mode";
        nReply = encRequest2(m_dapConnectBase, serverListSerialize(_badSevers),
                             URL_SERVER_LIST, "report", "badreport");
    } //else return;

    connect(nReply, &QNetworkReply::readyRead, [/*&ans,*/ nReply]
    {
           ans.append(nReply->readAll());
    });

    connect(nReply, &QNetworkReply::readChannelFinished, [/*&ans,*/ nReply, internal_state]
    {
        if(nReply->readBufferSize())
            ans.append(nReply->readAll());

        QByteArray ar2;
        DapCrypt::me()->decode(ans, ar2, KeyRoleSession);

        if (internal_state) {
            qDebug() <<"we in internal_state";
#ifdef Q_OS_WIN
            QFile file(QString(SERVER_LIST_FILE));
#else
            QFile file("/opt/divevpn/share/"+QString(SERVER_LIST_FILE));
#endif
            if (file.open(QIODevice::WriteOnly))
                file.write(ar2.toStdString().c_str());
            else
                qCritical() << "Can't open ServerList file";
        }

//        qDebug() << "SLEEP";
//        QThread::sleep(3); // for test TTL Session key
//        DapSession::getInstance()->requestTestKey();
    });

    connect(nReply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(errorSlt(QNetworkReply::NetworkError)));

}

/**
 * @brief DapSession::authorize
 * @param user
 * @param password
 * @param domain
 */
void DapSession::authorize(const QString& user, const QString& password,const QString& domain)
{
    emit authorizing();
    critError = false;
    m_xmlStreamReader.clear();
    m_user = user;
    m_userInform.clear();
    m_domain = domain;
    encRequest(user + " " + password + " " + domain, URL_DB, "auth", "login", SLOT(onAuthorize()));
}

/**
 * @brief DapSession::errorSlt
 * @param error
 */
void DapSession::errorSlt(QNetworkReply::NetworkError error)
{
    critError = true;
    switch(error){
        case QNetworkReply::ConnectionRefusedError: {
                _badSevers.append(BadServers(QString("NameNotNeed"), m_upstreamAddress, m_upstreamPort, m_user));
                emit errorNetwork("connection refused");
            }
            break;
        case QNetworkReply::HostNotFoundError:
            emit errorNetwork("Network error: host not found");
            break;
        case QNetworkReply::TimeoutError:
            emit errorNetwork("Network error: timeout, may be host is down?");
            break;
        case QNetworkReply::TemporaryNetworkFailureError:
            emit errorNetwork("Network error: Temprorary network problems, reply request as soon as the network connection is re-established");
            break;
        case QNetworkReply::NetworkSessionFailedError:
            emit errorNetwork("Network error: No network connection");
            break;
        case QNetworkReply::BackgroundRequestNotAllowedError:
            emit errorNetwork("Network error: background request are not permitted by OS");
            break;
        case QNetworkReply::ProxyConnectionRefusedError:
            emit errorNetwork("Network error: Proxy refused to connection");
            break;
        case QNetworkReply::ProxyNotFoundError:
            emit errorNetwork(tr("Proxy server is not found"));
            break;
        case QNetworkReply::ProxyTimeoutError:
            emit errorNetwork(tr("Proxy server timout, is it up?"));
            break;
        case QNetworkReply::ProxyAuthenticationRequiredError:
            emit errorNetwork(tr("Authorization problem"));
            break;
        default:{
            emit errorNetwork(tr("Undefined network error"));
            critError = false;
        }
    }
}

