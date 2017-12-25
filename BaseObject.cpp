#include "BaseObject.h"

BaseObject::BaseObject(QObject *parent) : QObject(parent)
{

}

void BaseObject::connectSignals(QObject * rcv)
{
    connect(this,SIGNAL(error(QString)),rcv, SLOT(raiseErrorDialog(QString)));
    connect(this,SIGNAL(notify(QString)),rcv, SLOT(raiseNotifyDialog(QString)));
    connect(this,SIGNAL(status(QString,int)),rcv,SLOT(onStatusUpdate(QString,int)));
    connect(this,SIGNAL(progressOn(QString)),rcv,SLOT(raiseProgressDialog(QString)));
    connect(this,SIGNAL(progressOff()),rcv,SLOT(hideProgressDialog()));
    connect(this,SIGNAL(progressUpdate(int)),rcv,SLOT(updateProgress(int)));
}


void BaseObject::passSignals(BaseObject * rcv)
{
    connect(this,SIGNAL(error(QString)),rcv, SIGNAL(error(QString)));
    connect(this,SIGNAL(notify(QString)),rcv, SIGNAL(notify(QString)));
    connect(this,SIGNAL(status(QString,int)),rcv,SIGNAL(status(QString,int)));
    connect(this,SIGNAL(progressOn(QString)),rcv,SIGNAL(progressOn(QString)));
    connect(this,SIGNAL(progressOff()),rcv,SIGNAL(progressOff()));
    connect(this,SIGNAL(progressUpdate(int)),rcv,SIGNAL(progressUpdate(int)));
}
