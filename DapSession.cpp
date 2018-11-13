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
#define OP_CODE_INCORRECT_SYM         "0xf6"

#include "DapSession.h"
#include "DapCrypt.h"
#include <QDir>
#include <QFile>

#include <QThread>
#include "msrln/msrln.h"

const QString DapSession::URL_ENCRYPT("/1901248124123459");
const QString DapSession::URL_STREAM("/874751843144");
const QString DapSession::URL_DB("/01094787531354");
const QString DapSession::URL_CTL("/091348758013553");
const QString DapSession::URL_DB_FILE("/98971341937495431398");
const QString DapSession::URL_SERVER_LIST("/slist");

#define SESSION_KEY_ID_LEN 33
/**
 * @brief DapSession::DapSession
 */
DapSession::DapSession()
{
    m_dapConnectBase = new DapConnectBase(this);
    baData = nullptr;
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
    if(baData)
        free(baData);
    baData=nullptr;

    m_xmlStreamReader.clear();
    arrData.clear();

    QByteArray reqData = DapCrypt::me()->generateAliceMessage().toBase64();
    netReply = m_dapConnectBase->request(URL_ENCRYPT + "/gd4y5yh78w42aaagh" , &reqData);

    connect(netReply, &QNetworkReply::readyRead, this, &DapSession::onDownloading);
    connect(netReply, &QNetworkReply::readChannelFinished, this,  &DapSession::onEnc);
    connect(netReply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(errorSlt(QNetworkReply::NetworkError)));

    emit pubKeyRequested();
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

    if(netReply->readBufferSize())
        arrData.append(netReply->readAll());

    QStringList buf = QString::fromLatin1(arrData).split(" ");
    if (buf.size()< 2) {
        qWarning() << "Wrong response from server";
        emit errorNetwork(tr("Wrong response from server"));
        return;
    }

    int pos = arrData.indexOf(' ') + 1;
    QByteArray result = arrData.mid(pos,arrData.size() - pos);
    qDebug() << "Pos :" << pos-1;
    m_sessionKeyID = QByteArray::fromBase64(buf[0].toLatin1()).mid(0, SESSION_KEY_ID_LEN);
    
    qDebug() << "m_sessionKeyID: " << m_sessionKeyID;

    if ( m_sessionKeyID.isEmpty() || result.isEmpty()) {
        qDebug() << "ERROR encryption not inited";
        emit errorEncryption();
        return;
    }

    QByteArray bobMsg = QByteArray::fromBase64(result);

    if (bobMsg.size() != MSRLN_PKB_BYTES) {
        qCritical() << "Server Bob message is failed, length = " << bobMsg.length();
        emit errorNetwork(tr("Server Bob message is failed"));
        return;
    }

    if(!DapCrypt::me()->generateSharedSessionKey(bobMsg, m_sessionKeyID.toLatin1())) {
        emit errorNetwork(tr("Error ganarete shared session key"));
        return;
    }

    emit encryptInitialized();
}

/**
 * @brief DapSession::onDownloading
 */
