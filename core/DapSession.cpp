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

#define OP_CODE_GENERAL_ERR             "0xf0"
#define OP_CODE_ALREADY_ACTIVATED       "0xf1"
#define OP_CODE_LOGIN_INCORRECT_PSWD    "0xf2"
#define OP_CODE_NOT_FOUND_LOGIN_IN_DB   "0xf3"
#define OP_CODE_SUBSCRIBE_EXPIRED       "0xf4"
#define OP_CODE_CANT_CONNECTION_TO_DB   "0xf5"
#define OP_CODE_INCORRECT_SYM           "0xf6"
#define OP_CODE_LOGIN_INACTIVE          "0xf7"
#define OP_CODE_SERIAL_ACTIVATED        "0xf8"

#include "DapSession.h"
#include "DapCrypt.h"
#include "msrln/msrln.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "DapDataLocal.h"
#include "DapSerialKeyData.h"

const QString DapSession::URL_ENCRYPT               ("enc_init");
const QString DapSession::URL_STREAM                ("stream");
const QString DapSession::URL_DB                    ("db");
const QString DapSession::URL_CTL                   ("stream_ctl");
const QString DapSession::URL_DB_FILE               ("db_file");
const QString DapSession::URL_SERVER_LIST           ("nodelist");
const QString DapSession::URL_TX                    ("tx");
const QString DapSession::URL_BUG_REPORT            ("bugreport");
const QString DapSession::URL_NEWS                  ("news");
const QString DapSession::URL_SIGN_UP               ("wp-json/dapvpn/v1/register/");
#ifdef BUILD_VAR_GOOGLE
const QString DapSession::URL_VERIFY_PURCHASE("verify_purchase");
#endif

DapSession::DapSession(QObject * obj, int requestTimeout) :
    QObject(obj), m_requestTimeout(requestTimeout)
{
    m_dapCrypt = new DapCrypt;
    m_dapCryptCDB = nullptr;
//    m_netAccessManager = new DapNetworkAccessManager;
}

DapSession::~DapSession()
{
    delete m_dapCrypt;
    if (m_dapCryptCDB)
        delete m_dapCryptCDB;
}

DapNetworkReply * DapSession::streamOpenRequest(const QString& subUrl, const QString& query, QObject *obj, const char *slot, const char *slot_err) {
    if(m_sessionKeyID.isEmpty()) {
        qCritical() << "Can't send request to server."
                       " Session was not initialized";
        return Q_NULLPTR;
    }

    QByteArray subUrlEncrypted, queryEncrypted;

    QByteArray subUrlByte = subUrl.toLatin1();
    QByteArray queryByte = query.toLatin1();

    m_dapCrypt->encode(subUrlByte, subUrlEncrypted, KeyRoleSession);
    m_dapCrypt->encode(queryByte, queryEncrypted, KeyRoleSession);

    QString str_url = QString("%1/%2?%3").arg(URL_CTL)
            .arg(QString(subUrlEncrypted.toBase64(QByteArray::Base64UrlEncoding)))
            .arg(QString(queryEncrypted.toBase64(QByteArray::Base64UrlEncoding)));

    return _buildNetworkReplyReq(str_url, obj, slot, slot_err, Q_NULLPTR);
}

DapNetworkReply* DapSession::_buildNetworkReplyReq(const QString& urlPath, QObject *obj, const char *slot,  const char *slot_err,
                                                 const QByteArray* data, bool isCDB)
{
    DapNetworkReply *netReply = new DapNetworkReply();
    if (slot)
        connect(netReply, SIGNAL(finished()), obj, slot);
    connect(netReply, &DapNetworkReply::sigError, this, [=] {
        emit errorNetwork(netReply->error(), netReply->errorString());
        if (slot_err)
            QMetaObject::invokeMethod(obj, slot_err, Qt::ConnectionType::AutoConnection, Q_ARG(const QString&, netReply->errorString()));
    });
    data ? DapConnectClient::instance()->request_POST(isCDB ? m_CDBaddress : m_upstreamAddress,
                                                      isCDB ? m_CDBport : m_upstreamPort,
                                                      urlPath, *data, *netReply,
                                                      QString("KeyID: %1\r\n").arg(isCDB ? m_sessionKeyID_CDB : m_sessionKeyID))
         : DapConnectClient::instance()->request_GET(isCDB ? m_CDBaddress : m_upstreamAddress,
                                                     isCDB ? m_CDBport : m_upstreamPort,
                                                     urlPath, *netReply,
                                                     QString("KeyID: %1\r\n").arg(isCDB ? m_sessionKeyID_CDB : m_sessionKeyID));
    return netReply;
}

