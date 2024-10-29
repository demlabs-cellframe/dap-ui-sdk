#include "DapCmdParser.h"

DapCmdParser::DapCmdParser(QTcpSocket* sockCtl, QObject *parent)
    : QObject(parent)
{
    m_sockCtl = sockCtl;
    connect(m_sockCtl, &QTcpSocket::readyRead,
            this, &DapCmdParser::onCtlReadReady);

    qDebug() << "DapCmdParser initialized with socket" << sockCtl;
}

void DapCmdParser::onCtlReadReady()
{
    QByteArray readBytes = m_sockCtl->readAll();
    int nInd;

    // Loop to handle multiple potential commands in the incoming data
    while (!readBytes.isEmpty()) {
        // Check if the end of the command sequence (}\n\n) is present
        if ((nInd = readBytes.indexOf("}\n\n")) == -1) {
            // Append received data to the buffer
            readBuffer += readBytes;

            // Check if buffer size exceeds the maximum allowed
            if (readBuffer.size() > MAX_BUFFER_SIZE) {
                qWarning() << "Buffer is full. All data is dropped. Buffer content: " << readBuffer;
                readBuffer.clear(); // Clear buffer to prevent overflow
            }
            break;
        } else {
            // Command sequence found, processing the command
            nInd += 2; // Move index to the second \n

            // Extract the complete command from the data
            readBuffer += readBytes.left(nInd);
            emit cmdReady(readBuffer);

            // Remove processed command from the remaining data
            readBytes = readBytes.mid(nInd + 1);
            readBuffer.clear();
        }
    }
}
