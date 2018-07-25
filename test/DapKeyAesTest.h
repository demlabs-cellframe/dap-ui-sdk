#ifndef DAPKEYAESTEST_H
#define DAPKEYAESTEST_H
#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QTest>

#include "DapKeyAes.h"

class DapKeyAesTest : public QObject {
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

    void encodeDecode() {
        DapKeyAes dka;
        const char aes_key[AES_KEY_LENGTH] = {
            's','b','s','s','q','e',
            's','b','s','s','q','e',
            '2', 's'
        };
        dka.initKeyChar(aes_key);
        QByteArray encode, decode;
        for(int i = 0; i < 10; i++ ) {
            QByteArray source = getRandomBytes(1 + (qrand() % 255));
            dka.encode(source, encode);
            dka.decode(encode, decode);
            QVERIFY(QString(source) == QString(decode));
        }
    }
};

#endif // DAPKEYAESTEST_H
