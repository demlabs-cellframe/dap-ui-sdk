#include <io.h>
#include <time.h>
#include <string>
#include "DapChSockForw.h"

#ifndef IP_MTU
#define IP_MTU 0xffff // Until this is integrated from linux/in.h to netinet/in.h
#endif

#include "DapTunWorkerWindows.h"

/**
 * @brief DapTunWorkerWindows::DapTunWorkerWindows
 */
DapTunWorkerWindows::DapTunWorkerWindows(DapTunAbstract *a_tun)
    :DapTunWorkerAbstract(a_tun)

{

}

/**
 * @brief DapTunWorkerWindows::loop
 */
void DapTunWorkerWindows::loop() {
    if (TunTap::getInstance().setAdress(tun()->addr(), tun()->gw(), QString("255.255.255.0"))) {
        qCritical() << "Tunnel cannot be setup with provided settings";
        emit loopError("Error TUN setup");
        //emit loopStopped();
        return;
    }
    size_t l_bufSize = 0;
    qDebug() << "Tunnel listen, socket: " << tun()->m_tunSocket << ", mtu " << DAP_IP_MTU;
    do {
        if (!TunTap::getInstance()) {
            qCritical() << "No tun device present!";
            break;
        }
        int readRet = TunTap::getInstance().read_tun(tun()->m_tunSocket, m_tunBuf + l_bufSize, DAP_IP_MTU - l_bufSize);
        if (readRet < 0) {
            qWarning() << "[DapTunWorkerWindows] Error read from tun";
            //continue;
            emit loopError("Error read from tun");
            break;
        }
        l_bufSize += readRet;
        if(l_bufSize > (int)sizeof(struct ip)) {
            emit bytesWrite((int)l_bufSize);
            procDataFromTun(m_tunBuf, l_bufSize);
            l_bufSize = 0;
        }
    } while (1);
}