/**
 * @brief DapSession::requestServerPublicKey
 */
DapNetworkReply* DapSession::requestServerPublicKey()
{
    QByteArray aliceMessage= m_dapCrypt->generateAliceMessage();
    QByteArray reqData = aliceMessage.toBase64();
    m_enc_type          = DAP_ENC_KEY_TYPE_SALSA2012;
    m_pkey_exch_type    = DAP_ENC_KEY_TYPE_MSRLN;
    m_enc_size = 32;
    m_netEncryptReply = _buildNetworkReplyReq(QString("%1/gd4y5yh78w42aaagh?enc_type=%2,pkey_exchange_type=%3,pkey_exchange_size=%4,block_key_size=%5")
                                              .arg(URL_ENCRYPT)
                                              .arg(m_enc_type)
                                              .arg(m_pkey_exch_type)
                                              .arg(MSRLN_PKA_BYTES)
                                              .arg(m_enc_size)
                                              ,this, SLOT(onEnc()), NULL, /*QT_STRINGIFY(errorNetwork),*/ &reqData);
    qDebug() << "Public key requested";
    return m_netEncryptReply;
}

void DapSession::sendBugReport(const QByteArray &data)
{
    if (!m_dapCryptCDB) {
        this->setDapUri(*DapDataLocal::instance()->m_cdbIter, 80);
        auto *l_tempConn = new QMetaObject::Connection();
        *l_tempConn = connect(this, &DapSession::encryptInitialized, [&, data, l_tempConn]{
            preserveCDBSession();
            m_netSendBugReportReply = encRequestRaw(data, URL_BUG_REPORT, QString(), QString(),
                                                    SLOT(answerBugReport()), QT_STRINGIFY(receivedBugReportAnswer));
            disconnect(*l_tempConn);
            delete l_tempConn;
        });
        requestServerPublicKey();
    } else {
        m_netSendBugReportReply = encRequestRaw(data, URL_BUG_REPORT, QString(), QString(),
                                                SLOT(answerBugReport()), QT_STRINGIFY(receivedBugReportAnswer));
    }
}

void DapSession::sendBugReportStatusRequest(const QByteArray &data)
{
    m_CDBaddress = *DapDataLocal::instance()->m_cdbIter;
    m_CDBport = 80;
    m_netBugReportsStatusReply = _buildNetworkReplyReq(URL_BUG_REPORT + "?bugreports=" + data, this, SLOT(answerBugReportsStatus()), QT_STRINGIFY(answerBugReportsStatusError), NULL, true);
}

void DapSession::sendSignUpRequest(const QString &host, const QString &email, const QString &password)
{
    m_netSignUpReply = requestRawToSite(host, URL_SIGN_UP,
                                        QString("email=%1&password=%2").arg(email).arg(password).toUtf8(), SLOT(answerSignUp()),
                                        true, /*QString("Content-Type: application/x-www-form-urlencoded\r\n")*/ "");
}

void DapSession::getNews()
{
    DapNetworkReply *m_netNewsReply = new DapNetworkReply;
    connect(m_netNewsReply, &DapNetworkReply::sigError, this, [=]() {
        qCritical() << "Couldn't fetch news";
        return;
    });

    connect(m_netNewsReply, &DapNetworkReply::finished, this, [=]() {
        qInfo() << "News received";
        QJsonParseError jsonErr;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(m_netNewsReply->getReplyData(), &jsonErr);
        if(!jsonDoc.isNull()) {
            if(!jsonDoc.isArray()) {
                qCritical() << "Error parse response. Must be array";
            }
        } else {
            qWarning() << "Server response:" << m_netNewsReply->getReplyData();
            qCritical() << "Can't parse server response to JSON: "<<jsonErr.errorString()<< " on position "<< jsonErr.offset ;
        }
        emit sigReceivedNewsMessage(jsonDoc);
    });
    DapConnectClient::instance()->request_GET(*DapDataLocal::instance()->m_cdbIter, 80, URL_NEWS, *m_netNewsReply);
}

