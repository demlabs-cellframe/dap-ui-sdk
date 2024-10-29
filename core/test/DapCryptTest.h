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
        DapCrypt dc;
        QString resultString =
                dc.getRandomString(exptectedStringLength);

        QCOMPARE(exptectedStringLength, resultString.length());
    }
};

#endif // DAPCRYPTTEST_H
