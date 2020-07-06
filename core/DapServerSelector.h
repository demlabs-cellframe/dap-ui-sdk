#ifndef DAPSERVERSELECTOR_H
#define DAPSERVERSELECTOR_H

#include "DapDataLocal.h"
#include <QWidget>
//#include <QMessageBox>
#include <QtWidgets/QMessageBox>

class DapServerSelector : public QWidget
{
public:

    /*static DapServerSelector *_me;
    static DapServerSelector * me(){ return _me?_me: _me = new DapServerSelector();}*/

    DapServerSelector();

    bool newServerAfterNetworkError();
};

#endif // DAPSERVERSELECTOR_H
