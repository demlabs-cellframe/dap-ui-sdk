#include "DapKeyMsrln.h"
#include "dap_enc_key.h"
#include "dap_enc_msrln.h"
#include <QDebug>


DapKeyMsrln::DapKeyMsrln()
{

}

DapKeyMsrln::~DapKeyMsrln()
{
    if(_key != Q_NULLPTR)
        dap_enc_key_delete(_key);
    if (_sharedSessionKey)
        delete _sharedSessionKey;
}

QByteArray DapKeyMsrln::generateAliceMessage()
{
    if(_key != Q_NULLPTR) {
        dap_enc_key_delete(_key);
    }

    _key = dap_enc_key_new_generate(DAP_ENC_KEY_TYPE_MSRLN, NULL, 0, NULL, 0, 0);

    return QByteArray((char*)_key->pub_key_data, _key->pub_key_data_size);
}

bool DapKeyMsrln::generateAliceSharedKey(const QByteArray& bobMessage)
{
    _key->gen_alice_shared_key(_key, _key->priv_key_data, (size_t)bobMessage.size(),
                               (unsigned char*) bobMessage.data());
    return true;
}

QByteArray DapKeyMsrln::generateBobMessage(QByteArray aliceMessage)
{
    if(_key != Q_NULLPTR) {
        dap_enc_key_delete(_key);
    }

    _key = dap_enc_key_new(DAP_ENC_KEY_TYPE_MSRLN);
    _key->gen_bob_shared_key(_key, aliceMessage.data(), (size_t)aliceMessage.size(),
                             (void**)&_key->pub_key_data);

    return QByteArray((char*)_key->pub_key_data, _key->pub_key_data_size);
}

bool DapKeyMsrln::generateSessionKey(const QByteArray& bobMessage,
                                              const QByteArray& seed)
{
    _key->gen_alice_shared_key(_key, _key->priv_key_data, (size_t)bobMessage.size(),
                               (unsigned char*) bobMessage.data());
    if(_sharedSessionKey != Q_NULLPTR) {
        delete _sharedSessionKey;
        _sharedSessionKey = nullptr;
    }
    _sharedSessionKey = new DapKeyAes();

    _sharedSessionKey->init(seed, QByteArray((char*)_key->priv_key_data, _key->priv_key_data_size));

    return true;
}

void DapKeyMsrln::encode(QByteArray &in, QByteArray &out)
{
    _sharedSessionKey->encode(in,out);
}

void DapKeyMsrln::decode(QByteArray &in, QByteArray &out)
{
    _sharedSessionKey->decode(in,out);
}

bool DapKeyMsrln::init(const QString &key)
{
    (void)key;
    //aes_key = new DapKeyIaes();
    //return aes_key->init(key);
    return false;
}