/**
 * @brief DapSession::onEnc
 */
void DapSession::onEnc()
{
    qDebug() << "Enc reply";
    if(m_netEncryptReply->getReplyData().isEmpty()) {
        qWarning() << "Empty enc reply...";
        emit errorEncryptInitialization("Empty enc reply");
        return;
    }

    QJsonParseError json_err;
    auto json_resp = QJsonDocument::fromJson(m_netEncryptReply->getReplyData(), &json_err);
    if(json_err.error != QJsonParseError::NoError) {
        QString errorMessage = "Can't parse response from server";
        qCritical() << errorMessage << json_err.errorString();
        emit errorEncryptInitialization(errorMessage);
        return;
    }

    auto json_err_resp = json_resp.object().value("error");
    if(json_err_resp != QJsonValue::Undefined) {
        QString serverErrorMsg = json_err_resp.toString();
        qCritical() << "Got error message from server:"
                    << json_err_resp.toString();
        emit errorEncryptInitialization(serverErrorMsg);
        return;
    }

    auto json_encrypt_id = json_resp.object().value("encrypt_id");
    auto json_encrypt_msg = json_resp.object().value("encrypt_msg");
    if(json_encrypt_id == QJsonValue::Undefined ||
            json_encrypt_msg == QJsonValue::Undefined) {
        QString errorMessage = "Bad response from server";
        emit errorEncryptInitialization(errorMessage);
        return;
    }

    m_sessionKeyID = QByteArray::fromBase64(json_encrypt_id.toString().toLatin1());
    QByteArray bobMsg = QByteArray::fromBase64(json_encrypt_msg.toString().toLatin1());

    if (bobMsg.size() != MSRLN_PKB_BYTES) {
        QString errorMessage = "Bad length encrypt message from server";
        qCritical() << "Server Bob message is failed, length = " << bobMsg.length();
        emit errorEncryptInitialization(errorMessage);
        return;
    }

    if(!m_dapCrypt->generateSharedSessionKey(bobMsg, m_sessionKeyID.toLatin1())) {
        QString errorMessage = "Failed generate session key";
        qCritical() << errorMessage;
        emit errorEncryptInitialization("Failed generate session key");
        return;
    }
    qDebug() << "Encryption initialized";
    emit encryptInitialized();
}

void DapSession::setUserAgent(const QString& userAgent)
{
    m_userAgent = userAgent;
}

/**
 * @brief DapSession::encRequest
 * @param dcb
 * @param reqData
 * @param url
 * @param subUrl
 * @param query
 * @return
 */
DapNetworkReply* DapSession::encRequest(const QString& reqData, const QString& url,
                          const QString& subUrl, const QString& query, QObject* obj, const char* slot, const char* slot_err, bool isCDB)
{
    QByteArray BAreqData = reqData.toLatin1();
    QByteArray BAreqDataEnc;
    QByteArray BAsubUrlEncrypted;
    QByteArray BAqueryEncrypted;
    QByteArray subUrlByte = subUrl.toLatin1();
    QByteArray queryByte = query.toLatin1();
    DapCrypt *l_dapCrypt = isCDB ? m_dapCryptCDB : m_dapCrypt;

    l_dapCrypt->encode(BAreqData, BAreqDataEnc, KeyRoleSession);

    QString urlPath = url;
    if(subUrl.length()) {
        l_dapCrypt->encode(subUrlByte, BAsubUrlEncrypted, KeyRoleSession);
        urlPath += "/" + BAsubUrlEncrypted.toBase64(QByteArray::Base64UrlEncoding);
    }
    if(query.length()) {
        l_dapCrypt->encode(queryByte, BAqueryEncrypted, KeyRoleSession);
        urlPath += "?" + BAqueryEncrypted.toBase64(QByteArray::Base64UrlEncoding);
    }

    return _buildNetworkReplyReq(urlPath, obj, slot, slot_err, &BAreqDataEnc, isCDB);
}

