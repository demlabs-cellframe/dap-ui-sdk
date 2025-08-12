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

#include <QtDebug>
#include "dap_enc.h"
#include "dap_sign.h"
#include "DapKeySign.h"

using namespace Dap;
using namespace Dap::Crypto;

KeySignType KeySign::encTypeToType(dap_enc_key_type_t a_encType)
{
    switch ( a_encType) {
        case DAP_ENC_KEY_TYPE_SIG_PICNIC: return  KeySignTypePicnic;  // signature based on zero-knowledge proof as specified in
                        // Post-Quantum Zero-Knowledge and Signatures from Symmetric-Key Primitives
                        // (Melissa Chase and David Derler and Steven Goldfeder and Claudio Orlandi
                        // and Sebastian Ramacher and Christian Rechberger and Daniel Slamanig and Greg Zaverucha
                        // https://eprint.iacr.org/2017/279.pdf), using the optimized implemenation
                        //  from https://github.com/IAIK/Picnic
        case DAP_ENC_KEY_TYPE_SIG_BLISS: return KeySignTypeBliss;  // signature based on zero-knowledge proof as specified in
                        // Post-Quantum Zero-Knowledge and Signatures from Symmetric-Key Primitives

        case DAP_ENC_KEY_TYPE_SIG_TESLA: return KeySignTypeTesla;  // signature based on Ring_LWE problem with zero-knowledge proof as specified in
                        // Post-Quantum Zero-Knowledge and Signatures from Symmetric-Key Primitives

        case DAP_ENC_KEY_TYPE_SIG_DILITHIUM: return  KeySignTypeDilithium;
        default: return KeySignTypeUndefined;
    }
}

/**
 * @brief KeySign::typeToEncType
 * @param a_type
 * @return
 */
dap_enc_key_type_t KeySign::typeToEncType( KeySignType a_type)
{
    switch (a_type) {
        case KeySignTypeTesla:       return DAP_ENC_KEY_TYPE_SIG_TESLA;
        case KeySignTypeDilithium:   return DAP_ENC_KEY_TYPE_SIG_DILITHIUM;
        case KeySignTypeBliss:       return DAP_ENC_KEY_TYPE_SIG_BLISS;
        case KeySignTypePicnic:      return DAP_ENC_KEY_TYPE_SIG_PICNIC;
        default:                     return  DAP_ENC_KEY_TYPE_NULL;
    }

}

/**
 * @brief KeySign::generate
 * @param a_type
 * @return
 */
KeySign* KeySign::generate(KeySignType a_type)
{
    KeySign * ret = new KeySign();
    ret->m_key = dap_enc_key_new_generate(typeToEncType(a_type),nullptr,0,nullptr,0,0);
    return ret;
}

/**
 * @brief KeySign::type
 * @return
 */
KeySignType KeySign::type()
{
    if ( m_key == nullptr )
        return  KeySignTypeUndefined;
    return encTypeToType(m_key->type);
}


/**
 * @brief KeySign::sign
 * @param a_data
 * @param a_output
 */
void KeySign::sign(const QByteArray & a_data, QByteArray & a_output)
{
    // Create signature using default hash type (DAP_SIGN_HASH_TYPE_DEFAULT)
    dap_sign_t * ret = dap_sign_create( m_key, a_data.constData(), static_cast<size_t>(a_data.size()) );
    a_output.append(  QByteArray( reinterpret_cast<char*>(ret), static_cast<int>(dap_sign_get_size( ret )) ));
}
