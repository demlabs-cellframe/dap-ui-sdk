#include "DapNetworkReply.h"

DapNetworkReply::DapNetworkReply()
    :QObject(nullptr)
{

}

//void DapNetworkReply::responseCallback(void * a_response, size_t a_response_size, void * a_obj)
//{
//    DapNetworkReply * reply = reinterpret_cast<DapNetworkReply*>(a_obj);
//    //QByteArray response(  reinterpret_cast<const char*>(a_response), a_response_size);
////    reply->bRunning = false;
//    reply->m_reply = QString::fromLatin1( reinterpret_cast<const char*>(a_response), a_response_size) ;

//    qDebug() << "Dap Client HTTP Request: response received, size=" << a_response_size;
//    emit reply->finished();
//}

//void DapNetworkReply::responseCallbackError(int a_err_code, void * a_obj)
//{
//    DapNetworkReply * reply = reinterpret_cast<DapNetworkReply*>(a_obj);
////    reply->bRunning = false;
//    reply->m_error = NetworkError::Error;
//    emit reply->sigError();
//    qWarning() << "Dap Client HTTP Request: error code " << a_err_code ;
//}