DapNetworkReply* DapSession::encRequestRaw(const QByteArray& bData, const QString& url,
                          const QString& subUrl, const QString& query, QObject* obj, const char* slot, const char* slot_err)
{
    QByteArray BAreqDataEnc;
    QByteArray BAsubUrlEncrypted;
    QByteArray BAqueryEncrypted;
    QByteArray subUrlByte = subUrl.toLatin1();
    QByteArray queryByte = query.toLatin1();
    DapCrypt *l_dapCrypt = m_dapCryptCDB;

    l_dapCrypt->encode(const_cast<QByteArray&>(bData), BAreqDataEnc, KeyRoleSession);

    QString urlPath = url;
    if(subUrl.length()) {
        l_dapCrypt->encode(subUrlByte, BAsubUrlEncrypted, KeyRoleSession);
        urlPath += "/" + BAsubUrlEncrypted.toBase64(QByteArray::Base64UrlEncoding);
    }
    if(query.length()) {
        l_dapCrypt->encode(queryByte, BAqueryEncrypted, KeyRoleSession);
        urlPath += "?" + BAqueryEncrypted.toBase64(QByteArray::Base64UrlEncoding);
    }

    return _buildNetworkReplyReq(urlPath, obj, slot, slot_err, &BAreqDataEnc, true);
}

/**
 * @brief DapSession::setSaUri
 * @param saUri
 */
void DapSession::setDapUri(const QString& addr, const uint16_t port)
{
    qDebug() << "Set upstream: " << addr << ":" << port;
    m_upstreamAddress = addr;
    m_upstreamPort = port;
}

void DapSession::onKeyActivated() {
    qInfo() << "Activation reply";

    if(m_netKeyActivateReply->getReplyData().size() <= 0) {
        emit errorAuthorization("Wrong answer from server");
        return;
    }

    QByteArray dByteArr;
    m_dapCrypt->decode(m_netKeyActivateReply->getReplyData(), dByteArr, KeyRoleSession);
    QString op_code = QString::fromLatin1(dByteArr).left(4);
    if (op_code == OP_CODE_SERIAL_ACTIVATED) {
        qInfo() << "Serial key activated, try to authorize";
        emit authRequested();
        return;
    }
    else {
        emit errorAuthorization("Serial key was not activated, code " + op_code);
        return;
    }
}

#ifdef BUILD_VAR_GOOGLE
void DapSession::onPurchaseVerified() {
    if (m_netPurchaseReply && (m_netPurchaseReply->error() != 0)) {
        qCritical() << m_netPurchaseReply->errorString();
        emit errorNetwork("Purchase error, please report");
        return;
    }

    if(m_netPurchaseReply->getReplyData().size() <= 0) {
        emit errorNetwork("Wrong answer from server");
        return;
    }

    qInfo() << "purchase verify request replied";
    QByteArray arrData(m_netPurchaseReply->getReplyData());
    QJsonParseError jsonErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(arrData, &jsonErr);

    if(!jsonDoc.isNull()) {
        emit purchaseResponseReceived(jsonDoc);
    } else {
        qWarning() << "Purchase responce is null" << arrData;
        return;
    }
}
#endif
/**
 * @brief DapSession::onAuthorize
 */
