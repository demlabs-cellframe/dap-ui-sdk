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

#include "dap_hash.h"
#include <QString>

namespace Dap {
    namespace Crypto {
        class HashFast
        {
        private:
            dap_chain_hash_fast_t m_value;
        public:
            const dap_chain_hash_fast_t& value() const { return m_value; }
            HashFast(){ ::memset(m_value.raw,0,sizeof (m_value)); }
            HashFast(const dap_chain_hash_fast_t& a_value) { memcpy(&m_value, &a_value, sizeof (a_value)); }
            HashFast(const HashFast& a_hashFast) { memcpy(&m_value, &a_hashFast.m_value, sizeof (m_value)); }
            HashFast(const QString& a_value) {
                if (dap_chain_hash_fast_from_str(qUtf8Printable(a_value), &m_value) != 0)
                    memset(m_value.raw, 0, sizeof (m_value));
            }

            bool operator==(const HashFast& a_hashFast) { return memcmp(&m_value, &a_hashFast.m_value, sizeof (m_value))==0; }
            bool operator==(const dap_chain_hash_fast_t& a_value) { return memcmp(&m_value, &a_value, sizeof (m_value))==0; }
            HashFast& operator =(const dap_chain_hash_fast_t& a_value) { memcpy(&m_value, &a_value, sizeof (m_value)); return *this;}
            HashFast& operator =(const HashFast& a_hashFast) { memcpy(&m_value, &a_hashFast.m_value, sizeof (m_value)); return *this;}
            bool isFilled() { return m_value.raw[0] != 0; }
        };
    }
}
