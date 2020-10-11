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

#include <QByteArray>
#include "DapKey.h"
#include "dap_enc_key.h"
#include "dap_sign.h"

namespace Dap {
    namespace  Crypto{
        enum KeySignType { KeySignTypeTesla , KeySignTypeDilithium, KeySignTypeBliss, KeySignTypePicnic , KeySignTypeUndefined = -1};
        class KeySign: public Key
        {
        private:
            KeySign():Key(){}
        public:
            KeySignType type();
            static KeySign* generate(KeySignType a_type);
            static dap_enc_key_type_t typeToEncType( KeySignType a_type);
            static KeySignType encTypeToType(dap_enc_key_type_t a_encType);
            void sign(const QByteArray & a_data, QByteArray & a_output);

        };
    }
}
