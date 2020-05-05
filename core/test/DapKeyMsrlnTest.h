#ifndef DAPKEYMSRLNTEST_H
#define DAPKEYMSRLNTEST_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QTest>
#include "DapKeyMsrln.h"

class DapKeyMsrlnTest : public QObject {
    Q_OBJECT
private:
    QByteArray getRandomBytes(const int stringLength) const
    {
       const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
       const int randomStringLength = stringLength;

       QString randomString;
       for(int i = 0; i<randomStringLength; ++i)
       {
           int index = qrand() % possibleCharacters.length();
           QChar nextChar = possibleCharacters.at(index);
           randomString.append(nextChar);
       }
       return randomString.toLatin1();
    }
private slots:
    void initTestCase() {
        QDateTime currentDateTime = QDateTime::currentDateTime();
        qsrand(QDateTime::currentDateTime().toTime_t());
    }

    void transfer_keys() {
        DapKeyMsrln aliceKey, bobKey;
        QByteArray aliceMsg = aliceKey.generateAliceMessage();
        QByteArray bobMsg = bobKey.generateBobMessage(aliceMsg);
        aliceKey.generateAliceSharedKey(bobMsg);
        QVERIFY(memcmp(aliceKey._key->priv_key_data, bobKey._key->priv_key_data,
                       aliceKey._key->priv_key_data_size) == 0);
    }
};


#endif // DAPKEYMSRLNTEST_H
