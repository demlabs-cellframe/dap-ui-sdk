/*
 Copyright (c) 2017-2018 (c) Project "DeM Labs Inc" https://github.com/demlabsinc
  All rights reserved.

 This file is part of DAP (Deus Applications Prototypes) the open source project

    DAP (Deus Applicaions Prototypes) is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DAP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with any DAP based project.  If not, see <http://www.gnu.org/licenses/>.
*/


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
