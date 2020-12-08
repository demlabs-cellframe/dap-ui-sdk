#include "DapNetworkAccessManager.h"

DapNetworkAccessManager::DapNetworkAccessManager()
{

}

void DapNetworkAccessManager::requestHttp(const QString &address, const uint16_t &port, const QString & urlPath, const QByteArray & body)
{
    qDebug() << "Dap Client HTTP Requested " << urlPath ;
    bRunning = true;
    dap_client_http_request(NULL ,
                            address.toLocal8Bit().constData(),
                            port,
                            "POST",
                            "text/text",
                            urlPath.toLocal8Bit().constData(),
                            body.constData(),
                            body.size(),
                            NULL,
                            &DapNetworkAccessManager::responseCallback,
                            &DapNetworkAccessManager::responseCallbackError,
                            this,
                            NULL);
}

void DapNetworkAccessManager::responseCallback(void * a_response, size_t a_response_size, void * a_obj)
{
    DapNetworkAccessManager * manager = reinterpret_cast<DapNetworkAccessManager*>(a_obj);
    manager->bRunning = false;
    manager->reply = QString(reinterpret_cast<const QChar*>(a_response), a_response_size).toUtf8();

    qDebug() << "Dap Client HTTP Request: response received, size=" << a_response_size ;
    emit manager->finished();
}

void DapNetworkAccessManager::responseCallbackError(int a_err_code, void * a_obj)
{
    DapNetworkAccessManager * manager = reinterpret_cast<DapNetworkAccessManager*>(a_obj);
    manager->bRunning = false;

    qWarning() << "Dap Client HTTP Request: error code " << a_err_code ;
}
