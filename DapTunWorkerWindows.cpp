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
    TunTap::getInstance().setAdress(tun()->addr(), tun()->gw(), QString("255.255.255.0"));
    const int mtu = 6000; // x4 relative to max protocol data unit, aswell as max PPP frame.
    quint8 tmpBuf[mtu] = {'\0'};
    size_t tmpBufSize = 0;
    qDebug() << "Tunnel listen, socket: " << tun()->m_tunSocket << ", mtu " << mtu;
    do {
        if (!TunTap::getInstance()) {
            qCritical() << "No tun device present!";
            break;
        }
        int readRet = TunTap::getInstance().read_tun(tun()->m_tunSocket, tmpBuf + tmpBufSize, mtu - tmpBufSize);
        if (readRet < 0) {
            qWarning() << "[DapTunWorkerWindows] Error read from tun";
            //continue;
            emit loopError("Error read from tun");
            break;
        }
        tmpBufSize += readRet;
        if(tmpBufSize > (int)sizeof(struct ip)) {
            emit bytesWrite((int)tmpBufSize);
            procDataFromTun(tmpBuf, tmpBufSize);
            tmpBufSize = 0;
        }
    } while (1);
}
