#ifndef DAPSERVERROTATOR_H
#define DAPSERVERROTATOR_H

#include "DapDataLocal.h"
#include <QWidget>
//#include <QMessageBox>
#include <QtWidgets/QMessageBox>

class DapServerRotator : public QWidget
{
public:

    /*static DapServerRotator *_me;
    static DapServerRotator * me(){ return _me?_me: _me = new DapServerRotator();}*/

    DapServerRotator(QWidget* a_parent = nullptr);

    bool selectingRandomServer();
    bool selectingSameLocationServer();
    void dropWasteServers();
    void setRotationServersProcess(bool state){
        bRotationServersProcess = state;
    };
    bool getRotationServersProcess(){
        return bRotationServersProcess;
    }

private:

    void fillingServersContainer();

    bool isDuplicate(const DapServerInfo m_server);

    bool bRotationServersProcess = false;


};

#endif // DAPSERVERROTATOR_H
