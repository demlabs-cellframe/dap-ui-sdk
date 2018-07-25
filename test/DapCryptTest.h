#ifndef DAPCRYPTTEST_H
#define DAPCRYPTTEST_H

#include <QTest>
#include "DapCrypt.h"

class DapCryptTest : public QObject {
    Q_OBJECT
private:

private slots:
    void randomString() {
        const int exptectedStringLength = 22;
        QString resultString =
                DapCrypt::me()->getRandomString(exptectedStringLength);

        QCOMPARE(exptectedStringLength, resultString.length());
    }
};

#endif // DAPCRYPTTEST_H
