#include "DapKeyMsrln.h"
#include "liboqs/kex_rlwe_msrln16/kex_rlwe_msrln16.h"
#include <QDebug>
#include <QFile>


DapKeyMsrln::DapKeyMsrln()
{

}

QByteArray DapKeyMsrln::generateAliceMessage()
{
    uint8_t* out_msg = NULL;
    size_t out_msg_size = 0;
    OQS_KEX_rlwe_msrln16_alice_0(kex,&private_key,&out_msg,&out_msg_size);
    return QByteArray((char*)out_msg,out_msg_size);
}

QByteArray DapKeyMsrln::generateBobMessage(QByteArray aliceMessage)
{
    uint8_t* out_msg = NULL;
    size_t out_msg_size = 0;
    OQS_KEX_rlwe_msrln16_bob(kex,(unsigned char*)aliceMessage.data(),1824,&out_msg,&out_msg_size,&public_key,&public_length);
    return QByteArray((char*)out_msg,out_msg_size);
}

bool DapKeyMsrln::makePublicKey(QByteArray& bobMessage)
{
    if(OQS_KEX_rlwe_msrln16_alice_1(kex, private_key,(unsigned char*)bobMessage.data(), 2048,&public_key,&public_length) == 0)
        return false;
    convertToAes();
    return true;
}

void DapKeyMsrln::convertToAes(){
    aes_key = new DapKeyAes();
    aes_key->initKeyChar((char*)public_key);
}

void DapKeyMsrln::prepare()
{
    rand = OQS_RAND_new(OQS_RAND_alg_urandom_chacha20);
    kex = OQS_KEX_rlwe_msrln16_new(rand);
}

void DapKeyMsrln::encode(QByteArray &in, QByteArray &out)
{
    aes_key->encode(in,out);
}

void DapKeyMsrln::decode(QByteArray &in, QByteArray &out)
{
    aes_key->decode(in,out);
}

bool DapKeyMsrln::init(const QString &key)
{
    aes_key = new DapKeyAes();
    return aes_key->init(key);
}
