#include "DapKeyMsrln.h"
#include "dap_enc_msrln.h"
#include <QDebug>


DapKeyMsrln::DapKeyMsrln()
{

}

DapKeyMsrln::~DapKeyMsrln()
{
 //   dap_enc_key_delete(_key);
}

QByteArray DapKeyMsrln::generateAliceMessage()
{
    if(_key != Q_NULLPTR) {
        qWarning() << "DapKeyMsrln can't be generated twice";
        return Q_NULLPTR;
    }

    _key = dap_enc_key_new_generate(DAP_ENC_KEY_TYPE_MSRLN, NULL, 0, NULL, 0, 0);

    return QByteArray((char*)_key->pub_key_data, _key->pub_key_data_size);
}

QByteArray DapKeyMsrln::generateBobMessage(QByteArray aliceMessage)
{
    if(_key != Q_NULLPTR) {
        qWarning() << "DapKeyMsrln can't be generated twice";
        return Q_NULLPTR;
    }

    _key = dap_enc_key_new(DAP_ENC_KEY_TYPE_MSRLN);
    _key->gen_bob_shared_key(_key, aliceMessage.data(), (size_t)aliceMessage.size(),
                             (void**)&_key->pub_key_data);

    return QByteArray((char*)_key->pub_key_data, _key->pub_key_data_size);
}

void DapKeyMsrln::generateAliceSharedKey(QByteArray& bobMessage)
{
    if(_key == Q_NULLPTR) {
        qWarning() << "Alice message must be generate firstly";
        return;
    }
    _key->gen_alice_shared_key(_key, _key->priv_key_data, (size_t)bobMessage.size(),
                               (unsigned char*) bobMessage.data());
}

void DapKeyMsrln::encode(QByteArray &in, QByteArray &out)
{
    //aes_key->encode(in,out);
}

void DapKeyMsrln::decode(QByteArray &in, QByteArray &out)
{
    //aes_key->decode(in,out);
}

bool DapKeyMsrln::init(const QString &key)
{
    //aes_key = new DapKeyIaes();
    //return aes_key->init(key);
}
