#include <QtDebug>
#include "DapTunWorkerDarwin.h"

DapTunWorkerDarwin::DapTunWorkerDarwin(DapTunAbstract *a_tun)
                :DapTunWorkerAbstract(a_tun)
{

}

/**
 * @brief DapTunWorkerWindows::loop
 */
void DapTunWorkerDarwin::loop() {
    do {
        int readRet = -1;//TunTap::getInstance().read_tun(tun()->m_tunSocket, m_tunBuf + l_bufSize, DAP_IP_MTU - l_bufSize);
        if (readRet < 0) {
            qWarning() << " Error read from tun";
            //continue;
            emit loopError("Error read from tun");
            break;
        }
        procDataFromTun(m_tunBuf, readRet);
    } while (1);
}
