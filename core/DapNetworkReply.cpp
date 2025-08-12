#include "DapNetworkReply.h"

DapNetworkReply::DapNetworkReply()
    :QObject(nullptr)
    , m_error(0)
    , m_finished(false)
    , m_response_size(0)
    , m_content_length(0)
{

}
