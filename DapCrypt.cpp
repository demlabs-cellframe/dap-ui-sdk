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


#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#include "DapCrypt.h"
#include "DapKeyAes.h"
#include <QDebug>
#include "DapSession.h"

/**
 * @brief DapCrypt::DapCrypt
 * @details keySessionRsa - local RSA key
 * keySessionServer - public Server Key
 * keyStream - AES key getting from server
 */
DapCrypt::DapCrypt()
{
    keyClientSessionRsa = new DapKeyRsa;
    keySesionServer = new DapKeyRsa;
    keyServerPublic = new DapKeyRsa;
    keyStream = Q_NULLPTR;

#if OPENSSL_VERSION_NUMBER < 0x10100000L
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
#else
    //SSL_library_init();
#endif

}

DapKey* DapCrypt::roleToKey(KeyRole kRole)
{
    switch(kRole)
    {
        case KeyServerPublic: return keyServerPublic;
        case KeyRoleSession: return keyClientSessionRsa;
        case KeyRoleStream:   return keyStream;
        case KeyRoleSessionServer: return keySesionServer;
        default: return Q_NULLPTR;
    }
}


/**
 * @brief DapCrypt::readKeyFromBio
 * @param bio
 * @return
 */
QString DapCrypt::readKeyFromBio(BIO *bio)
{
    size_t length = BIO_pending(bio);
    char *buff = new char[length + 1];
    BIO_read(bio, buff, length);
    buff[length] = '\0';
    QString ret(buff);
    delete []buff;
    return ret;
}

QString DapCrypt::getRandomString(int lengthStr)
{
   const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

   QString randomString;

   for(int i = 0; i < lengthStr; ++i)
   {
       int index = qrand() % possibleCharacters.length();
       randomString.append(possibleCharacters.at(index));
   }
   return randomString;
}

void DapCrypt::encode(QByteArray& in, QByteArray& out, KeyRole kRole)
{
    DapKey * key = roleToKey(kRole);
    if( key == Q_NULLPTR )
    {
        qCritical() << "[DapCrypt] Error! No key for role! [" << kRole << "]";
        return;
    }
    key->encode(in,out);
}

void DapCrypt::encodeB64(const QString& in, QByteArray& out, KeyRole kRole)
{
    QByteArray dataEnc;
    QByteArray dataIn = in.toLatin1();
    encode(dataIn, dataEnc, kRole);
    out = dataEnc.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);
}

void DapCrypt::decode(QByteArray& in, QByteArray& out, KeyRole kRole)
{
    DapKey * key = roleToKey(kRole);
    if( key == Q_NULLPTR )
    {
        qCritical() << "[DapCrypt] Error! No key for role! [" << kRole << "]";
        return;
    }
    key->decode(in,out);
}

void DapCrypt::decodeB1k(QByteArray& in, QByteArray& out, int sizeBlock, KeyRole kRole)
{
    out.clear();
    QByteArray b1k, b1kDec;
    int b1kLength;
    for(int i = 0; i < in.length(); i += sizeBlock)
    {
        b1kLength = in.length() - i < sizeBlock ? in.length() - i : sizeBlock;
        b1k = in.mid(i, b1kLength);
        decode(b1k, b1kDec, kRole);
        out.append(b1kDec);
    }
}

/**
 * @brief DapCrypt::createRSAKey
 * @details Creates RSA key pair
 * @param kRole Key role
 * @return
 */
QString DapCrypt::createRSAKey()
{
    keyClientSessionRsa->generateRsaLocalKeyPair();
    return keyClientSessionRsa->getStringPubKey();
}

/**
 * @brief DapCrypt::addRSAKey
 * @details Add RSA key pair
 * @param kRole
 * @param a_keyStr
 */
bool DapCrypt::setRsaSessionServerKey(const QString &a_keyStr)
{
    return keySesionServer->init(a_keyStr);
}

void DapCrypt::initAesKey(QString &keyStr, KeyRole kRole)
{
    QString keyEnd;
    DapKey ** dapKey;
    switch(kRole)
    {
    case KeyRoleStream:
        dapKey = &keyStream;
        keyEnd = keyStr;
        break;
    default:
        qCritical() <<"[DapCrypt] initAESKEY: Wrong key role " << kRole;
        return;
    }

    if(*dapKey)
        delete *dapKey;

    *dapKey = new DapKeyAes;
    (*dapKey)->init(QString(keyStr));
}


size_t DapCrypt::b64_get_decodet_size(size_t in_size)
{
    return (in_size/4)*3 + ((in_size%4==2) ?1: (in_size%5==3) ? 2:0);
}

unsigned char DapCrypt::b64_byte_decode(unsigned char b)
{
    if (( b == '+' ) || (b =='-') )
        return 62;
    if( (b == '/' ) || (b == '_') )
        return 63;
    if( b <= '9' )
        return (b - '0' + 52);
    if(b <= 'Z')
        return (b - 'A');
    return (b - 'a' + 26);
}

void DapCrypt::Base64Decode(const char* source, size_t srcLen, unsigned char* out)
{
    unsigned char b1, b2, b3, b4;
    const unsigned char* srcBytes = (unsigned char*)source;
    unsigned char* dest = out;

    size_t dec_length = b64_get_decodet_size(srcLen);
    unsigned char *buffer = (unsigned char*)malloc(dec_length + 1);
    buffer[dec_length] = '\0';

    // walk through the source, taking 4 bytes at a time
    size_t source_index = 0;
    size_t dest_index = 0;
    size_t remaining = srcLen;
    for( ; remaining > 3; remaining -= 4 ) {
        b1 = b64_byte_decode(srcBytes[source_index++]);
        b2 = b64_byte_decode(srcBytes[source_index++]);
        b3 = b64_byte_decode(srcBytes[source_index++]);
        b4 = b64_byte_decode(srcBytes[source_index++]);

        dest[dest_index++] = (unsigned char)( ( b1 << 2 ) | ( b2 >> 4 ) );
        dest[dest_index++] = (unsigned char)( ( b2 << 4 ) | ( b3 >> 2 ) );
        dest[dest_index++] = (unsigned char)( ( b3 << 6 ) | b4 );
    }

    // process the remaining bytes
    b2 = b3 = 0;
    if( remaining > 0 ) {
        b1 = b64_byte_decode( srcBytes[source_index++] );
        if( remaining > 1 )
            b2 = b64_byte_decode( srcBytes[source_index++] );
        if( remaining == 3 )
            b3 = b64_byte_decode( srcBytes[source_index++] );

        dest[dest_index++] = (unsigned char)( ( b1 << 2 ) | ( b2 >> 4 ) );
        if( remaining == 3 )
            dest[dest_index++] = (unsigned char)( ( b2 << 4 ) | ( b3 >> 2 ) );
    }
}

size_t DapCrypt::fromBase64(QByteArray in, size_t in_size, unsigned char* out)
{
    Base64Decode(in.data(), in_size, out);
    return b64_get_decodet_size(in_size);
}

