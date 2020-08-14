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

private:

    void dropWasteServers();
    void fillingServersContainer();

    bool isDuplicate(const DapServerInfo m_server);


};

#endif // DAPSERVERROTATOR_H
