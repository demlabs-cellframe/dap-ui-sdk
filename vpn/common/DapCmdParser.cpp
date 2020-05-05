#include "DapCmdParser.h"

DapCmdParser::DapCmdParser(QTcpSocket* sockCtl, QObject *parent)
    : QObject(parent)
{    
    m_sockCtl = sockCtl;
    connect(m_sockCtl, &QTcpSocket::readyRead,
            this, &DapCmdParser::onCtlReadReady);
}

void DapCmdParser::onCtlReadReady()
{
    QByteArray readBytes = m_sockCtl->readAll();
    int nInd;

lb_read_str:
    if((nInd=readBytes.indexOf("}\n\n")) == -1 ) {
        qDebug() << "No CR symbol";
        readBuffer += readBytes;
        if(readBuffer.size() > MAX_BUFFER_SIZE) {
            qWarning() << "Buffer is full. All data is dropped";
            return;
        }
    } else {
        nInd+=2; // move idx to second \n
        readBuffer += readBytes.left(nInd);
        emit cmdReady(readBuffer);
        readBytes = readBytes.mid(nInd+1);
        readBuffer.clear();
        if(readBytes.length() > 0) {
            goto lb_read_str;
        }
    }
}