void DapSession::onAuthorize()
{
    qDebug() << "Auth reply";
    if(m_netAuthorizeReply->getReplyData().size() <= 0)
    {
        emit errorAuthorization("Wrong answer from server");
        return;
    }

    QByteArray dByteArr;
    m_dapCrypt->decode(m_netAuthorizeReply->getReplyData(), dByteArr, KeyRoleSession);

    QString op_code = QString::fromUtf8(dByteArr).left(4);

    if (op_code == OP_CODE_GENERAL_ERR) {
        emit errorAuthorization ("Unknown authorization error");
        return;
    } else if (op_code == OP_CODE_ALREADY_ACTIVATED) {
        emit errorAuthorization ("Serial key already activated on another device");
        return;
    } else if (op_code == OP_CODE_NOT_FOUND_LOGIN_IN_DB) {
        emit errorAuthorization (isSerial ? tr("Serial key not found in database") : "Login not found in database");
        return;
    } else if (op_code == OP_CODE_LOGIN_INCORRECT_PSWD) {
        if(m_user.isEmpty() && !isSerial)
            emit errorAuthorization ("Login not found in database");
        else
        emit errorAuthorization (isSerial ? tr("Incorrect serial key") : "Incorrect password");
        return;
    } else if (op_code == OP_CODE_SUBSCRIBE_EXPIRED) {
        emit errorAuthorization ("Serial key expired");
        return;
    } else if (op_code == OP_CODE_CANT_CONNECTION_TO_DB) {
        emit errorAuthorization ("Can't connect to database");
        return;
    } else if (op_code == OP_CODE_INCORRECT_SYM) {
        emit errorAuthorization("Incorrect symbols in request");
        return;
    } else if (op_code == OP_CODE_LOGIN_INACTIVE) {
        emit activateKey();
        return;
    }

    QXmlStreamReader m_xmlStreamReader;
    m_xmlStreamReader.addData(dByteArr);

    bool isCookie = false;
    bool isAuth = false;
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
                    qDebug() << "m_cookie: " << m_cookie;
                    isCookie = true;
                    //requestServerList();
                    emit authorized(m_cookie);
                } else if (m_xmlStreamReader.name() == "tx_cond_tpl") {
                    while(m_xmlStreamReader.readNextStartElement()) {
                        qDebug() << " tx_cond_tpl: " << m_xmlStreamReader.name();
                        if (m_xmlStreamReader.name() == "net") {
                            m_cdbAuthNet = m_xmlStreamReader.readElementText();
                            qDebug() << "m_srvNet: " << m_cdbAuthNet;
                        } else if (m_xmlStreamReader.name() == "token") {
                             m_cdbAuthToken = m_xmlStreamReader.readElementText();
                            qDebug() << "m_srvToken: " << m_cdbAuthToken;
                        } else if (m_xmlStreamReader.name() == "tx_cond") {
                             m_cdbAuthTxCond = m_xmlStreamReader.readElementText();
                            qDebug() << "m_srvTxCond: " << m_cdbAuthTxCond;
                        } else {
                            qWarning() <<"Unknown element" << m_xmlStreamReader.readElementText();
                        }
                    }
                } else if (m_xmlStreamReader.name() == "ts_active_till"){
                    DapDataLocal::instance()->serialKeyData()->setLicenseTermTill(m_xmlStreamReader.readElementText());
                    qDebug() << "ts_active_till: " << DapDataLocal::instance()->serialKeyData()->licenseTermTill().toTime_t();
                } else {
                    m_userInform[m_xmlStreamReader.name().toString()] = m_xmlStreamReader.readElementText();
                    qDebug() << "Add user information: " << m_xmlStreamReader.name().toString()
                             << m_userInform[m_xmlStreamReader.name().toString()];
                }
            }
            isAuth = true;
            emit authorized(m_cookie);
        }/*else if (m_xmlStreamReader.name() == "tx_cond_tpl") {
            while(m_xmlStreamReader.readNextStartElement()) {
                qDebug() << " tx_cond_tpl: " << m_xmlStreamReader.name();

                if (m_xmlStreamReader.name() == "net") {
                    m_cdbAuthNet = m_xmlStreamReader.readElementText();
                    qDebug() << "m_srvNet: " << m_cdbAuthNet;
                }else if (m_xmlStreamReader.name() == "token") {
                     m_cdbAuthToken = m_xmlStreamReader.readElementText();
                     qDebug() << "m_srvToken: " << m_cdbAuthToken;
                }else if (m_xmlStreamReader.name() == "tx_cond") {
                     m_cdbAuthTxCond = m_xmlStreamReader.readElementText();
                     qDebug() << "m_srvTxCond: " << m_cdbAuthTxCond;
                } else {
                    qWarning() <<"Unknown element" << m_xmlStreamReader.readElementText();
                }
            }
        }*/ else {
            m_xmlStreamReader.skipCurrentElement();
        }
    }
    if (!isAuth) {
        emit errorAuthorization("Authorization error");
    }

    /*if(!isCookie) {
        m_cookie.clear();
        emit errorAuthorization("No authorization cookie in server's reply");
    }*/
}

void DapSession::preserveCDBSession() {
    qInfo() << "Saving CDB session data";
    if (m_dapCryptCDB) {
        delete m_dapCryptCDB;
    }
    m_dapCryptCDB = new DapCrypt(*m_dapCrypt);
    m_sessionKeyID_CDB = m_sessionKeyID;
    m_CDBaddress = m_upstreamAddress;
    m_CDBport = m_upstreamPort;
    //todo: save cookie too
}
/**
 * @brief DapSession::onLogout
 */
