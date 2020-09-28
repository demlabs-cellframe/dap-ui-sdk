/*
* Authors:
* Dmitriy Gerasimov <naeper@demlabs.net>
* Cellframe       https://cellframe.net
* DeM Labs Inc.   https://demlabs.net
* Copyright  (c) 2017-2019
* All rights reserved.

This file is part of CellFrame SDK the open source project

CellFrame SDK is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

CellFrame SDK is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with any CellFrame SDK based project.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once
#include <QString>
#include <QByteArray>
#include "DapKeySign.h"

typedef struct dap_cert dap_cert_t;

namespace Dap {
    namespace  Crypto{
        class Cert
        {
        protected:
            dap_cert_t * m_cert = nullptr;
            Key *m_key = nullptr;
            Cert(){ }
        public:
            static Cert * generate(const QString& a_name, KeySignType a_type);
            static Cert * generate(const QString& a_name, const QString& a_seed, KeySignType a_type);
            static Cert * load(const QByteArray& a_certData);
            static Cert * load(const QString& a_filePath);
            ~Cert();

            Key& key() {  return  *m_key; }
            void sign(const QByteArray & a_data, QByteArray & a_output);
            bool compareWithSign(const QByteArray & a_data);

            bool save(const QString & a_filePath);
            QString exportPKeyBase64();
            bool exportPKeyToFile   (const QString&);
            bool importPKeyFromFile (const QString&);
            QString storagePath     ();
        };
    }
}
