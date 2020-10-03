#ifndef DAPKEY_H
#define DAPKEY_H

#include <QByteArray>
#include "InterfaceDapKey.h"
#include "dap_enc_key.h"

#define BUF_SIZE 524288

class DapKey : public DapKeyAbstract
{
private:
    dap_enc_key_t *m_key;
    friend class DapCrypt;
public:
    DapKey();
    DapKey(dap_enc_key_t *a_key);
    DapKey(dap_enc_key_type_t a_type, const QString &a_kexBuf);
    DapKey(dap_enc_key_type_t a_type, const QByteArray &a_kexBuf, const QByteArray &a_seed);

    ~DapKey() override;

    void encode(const QByteArray &in, QByteArray &out) override;
    void decode(const QByteArray &in, QByteArray &out) override;
};

#endif
