#include "DapNetworkReply.h"

DapNetworkReply::DapNetworkReply()
    :QObject(nullptr)
{

}

DapNetworkReply::DapNetworkError DapNetworkReply::error() const
{
    int test = 3;
    return m_error;
}