void DapSession::onLogout() {
    qInfo() << "Logouted";
}

void DapSession::answerSignUp()
{
    qInfo() << "answerSignUp";
    if(m_netSignUpReply->error() != 0) {
        qInfo() << m_netSignUpReply->errorString();
        emit sigSignUpAnswer(m_netSignUpReply->errorString());
        return;
    }

    QJsonDocument itemDoc = QJsonDocument::fromJson(m_netSignUpReply->getReplyData());
    QJsonObject itemObj = itemDoc.object();
    QVariantMap mainMap = itemObj.toVariantMap();
    QVariantMap map = mainMap["result"].toMap();
    QString message = map["message"].toString();
    emit sigSignUpAnswer(message);
}

void DapSession::answerBugReport()
{
    if (!m_netSendBugReportReply)
        return;
    qInfo() << "Bugreport reply: " << m_netSendBugReportReply->getReplyData();
    emit receivedBugReportAnswer(QString::fromUtf8(m_netSendBugReportReply->getReplyData()));
}

void DapSession::errorResetSerialKey(const QString& error)
{
    qDebug() << "Reset serial key error: network error";
    emit sigResetSerialKeyError (2, "Network error during the reset of the serial number");
    return;
}

void DapSession::onResetSerialKey()
{
    if(m_netKeyActivateReply->getReplyData().size() <= 0 ) {
        emit errorResetSerialKey("Wrong answer from server");
        return;
    }

    QByteArray replyArr;
    m_dapCryptCDB->decode(m_netKeyActivateReply->getReplyData(), replyArr, KeyRoleSession);
    qDebug() << "Serial key reset reply: " << QString::fromUtf8(replyArr);

    QString op_code = QString::fromUtf8(replyArr).left(4);

    if (op_code == OP_CODE_GENERAL_ERR) {
        emit sigResetSerialKeyError (1, "Unknown authorization error");
        return;
    } else if (op_code == OP_CODE_ALREADY_ACTIVATED) {
        emit sigResetSerialKeyError (1, "Serial key already activated on another device");
        return;
    } else if (op_code == OP_CODE_NOT_FOUND_LOGIN_IN_DB) {
        emit sigResetSerialKeyError (1, isSerial ? tr("Serial key not found in database") : "Login not found in database");
        return;
    } else if (op_code == OP_CODE_SUBSCRIBE_EXPIRED) {
        emit sigResetSerialKeyError (1, "Serial key expired");
        return;
    } else if (op_code == OP_CODE_CANT_CONNECTION_TO_DB) {
        emit sigResetSerialKeyError (1, "Can't connect to database");
        return;
    } else if (op_code == OP_CODE_INCORRECT_SYM) {
        emit sigResetSerialKeyError(1, "Incorrect symbols in request");
        return;
    }

    emit sigSerialKeyReseted(tr("Serial key reset successfully"));
}
void DapSession::answerBugReportsStatus()
{
    if (!m_netBugReportsStatusReply)
        return;
    qInfo() << "Bugreport status reply: " << m_netBugReportsStatusReply->getReplyData();
    emit receivedBugReportStatusAnswer(QString::fromUtf8(m_netBugReportsStatusReply->getReplyData()));
}

void DapSession::answerBugReportsStatusError(const QString& msg)
{
    if (msg.isEmpty())
        return;
    qInfo() << "Bugreport status reply error: " << msg;
}

void DapSession::clearCredentials()
{
    qDebug() << "clearCredentials()";
    m_cookie.clear();
    m_sessionKeyID.clear();
}

/**
 * @brief DapSession::logout
 */
DapNetworkReply *DapSession::logoutRequest() {
    qInfo() << "Logout on CDB...";
    //m_netLogoutReply = encRequest("", URL_DB, "auth", "logout", SLOT(onLogout()), true);
    emit logouted();
    clearCredentials();
    m_upstreamAddress.clear();
    m_upstreamPort = 0;
    return nullptr;
}

