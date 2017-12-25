#ifndef BASEFORM_H
#define BASEFORM_H

#include <QObject>
#include "BaseObject.h"

class BaseForm : BaseObject
{
    Q_OBJECT
public:
    BaseForm(QObject *w);
    virtual ~BaseForm() { }
protected:
    QObject* widget;

public slots:
    virtual void setupUi() = 0;
    virtual void destroyUi() = 0;

    void hide() { emit sigHide(); }
    void show() { emit sigShow(); }
    void update() { emit sigUpdate(); }
signals:
    void sigHide();
    void sigShow();
    void sigUpdate();
};

#endif // BASEFORM_H
