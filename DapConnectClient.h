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


#ifndef DapConnectClient_H
#define DapConnectClient_H

#include <QObject>
#include <QNetworkReply>
#include <QAbstractSocket>
#include <QString>
#include <QList>
#include <QBuffer>

// using HttpMethod = QNetworkAccessManager::Operation;

using HttpRequestHeader = QPair<const QString&, const QString&>;

// MAKE SINGELTON
class DapConnectClient : public QObject
{
    Q_OBJECT
private:
    void rebuildNetworkManager();
private Q_SLOTS:
    void slotNetworkError(QNetworkReply::NetworkError);
    void slotReadPacketFinished();

public:
    DapConnectClient(QObject *parent = Q_NULLPTR);
    virtual ~DapConnectClient();

    /* Simple GET or POST request without custom headers.
       If rData == Q_NULLPTR GET request is sents */
    QNetworkReply* request(const QString& domain, const QString & url,
                           QByteArray * rData = Q_NULLPTR);

    /* Example call: request_GET("google.com", {
     *                                           HTTP_REQUEST_HEADER("User-Agent", "Mozilla/5.0"),
     *                                           HTTP_REQUEST_HEADER("Accept", "text/html")
     *                                         }); */
    QNetworkReply* request_GET(const QString& domain, const QString & url,
                               std::initializer_list<HttpRequestHeader> headers);

    // If Content Type Header not specified, sets: "text/plain"
    QNetworkReply* request_POST(const QString& domain, const QString & url, const QByteArray& data,
                                std::initializer_list<HttpRequestHeader> headers);

protected:
    QNetworkAccessManager * http_client;
    QNetworkReply * network_reply;
    // Network setting before upping DAP network interface
    const QNetworkConfiguration * defaultNetworkConfig = Q_NULLPTR;

    QByteArray byte_buffer;

    static QString httpAddress();

public slots:
    // Restore network configuration before upping DAP network interface
    void restoreDefaultNetConf();
    // Saving network configuration before upping DAP network interface
    void saveCurrentNetConf();
Q_SIGNALS:
    void finished();
    void errorNetwork(const QString &e);
    void errorAuth(int);

    void authenticationRequiredError();

    void notify(const QString&);
};

#endif // DapConnectClient_H