DapNetworkReply * DapSession::requestRawToSite(const QString& dnsName, const QString& url, const QByteArray& bData, const char * slot, bool ssl, const QString& headers)
{
    DapNetworkReply * netReply = new DapNetworkReply;
    DapConnectClient::instance()->request_POST(dnsName, 443, url, bData, *netReply, headers, ssl);
    connect(netReply, SIGNAL(finished()), this, slot);
    return netReply;
}

void DapSession::sendTxOutRequest(const QString &tx) {
    qDebug() << "Send tx out to cdb" << tx;
    encRequest(tx, URL_TX, "tx_out", NULL, true);
}

/**
 * @brief DapSession::authorize
 * @param user
 * @param password
 * @param domain
 */
DapNetworkReply * DapSession::authorizeRequest(const QString& a_user, const QString& a_password, const QString& a_domain, const QString& a_pkey)
{
    m_user = a_user;
    m_userInform.clear();

    m_netAuthorizeReply = encRequest(a_pkey.isNull() ? a_user + " " + a_password + " " + a_domain :
                                                       a_user + " " + a_password + " " + a_domain + " " + a_pkey,
                                     URL_DB, "auth", "login", SLOT(onAuthorize()), /*QT_STRINGIFY(errorAuthorization)*/ NULL);
    return m_netAuthorizeReply;
}

DapNetworkReply * DapSession::authorizeByKeyRequest(const QString& a_serial, const QString& a_domain, const QString& a_pkey) {
    m_userInform.clear();
    m_netAuthorizeReply = encRequest(a_serial + " " + a_domain + " " + a_pkey,
                                     URL_DB, "auth", "serial", SLOT(onAuthorize()), /*QT_STRINGIFY(errorAuthorization)*/ NULL);
    return m_netAuthorizeReply;
}

DapNetworkReply *DapSession::activateKeyRequest(const QString& a_serial, const QByteArray& a_signed, const QString& a_domain,
                                              const QString& a_pkey) {
    m_userInform.clear();
    char buf64[a_signed.size() * 2 + 6];
    int buf64len = dap_enc_base64_encode(a_signed.constData(), a_signed.size(), buf64, DAP_ENC_DATA_TYPE_B64_URLSAFE);
    QByteArray a_signedB64(buf64, buf64len);
    QByteArray bData = QString(a_serial + " ").toLocal8Bit() + a_signedB64 + QString(" " + a_domain + " " + a_pkey).toLocal8Bit();
    m_netKeyActivateReply = encRequestRaw(bData, URL_DB, "auth_key", "serial", SLOT(onKeyActivated()), QT_STRINGIFY(errorActivation));
    return m_netKeyActivateReply;
}

void DapSession::resetKeyRequest(const QString& a_serial, const QString& a_domain, const QString& a_pkey) {
    if (!m_dapCryptCDB) {
        this->setDapUri(*DapDataLocal::instance()->m_cdbIter, 80);
        auto *l_tempConn = new QMetaObject::Connection();
        *l_tempConn = connect(this, &DapSession::encryptInitialized, [&, a_serial, a_domain, a_pkey, l_tempConn]{
            preserveCDBSession();
            m_netKeyActivateReply = encRequest(a_serial + " " + a_domain + " " + a_pkey,
                                               URL_DB, "auth_deactivate", "serial",
                                               SLOT(onResetSerialKey()), QT_STRINGIFY(errorResetSerialKey), true);
            disconnect(*l_tempConn);
            delete l_tempConn;
        });
        requestServerPublicKey();
    } else {
        m_netKeyActivateReply = encRequest(a_serial + " " + a_domain + " " + a_pkey,
                                           URL_DB, "auth_deactivate", "serial",
                                           SLOT(onResetSerialKey()), QT_STRINGIFY(errorResetSerialKey), true);
    }
}


#ifdef BUILD_VAR_GOOGLE
void DapSession::requestPurchaseVerify(const QJsonObject *params)
{
    QJsonDocument jdoc(*params);
    m_netPurchaseReply = encRequestRaw(jdoc.toJson(), URL_VERIFY_PURCHASE, QString(), QString(), SLOT(onPurchaseVerified()), NULL);
    if(m_netPurchaseReply == Q_NULLPTR) {
        qCritical() << "Can't send request";
        emit errorNetwork("Purchase verification error");
    }
    return;
}
#endif
