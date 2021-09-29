#include "DapNetworkReply.h"

DapNetworkReply::DapNetworkReply()
    :QObject(nullptr)
{

}

DapNetworkReply::DapNetworkError DapNetworkReply::error() const
{
    return m_error;
}

int DapNetworkReply::errorNative() const {
    return m_errorNative;
}
