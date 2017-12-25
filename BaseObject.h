#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <QObject>

class BaseObject : public QObject
{
    Q_OBJECT
public:
    explicit BaseObject(QObject *parent = 0);
public Q_SLOTS:
    void connectSignals(QObject * rcv);
    void passSignals(BaseObject * rcv);
signals:
    void status(const QString& str, int timeout = 10000);
    void error(const QString& msg);
    void notify(const QString& msg);
    void progressUpdate(int value);
    void progressOn(const QString & msg = "Progress");
    void progressOff();
};

#endif // BASEOBJECT_H
