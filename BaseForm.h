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
