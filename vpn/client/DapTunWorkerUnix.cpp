#include <QtDebug>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>

#include <errno.h>
#include <netinet/ip.h>

#include "dap_common.h"
#include "DapTunWorkerUnix.h"

const struct timespec DapTunWorkerUnix::BREAK_LATENCY = { 0, 1 * 1000 * 1000 };

static bool mDebugMore = false;


/**
 * @brief DapTunWorkerUnix::DapTunWorkerUnix
 */
DapTunWorkerUnix::DapTunWorkerUnix(DapTunAbstract *a_tun)
    :DapTunWorkerAbstract(a_tun)
{
    m_breakerSet[0] = m_breakerSet[1] = -1;
    if (::pipe(m_breakerSet) < 0){
        qCritical() <<"Can't create unix named pipe!";
        throw QString("Can't create unix named pipe!");
    }
}


/**
 * @brief DapTunWorkerUnix::loop
 */
void DapTunWorkerUnix::loop()
{
    size_t l_bufSize = 0;
    qDebug() << "listenTunnelThread() start with MTU = " << DAP_IP_MTU;

#ifdef DAP_OS_DARWIN
    byte_t pktBuf[DAP_IP_MTU+4];
    pktBuf[0] = 0;  // Magic numbers for PI header
    pktBuf[1] = 0;
    pktBuf[2] = 0;
    pktBuf[3] = 2;
#endif
    fd_set fds_read, fds_read_active;
    fd_set fds_write, fds_write_active;
    size_t pktOutPos=0;
    Dap::Stream::Packet* pktOut = nullptr;
    FD_ZERO (&fds_read);
    FD_ZERO (&fds_write);
    FD_SET (tunSocket(), &fds_read);
    FD_SET (tunSocket(), &fds_write);
    int select_breaker = breaker(0);
    FD_SET ( select_breaker,&fds_read);
    do{
        fds_read_active=fds_read;
        fds_write_active = fds_write;

        if (pktOut == nullptr) {  // if no non-sent data
            pktOut = writeDequeue();
        }
        int ret = 0;
        if (pktOut)
            ret = ::select(FD_SETSIZE,&fds_read_active,&fds_write_active,NULL,NULL);
        else
            ret = ::select(FD_SETSIZE,&fds_read_active,NULL,NULL,NULL) ;
        //
        if(ret > 0) {
            if (FD_ISSET (tunSocket(), &fds_read_active)){
                if(mDebugMore)
                    qDebug() << "Tun socket is ready for read() ";
                int readRet = ::read(tunSocket(), m_tunBuf + l_bufSize, DAP_IP_MTU);
                byte_t * curBuf = m_tunBuf;

                if(mDebugMore)
                    qDebug() << "Read "<< readRet<<" bytes";
                if (readRet < 0) {
                    emit loopError(QString("read() returned %1 (%2)").arg(readRet).arg(::strerror(errno)));
                    qCritical() << "Read returned "<< readRet;
                    qDebug() << QString("read() returned %1 (%2)").arg(readRet).arg(::strerror(errno));
                    break;
                }
#ifdef DAP_OS_DARWIN
                // Darwin's specifics, OS adds 4 bytes header in the beginning of each packet
                if (readRet < 4){
                    emit loopError(QString("read() returned %1 thats lesser than PI header").arg(readRet));
                    qCritical() << "Read returned "<< readRet;
                    qDebug() << QString("read() returned %1 tjats lesser than PI header").arg(readRet);
                    break;
                }
                if(mDebugMore)
                    qDebug() << "0x"<< curBuf[0]
                             << "0x"<< curBuf[1]
                             << "0x"<< curBuf[2]
                             << "0x"<< curBuf[3];
                curBuf += 4;
                readRet -= 4;
#endif
                l_bufSize += readRet;

                if (l_bufSize > (int)sizeof(struct ip)) {
                    if(mDebugMore){
                        struct ip * ipHdr = (struct ip*) (((unsigned char*) curBuf));
                        qInfo() << "iphdr src: " << inet_ntoa(ipHdr->ip_src);
                        qInfo() << "iphdr dst: " << inet_ntoa(ipHdr->ip_dst);
                    }
                    emit bytesWrite(l_bufSize);
                    procDataFromTun(curBuf, l_bufSize); // Pack data with SF headers and emit it out to DapStreamer
                    l_bufSize = 0;
                }

            }else if( (pktOut) && (FD_ISSET (tunSocket(), &fds_write_active))){ // Event to Tun socket
                if(mDebugMore)
                    qDebug() << "Tun socket is ready for write() ";
#ifdef DAP_OS_DARWIN
                int pktSize = pktOut->header.op_data.data_size - pktOutPos;
                ::memcpy(pktBuf+4, pktOut->data+pktOutPos, pktSize);
                int writeRet = ::write(tunSocket(), pktBuf,pktSize+4 );
                if(writeRet >=4)
                    writeRet -= 4;
#else
                int writeRet = ::write(tunSocket(), pktOut->data+pktOutPos,pktOut->header.op_data.data_size - pktOutPos );
#endif
                if(mDebugMore)
                    qDebug() << "Wrote"<< writeRet<<" bytes";
                if (writeRet < 0) {
                    qCritical() << "Write returned "<< writeRet;
                    break;
                }
                pktOutPos += writeRet;
                if(pktOutPos>= pktOut->header.op_data.data_size ){ // Packet is sent into the tunnel
                    emit bytesRead(pktOutPos);
                    delete pktOut;
                    pktOut = nullptr;
                    pktOutPos = 0;
                }
            } else if(FD_ISSET (select_breaker, &fds_read_active)) { // Event to stop the socket
                char ctlCode=0;
                ::read(select_breaker, &ctlCode,1 );
                if( ctlCode == 0 ){ // Other variants - just waked up to write smth out
                    qInfo() << "Got the signal to stop the loop";
                    break;
                }
            }
            else {
                emit loopError(QString("Select has no tun handler in the returned set"));
                qCritical() << "[SapStreamChSF] listenTunnelThread() select has no tun handler in the returned set";
                break;
            }
        }else {
            if (errno == EINTR) {
                continue;
            } else {
                qCritical() << "[SapStreamChSF] select() returned "<< ret << strerror(errno);
                emit loopError(QString("select() returned %1 (%2)").arg(ret).arg(::strerror(errno)));
                break;
            }
        }

    }while(1);

    qDebug() << "[SapStreamChSF] Listen thread finished!";
    //emit loopStopped();
}
