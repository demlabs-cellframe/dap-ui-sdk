#include <QString>
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>

#include "DapKeyCommon.h"

DapKey::DapKey() {

}

DapKey::DapKey(dap_enc_key_type_t a_type, const QString& a_kexBuf) {
    m_key = dap_enc_key_new_generate(a_type, qPrintable(a_kexBuf), size_t(a_kexBuf.length())
                                     , nullptr, 0, 32);
}

DapKey::DapKey(dap_enc_key_type_t a_type, const QByteArray &a_kexBuf, const QByteArray &a_seed) {
    m_key = dap_enc_key_new_generate(a_type, a_kexBuf.constData(), size_t(a_kexBuf.length())
                                     , a_seed.constData(), size_t(a_seed.size()), 32);
}

DapKey::DapKey(dap_enc_key_t *a_key) {
    dap_enc_key_serealize_t* tmp = dap_enc_key_serealize(a_key);
    m_key = dap_enc_key_deserealize(tmp, sizeof(dap_enc_key_serealize_t));
    DAP_DEL_Z(tmp)
}

DapKey::~DapKey() {
    dap_enc_key_delete(m_key);
}

void DapKey::encode(const QByteArray &a_in, QByteArray &a_out) {
    if(m_key == nullptr) {
        qCritical() << "Encoding error, key is null";
        return;
    }

    static char l_encBuf[BUF_SIZE];
    memset(l_encBuf, '\0', BUF_SIZE);
    size_t enc_size = m_key->enc_na(m_key, a_in, a_in.size(), l_encBuf, BUF_SIZE);
    a_out = QByteArray(l_encBuf, enc_size);
}

void DapKey::decode(const QByteArray &a_in, QByteArray &a_out) {
    if(m_key == nullptr) {
        qCritical() << "Encoding error, key is null";
        return;
    }

    static char l_encBuf[BUF_SIZE];
    memset(l_encBuf, '\0', BUF_SIZE);
    size_t enc_size = m_key->dec_na(m_key, a_in, a_in.size(), l_encBuf, BUF_SIZE);
    a_out = QByteArray(l_encBuf, enc_size);
}
