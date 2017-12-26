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


#include "DapKeyRsa.h"
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/rand.h>

DapKeyRsa::DapKeyRsa()
{
    keyPair = NULL;
    pubKey = NULL;

    srand(time(NULL));

    //    OPENSSL_config(NULL);
}

DapKeyRsa::~DapKeyRsa()
{
    if (pubKey)
        RSA_free(pubKey);
    if(keyPair)
        RSA_free(keyPair);
}


/**
 * @brief DapKeyRsa::setStrPubKey
 */
inline bool DapKeyRsa::setStrPubKey()
{

    if(pubKey == NULL)
    {
        qWarning() << "[DapKeyRsa] setStrPubKey() error!";
        return false;
    }

    BIO *bio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(bio, pubKey);

    size_t key_len = BIO_pending(bio);
    char *out = new char[key_len + 1];

    BIO_read(bio, out, key_len);
    out[key_len] = '\0';

    strPubKey = QString(out);

    delete[] out;
    BIO_free_all(bio);
    return true;
}

/**
 * @brief DapKeyRsa::generateRsaLocalKeyPair
 * @details generate local Key Pair and get Public Key
 */
void DapKeyRsa::generateRsaLocalKeyPair()
{

   /* if(keyPair != NULL || pubKey != NULL)
    {
        qWarning() << "[DapKeyRsa] generateRsaLocalKeyPair Error Keys already exists!";
        return;
    }*/
    if (pubKey)
        RSA_free(pubKey);
    if(keyPair)
        RSA_free(keyPair);


    keyPair = RSA_generate_key(RSA_KEY_LENGTH, PUB_EXP, NULL, NULL);

    /* Set PubKey from keyPair */

    if(keyPair == NULL)
    {
        qDebug() << "[DapKeyRsa] generateRsaLocalKeyPair() Error!";
        return;
    }

    BIO *bio = BIO_new(BIO_s_mem());
    pubKey = NULL;
    PEM_write_bio_RSAPublicKey(bio, keyPair);
    PEM_read_bio_RSAPublicKey( bio, &pubKey, NULL, NULL);
    PEM_write_bio_RSAPublicKey(bio, keyPair);

    BIO_free_all(bio);

    setStrPubKey();

    if(pubKey == NULL)
    {
        qDebug() << "[DapKeyRsa] generateRsaLocalKeyPair() Error!";
    }
}

/**
 * @brief DapKeyRsa::decode
 * @param in
 * @param out
 * @details Decode Encrypt Rsa by localKey
 */
void DapKeyRsa::decode(QByteArray& in, QByteArray& out)
{
    int decrypt_len;

    unsigned char *msg_out = (unsigned char*) malloc (sizeof(char)*in.size());
    memset(msg_out, 0, sizeof(char)*in.size());

    Q_ASSERT(msg_out);

    if (in.size() <= RSA_CHIPER_BLOCK_SIZE)
    {
        //qDebug() <<"~~BL1";
        if((decrypt_len = RSA_private_decrypt(in.size(), (unsigned char*)in.data(), msg_out,
                                             keyPair, RSA_PKCS1_PADDING)) == -1)
        {
            qWarning() << "[DapKeyRsa]Error Decrypt RSA";
        }
    }
    else
    {
        //qDebug() <<"~~BL2";

        size_t out_size = sizeof(char)*in.size();
        size_t p = 0, i = 0, end = in.size();
        while (i < end)
        {
            if (p>=out_size) {
                msg_out = (unsigned char*)realloc(msg_out, out_size*2);
                Q_ASSERT(msg_out);
                memset(msg_out+out_size, 0, out_size-1);
                out_size *=2;
            }

            size_t bl_size = (end <= RSA_CHIPER_BLOCK_SIZE) ? in.size()-i
                                                            : (int)RSA_CHIPER_BLOCK_SIZE;

            //qDebug() << "decoding part ["<<(char*)in.data()+i<<"]";
            decrypt_len = RSA_private_decrypt(bl_size, (unsigned char*)in.data()+i, msg_out+p,
                                              keyPair, RSA_PKCS1_PADDING);
            if (decrypt_len < 0)
                qWarning() << "[DapKeyRsa]Error Decrypt RSA";

            p += decrypt_len;
            i += bl_size;

        }
        decrypt_len = p;
    }

    out.clear();
    out.append((const char*)msg_out, decrypt_len);
    free(msg_out);
}


/**
 * @brief DapKeyRsa::encode
 * @param in
 * @param out
 */
void DapKeyRsa::encode(QByteArray& in, QByteArray& out)
{
    int encrypt_len, part = 255;

    if(pubKey == NULL)
    {
        qWarning() << "[DapKeyRsa] encode error! PubKey == NULL";
        return;
    }

    unsigned char *msg_out;

    if (in.size() <= part) {
        msg_out = new unsigned char[RSA_size(pubKey)];
        if((encrypt_len = RSA_public_encrypt(in.size(), (unsigned char*)in.data(), msg_out,
                                                 pubKey, RSA_PKCS1_PADDING)) == -1)
            qWarning() << "[DapKeyRsa]Error Encrypt RSA";
    } else {
        msg_out = new unsigned char[in.size()];
        size_t _enc_size = 0;
        size_t end;
        for (size_t i = 0; i<(size_t)in.size() ;) {
            end = in.size() - i;

            size_t copy_size = (end<=(size_t)part) ? end : part;

            char buf[part]; memset(buf, 0, part);
            memcpy(buf, in.data()+i, copy_size);
            i+=copy_size;

            encrypt_len = RSA_public_encrypt(copy_size, (unsigned char*)buf, msg_out+_enc_size,
                                             pubKey, RSA_PKCS1_PADDING);

            if(!encrypt_len) {
                qWarning() << "[DapKeyRsa]Error Encrypt RSA";
            }

            _enc_size += encrypt_len;
        }
        encrypt_len = _enc_size;
    }


    out.clear();
    out.append((const char*)msg_out, encrypt_len);

    delete[] msg_out;
}


/**
 * @brief DapKeyRsa::init
 * @param key
 * @details init RSA Pub key by QString
 */
bool DapKeyRsa::init(const QString& key)
{
    if (pubKey != NULL)
    {
        qWarning() << "[DapKeyRsa] Change Rsa Pub Key";
        RSA_free(pubKey);
    }

    qDebug() << "[DapKeyRsa::init] key = " << key;
    BIO *bio = BIO_new(BIO_s_mem());

    BIO_write(bio, key.toStdString().c_str(), key.size());

    pubKey = NULL;
    PEM_read_bio_RSAPublicKey(bio, &pubKey, NULL, NULL);

    BIO_free_all(bio);

    if (!setStrPubKey()) {
        return false;
    }

    if (key == NULL) {
        qWarning() << "[DapKeyRsa] init() failed";
        return false;
    }
    return true;
}
