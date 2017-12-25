#include "BaseForm.h"

/* NEED TEST */
BaseForm::BaseForm(QObject *w) : widget(w)
{
    auto widgConn = [&] (const char *sig, const char *slt)
    {
        connect(this, sig, widget, slt);
    };
    connect(this,SIGNAL(destroyed(QObject*)), this,SLOT(destroyUi()));
    widgConn(SIGNAL(sigHide()),SLOT(hide()));
    widgConn(SIGNAL(sigShow()),SLOT(show()));
    widgConn(SIGNAL(sigUpdate()),SLOT(update()));
}