void DapSession::onDownloading()
{
    arrData.append(netReply->readAll());
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
    QByteArray BAsubUrlEncrypted;
    QByteArray BAqueryEncrypted;
    QByteArray subUrlByte = subUrl.toLatin1();
    QByteArray queryByte = query.toLatin1();

    DapCrypt::me()->encode(BAreqData, BAreqDataEnc, KeyRoleSession);


    if(subUrl.length())
        DapCrypt::me()->encode(subUrlByte, BAsubUrlEncrypted, KeyRoleSession);
    if(query.length())
        DapCrypt::me()->encode(queryByte, BAqueryEncrypted, KeyRoleSession);

    if(subUrl.length())
    {
        if(query.length())
            nr = dcb->request(url + "/" + BAsubUrlEncrypted.toBase64(QByteArray::Base64UrlEncoding)
                              + "?" + BAqueryEncrypted.toBase64(QByteArray::Base64UrlEncoding), &BAreqDataEnc);
        else
            nr = dcb->request(url + "/" + BAsubUrlEncrypted.toBase64(QByteArray::Base64UrlEncoding), &BAreqDataEnc);
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
void DapSession::setDapUri(const QString& addr, const uint16_t port)
{
    m_upstreamAddress = addr;
    m_upstreamPort = port;
}

/**
 * @brief DapSession::onAuthorize
 */
void DapSession::onAuthorize()
{
    if (critError) {
        qDebug() << "Critical Error = True!";
        return;
    }

    arrData.append(netReply->readAll());

    if(arrData.size() <= 0)
    {
        emit errorAuthorization("Wrong answer from server");
        return;
    }

    QByteArray arrData2 = QByteArray::fromBase64(arrData);

    QByteArray dByteArr;
    DapCrypt::me()->decode(arrData, dByteArr, KeyRoleSession);

    m_xmlStreamReader.addData(dByteArr);
    qDebug() << "[DapSession] Decoded data: " << QString::fromLatin1(dByteArr);

    if (QString::fromLatin1(dByteArr) == OP_CODE_NOT_FOUND_LOGIN_IN_DB) {
        emit errorAuthorization ("Login not found in database");
        return;
    } else if (QString::fromLatin1(dByteArr) == OP_CODE_LOGIN_INCORRECT_PSWD) {
        emit errorAuthorization ("Incorrect password");
        return;
    } else if (QString::fromLatin1(dByteArr) == OP_CODE_SUBSCRIBE_EXPIRIED) {
        emit errorAuthorization ("Subscribe expired");
        return;
    } else if (QString::fromLatin1(dByteArr) == OP_CODE_CANT_CONNECTION_TO_DB) {
        emit errorAuthorization ("Can't connect to database");
        return;
    } else if (QString::fromLatin1(dByteArr) == OP_CODE_INCORRECT_SYM){
        emit errorAuthorization("Incorrect symbols in request");
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
}

/**
 * @brief DapSession::testRsaReplacementSlot
 */
void DapSession::testMsrlnReplacementSlot()
{
    arrData.append(netReply->readAll());

    QByteArray dByteArr;
    DapCrypt::me()->decode(arrData, dByteArr, KeyRoleSession);

    m_xmlStreamReader.addData(dByteArr);
    qDebug() << "[DapSession] Decoded data: " << QString::fromLatin1(dByteArr);
}


/**
 * @brief DapSession::onLogout
 */
void DapSession::onLogout()
{
    qInfo() << "Logouted on the remote server";
    emit logouted();
}


void DapSession::restoreNetworkConf() {
    m_dapConnectBase->restoreDefaultNetConf();
}

void DapSession::saveNetworkConf() {
    m_dapConnectBase->saveCurrentNetConf();
}
/**
 * @brief DapSession::logout
 */
void DapSession::logout()
{
    qDebug() << "Request for logout";
    critError = false;
    encRequest("", DapSession::getInstance()->URL_DB, "auth", "logout", SLOT(onLogout()));
    m_cookie.clear();
    m_sessionKeyID.clear();
    m_upstreamAddress.clear();
    m_upstreamPort = 0;
    emit logoutRequested();
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
 * @brief DapSession::authorize
 * @param user
 * @param password
 * @param domain
 */
void DapSession::authorize(const QString& user, const QString& password,const QString& domain)
{
    critError = false;
    m_xmlStreamReader.clear();
    m_user = user;
    m_userInform.clear();
    m_domain = domain;
    encRequest(user + " " + password + " " + domain, URL_DB, "auth", "login", SLOT(onAuthorize()));
    emit authRequested();
}

/**
 * @brief DapSession::errorSlt
 * @param error
 */
void DapSession::errorSlt(QNetworkReply::NetworkError error)
{
    critError = true;
    switch(error){
        case QNetworkReply::ConnectionRefusedError:
            emit errorNetwork("connection refused");
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
