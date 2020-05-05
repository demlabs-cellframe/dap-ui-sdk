#ifndef DAPCHTHREAD_H
#define DAPCHTHREAD_H

#include <QThread>
#include <DapChannelPacket.h>
class DapChThread : public QThread
{
    Q_OBJECT
public:
    DapChThread(QObject *obj) : QThread(obj) { }
    ~DapChThread() {  }
signals:
    void sigNewPkt(DapChannelPacket*);
public slots:
    void sltProcPacket(DapChannelPacket *pkt) {
        emit sigNewPkt(pkt);
    }
};


#endif // DAPCHTHREAD_H
