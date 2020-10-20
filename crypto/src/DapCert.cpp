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
#include "dap_enc_base64.h"
#include "dap_cert.h"
#include "dap_cert_file.h"
#include "DapCert.h"
#ifdef Q_OS_WIN
#include "registry.h"
#endif

using namespace Dap;
using namespace Dap::Crypto;

/**
 * @brief Cert::generate
 * @param a_type
 * @return
 */
Cert * Cert::generate(const QString& a_name, KeySignType a_type)
{
    dap_enc_init();
    Cert * ret = new Cert();
    ret->m_cert = dap_cert_generate_mem(a_name.toLatin1().constData() ,
                                                   KeySign::typeToEncType(a_type) );
    ret->m_key = new Key(ret->m_cert->enc_key);
    return ret;
}

Cert * Cert::generate(const QString& a_name, const QString& a_seed, KeySignType a_type)
{
    dap_enc_init();
    Cert * ret = new Cert();
    ret->m_cert = dap_cert_generate_mem_with_seed(a_name.toLatin1().constData() ,
                                                   KeySign::typeToEncType(a_type), qPrintable(a_seed), a_seed.length() );
    ret->m_key = new Key(ret->m_cert->enc_key);
    return ret;
}


/**
 * @brief Cert::Cert
 * @param a_filePath
 */
Cert* Cert::load(const QString& a_filePath )
{
    if (! a_filePath.isEmpty() ){
        dap_cert_t * l_cert = dap_cert_file_load(a_filePath.toLatin1().constData());
        if ( l_cert == nullptr){
            qCritical() << "Can't load cert "<<a_filePath;
            return nullptr;
        }else {
            Cert * ret = new Cert();
            ret->m_cert = l_cert;
            ret->m_key = new Key(ret->m_cert->enc_key);
            return ret;
        }
    }else
        return nullptr;
}

/**
 * @brief Cert::save
 * @param a_filePath
 * @return
 */
bool Cert::save(const QString & a_filePath)
{
    if (! a_filePath.isEmpty() ){
        return dap_cert_file_save(m_cert, a_filePath.toLatin1().constData())==0;
    }else
        return false;

}

/**
 * @brief Cert::Cert
 * @param a_certData
 */
Cert* Cert::load(const QByteArray& a_certData)
{
    if (! a_certData.isEmpty() ){
        dap_cert_t * l_cert = dap_cert_mem_load(a_certData.constData(),static_cast<size_t>(a_certData.size()) );
        if ( l_cert == nullptr){
            qCritical() << "Can't load cert from byte array (size "<< a_certData.size() << " )";
            return nullptr;
        }else{
            Cert * ret = new Cert();
            ret->m_cert = l_cert;
            ret->m_key = new Key(ret->m_cert->enc_key);
            return ret;
        }
    }else
        return nullptr;

}

/**
 * @brief Cert::~Cert
 */
Cert::~Cert()
{
    if ( m_cert != nullptr )
        dap_cert_delete( m_cert );
    if (m_key)
        delete m_key;
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
    qInfo() << "4554 pkey_size " << sign->header.sign_pkey_size << " sign_size " << sign->header.sign_size << "and total: " << a_output.size();
    DAP_DELETE (sign);
}

/**
 * @brief Cert::isSamePKey
 * @param a_data
 * @return
 */
bool Cert::compareWithSign(const QByteArray & a_data)
{
    return dap_cert_compare_with_sign(m_cert, reinterpret_cast<const dap_sign_t*>( a_data.constData() ) ) == 0;
}

/**
 * @brief Cert::exportPKeyBase64
 * @return
 */
QString Cert::exportPKeyBase64()
{
    if(!m_key){
        m_key = new Key(m_cert->enc_key);
        qWarning() << "pkey was updated from cert...";
    }
    size_t buflen = 0;
    uint8_t * buf = dap_enc_key_serealize_pub_key(*m_key, &buflen);
    if (!buf) {
        qWarning() << "Empty hash!";
        return QString();
    }
    char * buf64 = DAP_NEW_Z_SIZE(char, buflen * 2 + 6);
    size_t buf64len = dap_enc_base64_encode(buf, buflen, buf64, DAP_ENC_DATA_TYPE_B64_URLSAFE);
    QString ret = QString::fromLatin1(buf64, static_cast<int>(buf64len));
    DAP_DELETE(buf64);
    DAP_DELETE(buf);
    return ret;
}

bool Cert::exportPKeyToFile(const QString &a_path) {
    FILE *l_file = fopen(qPrintable(a_path), "wb");
    if (l_file) {
        size_t buflen = 0;
        uint8_t *buf = dap_enc_key_serealize_pub_key(m_cert->enc_key, &buflen);
        if (buf) {
            if (size_t l_ret = fwrite(buf, 1, buflen, l_file) != buflen) {
                qCritical() << "Error occured on pkey export: " << l_ret << " != " << buflen;
                fclose(l_file);
                DAP_DELETE(buf);
                return false;
            }
            fflush(l_file);
            fclose(l_file);
            DAP_DELETE(buf);
            qInfo() << "pkey exported";
            return true;
        }
    }
    qCritical() << "Couldn't export pkey";
    return false;
}

QString Cert::pkeyHash() {
    dap_chain_hash_fast_t l_hash_pkey, l_hash_cert_pkey;
    // serialized key -> key hash
    dap_hash_fast(((dap_enc_key_t*)(*m_key))->pub_key_data, ((dap_enc_key_t*)(*m_key))->pub_key_data_size, &l_hash_pkey);
    char *l_pkey_hash_str = dap_chain_hash_fast_to_str_new(&l_hash_pkey);
    dap_hash_fast(m_cert->enc_key->pub_key_data, m_cert->enc_key->pub_key_data_size, &l_hash_cert_pkey);
    char *l_cert_pkey_hash_str = dap_chain_hash_fast_to_str_new(&l_hash_cert_pkey);
    QString ret = QString("Key hash: %1, Certificate key hash: %2").arg(l_pkey_hash_str).arg(l_cert_pkey_hash_str);
    DAP_DEL_Z(l_pkey_hash_str);
    DAP_DEL_Z(l_cert_pkey_hash_str);
    return ret;
}

bool Cert::importPKeyFromFile(const QString &a_path) {
    FILE *l_file = fopen(qPrintable(a_path), "rb");
    if (!l_file) {
        return false;
    }
    fseek(l_file, 0L, SEEK_END);
    long l_len = ftell(l_file);
    fseek(l_file, 0L, SEEK_SET);
    uint8_t buf[l_len];
    fread(buf, 1, l_len, l_file);
    fclose(l_file);
    if ((dap_enc_key_deserealize_pub_key(*m_key, buf, l_len) != 0)
            /*|| dap_enc_key_deserealize_pub_key(m_cert->enc_key, buf, l_len) != 0*/) {
        delete m_key;
        m_key = new Key(m_cert->enc_key);
        qCritical() << "Error occured on deserialization";
        return false;
    }
    return true;
}

QString Cert::storagePath()
{
#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    return QString("/opt/%1").arg(DAP_BRAND).toLower();
#elif defined(Q_OS_MACOS)
    return QString("/tmp/%1").arg(DAP_BRAND);
#elif defined (Q_OS_WIN)
    return QString("%1/%2").arg(regWGetUsrPath()).arg(DAP_BRAND);
#elif defined Q_OS_ANDROID
    return QString("/sdcard/%1").arg(DAP_BRAND);
#endif
    return {};
}
