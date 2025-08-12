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
#include "dap_enc.h"
#include "dap_cert.h"
#include "dap_cert_file.h"
#include "DapCert.h"
#include "dap_enc.h"
#ifdef Q_OS_WIN
#include "registry.h"
#endif

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QAndroidJniObject>
#endif
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#ifndef O_BINARY
# define O_BINARY 0
#endif

using namespace Dap;
using namespace Dap::Crypto;

/**
 * @brief Cert::Cert
 */
void Cert::init()
{
    dap_enc_init();
}

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

Cert * Cert::generate(const QString& a_name, const QString& a_seed, KeySignType a_type)
{
    Cert * ret = new Cert();
    ret->m_cert = dap_cert_generate_mem_with_seed(a_name.toLatin1().constData() ,
                                                   KeySign::typeToEncType(a_type), qPrintable(a_seed), a_seed.length() );
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
    if ( m_cert != nullptr ) {
        dap_cert_delete( m_cert );
        m_cert = nullptr;
    }
}

/**
 * @brief Cert::sign
 * @param a_data
 * @param a_output
 */
void Cert::sign(const QByteArray & a_data, QByteArray & a_output)
{
    // Create certificate signature using default hash type (DAP_SIGN_HASH_TYPE_DEFAULT)
    dap_sign_t * sign = dap_cert_sign( m_cert, a_data.constData(), static_cast<size_t>(a_data.size()) );
    qInfo() << "Cert::sign call with pkey_size " << sign->header.sign_pkey_size << " sign_size " << sign->header.sign_size << "and total: " << dap_sign_get_size(sign);
    a_output.append(  QByteArray( reinterpret_cast<char*>(sign), static_cast<int>(dap_sign_get_size( sign )) ));
    DAP_DEL_Z(sign)
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
    // Add comprehensive validation before calling dap_enc_key_serialize_pub_key
    if (!m_cert) {
        qWarning() << "Certificate is null - cannot export public key";
        return QString();
    }
    
    if (!m_cert->enc_key) {
        qWarning() << "Certificate encryption key is null - cannot export public key";
        return QString();
    }
    
    if (!m_cert->enc_key->pub_key_data) {
        qWarning() << "Certificate public key data is null - cannot export public key";
        return QString();
    }
    
    if (m_cert->enc_key->pub_key_data_size == 0) {
        qWarning() << "Certificate public key data size is zero - cannot export public key";
        return QString();
    }
    
    size_t buflen = 0;
    uint8_t * buf = dap_enc_key_serialize_pub_key(m_cert->enc_key, &buflen);
    if (!buf) {
        qWarning() << "Failed to serialize public key - key structure may be corrupted";
        return QString();
    }
    char * buf64 = DAP_NEW_Z_SIZE(char, buflen * 2 + 6);
    size_t buf64len = dap_enc_base64_encode(buf, buflen, buf64, DAP_ENC_DATA_TYPE_B64_URLSAFE);
    DAP_DELETE(buf);
    return QString::fromLatin1(buf64, static_cast<int>(buf64len));
}

int Cert::exportPKeyToFile(const QString &a_path) {
    // Add validation before calling dap_enc_key_serialize_pub_key
    if (!m_cert || !m_cert->enc_key || !m_cert->enc_key->pub_key_data || m_cert->enc_key->pub_key_data_size == 0) {
        qWarning() << "Certificate or encryption key is invalid - cannot export to file";
        return 3;
    }
    
    int res = -1;
    FILE *l_file = fopen(qPrintable(a_path), "wb");
    if (!l_file) {
        return 1;
    }
    size_t buflen = 0;
    uint8_t *buf = dap_enc_key_serialize_pub_key(m_cert->enc_key, &buflen);
    if (!buf) {
        fclose(l_file);
        return 2;
    }
    if (size_t l_ret = fwrite(buf, 1, buflen, l_file) == buflen) {
        res = 0;
        fflush(l_file);
    }
    fclose(l_file);
    DAP_DELETE(buf);
    return res;
}

QString Cert::pkeyHash() {
    // Add validation before accessing encryption key data
    if (!m_cert || !m_cert->enc_key || !m_cert->enc_key->pub_key_data || m_cert->enc_key->pub_key_data_size == 0) {
        qWarning() << "Certificate or encryption key is invalid - cannot generate key hash";
        return QString();
    }
    
    dap_chain_hash_fast_t l_hash_cert_pkey;
    dap_hash_fast(m_cert->enc_key->pub_key_data, m_cert->enc_key->pub_key_data_size, &l_hash_cert_pkey);
    char *l_cert_pkey_hash_str = dap_chain_hash_fast_to_str_new(&l_hash_cert_pkey);
    QString ret = QString::fromLatin1(l_cert_pkey_hash_str);
    DAP_DEL_Z(l_cert_pkey_hash_str)
    return ret;
}

int Cert::importPKeyFromFile(const QString &a_path) {
    int l_file = open(qPrintable(a_path), O_RDONLY | O_BINARY);
    struct stat statBuf;
    fstat(l_file, &statBuf);
    size_t l_len = statBuf.st_size;
    uint8_t buf[l_len];
    read(l_file, buf, l_len);
    close(l_file);

    dap_enc_key_t *l_temp =  dap_enc_key_new(m_cert->enc_key->type);
    if ( dap_enc_key_deserialize_pub_key(l_temp, buf, l_len) != 0) {
        dap_enc_key_delete(l_temp);
        return 2;
    }
    dap_enc_key_delete(l_temp);
    int desrl_res = dap_enc_key_deserialize_pub_key(key(), buf, l_len);
    return desrl_res;
}


dap_enc_key_t* Cert::key() {
    return m_cert->enc_key;
}

void Cert::setPubKey(dap_enc_key_t* a_key) {
    m_cert = dap_cert_new("public");
    m_cert->enc_key = dap_enc_key_new(a_key->type);
    m_cert->enc_key->pub_key_data = DAP_NEW_Z_SIZE(void, a_key->pub_key_data_size);
    memcpy(m_cert->enc_key->pub_key_data, a_key->pub_key_data, a_key->pub_key_data_size);
    m_cert->enc_key->pub_key_data_size = a_key->pub_key_data_size;
}

void Cert::savePubCert(const char * saveDir, const char * newName) {

    if ( m_cert ) {
        if ( m_cert->enc_key->pub_key_data_size ) {
          // Create empty new cert
          dap_cert_t * l_cert_new = dap_cert_new(newName);
          l_cert_new->enc_key = dap_enc_key_new( m_cert->enc_key->type);

          // Copy only public key
          l_cert_new->enc_key->pub_key_data = DAP_DUP_SIZE(DAP_CAST_PTR(char, m_cert->enc_key->pub_key_data),
                                                           m_cert->enc_key->pub_key_data_size);
          if(!l_cert_new->enc_key->pub_key_data) {
            qDebug() << "Memory allocation error";
            return;
          }
          l_cert_new->enc_key->pub_key_data_size = m_cert->enc_key->pub_key_data_size;

          dap_cert_save_to_folder(l_cert_new, saveDir);
        } else {
          qDebug() <<  "Can't produce pkey from this cert type";
          exit(-7023);
        }
    } else {
        exit(-7021);
    }
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
    static QAndroidJniObject l_pathObj = QtAndroid::androidContext().callObjectMethod(
                "getExternalFilesDir"
                , "(Ljava/lang/String;)Ljava/io/File;"
                , QAndroidJniObject::fromString(QString("")).object());
    return l_pathObj.toString();
#endif
    return {};
}
