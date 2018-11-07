#ifndef DAPKEYAESTEST_H
#define DAPKEYAESTEST_H
#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QTest>

#include "DapKeyIaes.h"

class DapKeyIaesTest : public QObject {
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
        DapKeyIaes dka;
        QByteArray seed = getRandomBytes(256);
        QByteArray kex_buf = getRandomBytes(256);
        dka.init(seed, kex_buf);


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
