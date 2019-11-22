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
#include <QFile>
#include <QtDebug>
#include "dap_common.h"
#include "dap_cert.h"
#include "dap_cert_file.h"
#include "DapCert.h"

using namespace Dap;
using namespace Dap::Crypto;

/**
 * @brief Cert::generate
 * @param a_type
 * @return
 */
Cert * Cert::generate(const QString& a_name, KeySignType a_type)
{
    Cert * ret = new Cert();
    ret->m_cert = dap_cert_generate_mem(a_name.toLatin1().constData() ,
                                                   KeySign::typeToEncType(a_type) );
    return ret;
}

/**
 * @brief Cert::Cert
 * @param a_filePath
 */
Cert::Cert(const QString& a_filePath )
{
    if (! a_filePath.isEmpty() ){
        m_cert = dap_cert_file_load(a_filePath.toLatin1().constData());
        if ( m_cert == nullptr)
            qCritical() << "Can't load cert "<<a_filePath;
    }
}

/**
 * @brief Cert::Cert
 * @param a_certData
 */
Cert::Cert(const QByteArray& a_certData)
{
    if (! a_certData.isEmpty() ){
        m_cert = dap_cert_mem_load(a_certData.constData(),static_cast<size_t>(a_certData.size()) );
        if ( m_cert == nullptr)
            qCritical() << "Can't load cert from byte array (size "<< a_certData.size() << " )";
    }

}

/**
 * @brief Cert::~Cert
 */
Cert::~Cert()
{
    if ( m_cert != nullptr )
        dap_cert_delete( m_cert );
}

/**
 * @brief Cert::sign
 * @param a_data
 * @param a_output
 */
void Cert::sign(const QByteArray & a_data, QByteArray & a_output)
{
    dap_sign_t * sign = dap_cert_sign( m_cert, a_data.constData(), static_cast<size_t>(a_data.size()), 0 );
    a_output.append(  QByteArray( reinterpret_cast<char*>(sign), static_cast<int>(dap_sign_get_size( sign )) ));
    DAP_DELETE (sign);
}

/**
 * @brief Cert::isSamePKey
 * @param a_data
 * @return
 */
bool Cert::compareWithSign(const QByteArray & a_data)
{
    return dap_cert_compare_with_sign(m_cert, reinterpret_cast<const dap_sign_t*>( a_data.data() ) ) == 0;
}
