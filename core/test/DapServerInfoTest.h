#ifndef DAPSERVERINFOTEST_H
#define DAPSERVERINFOTEST_H

#include <QTest>
#include "DapServerInfo.h"
#include <QJsonDocument>

class DapServerInfoTest : public QObject {
    Q_OBJECT
private:
    void compareExpResultServs(const DapServerInfo& lhs, const DapServerInfo& rhs) {
        QVERIFY(lhs.address == rhs.address);
        QVERIFY(lhs.location == rhs.location);
        QVERIFY(lhs.name == rhs.name);
        QVERIFY(lhs.port == rhs.port);
    }
private slots:
    void parseDapServerInfoArray() {
        QByteArray ba = "[{\"Address\":\"89.163.225.221\",\"Country\":{\"Name\":\"Germany\"},"
                        "\"Description\":\"\",\"Name\":\"ap-de-0\",\"Port\":8001},"
                         "{\"Address\":\"163.173.209.126\",\"Country\":{\"Name\":\"Netherlands\"},"
                        "\"Description\":\"\",\"Name\":\"ap-nl-0\",\"Port\":8002}]";

        DapServerInfo expected1, expected2;
        expected1.name = "ap-de-0";
        expected1.port = 8001;
        expected1.address = "89.163.225.221";

        expected2.name = "ap-nl-0";
        expected2.port = 8002;
        expected2.address = "163.173.209.126";

        QJsonDocument jsonDoc = QJsonDocument::fromJson(ba);
        DapServerInfoList result;
        DapServerInfo::fromJSON(jsonDoc.array(), result);

        QVERIFY(result.contains(expected1));
        QVERIFY(result.contains(expected2));
    }

    void parseDapServerInfoObj() {
        QByteArray ba = "{\"Address\":\"89.163.225.221\",\"Country\":{\"Name\":\"Germany\"},"
                        "\"Description\":\"\",\"Name\":\"ap-de-0\",\"Port\":8001}";
        QJsonDocument jsonDoc = QJsonDocument::fromJson(ba);

        DapServerInfo expected;
        expected.address = "89.163.225.221";
        expected.location = DapServerLocation::GERMANY;
        expected.name = "ap-de-0";
        expected.port = 8001;


        DapServerInfo result;
        DapServerInfo::fromJSON(jsonDoc.object(), result);

        compareExpResultServs(expected, result);
    }

    void testFromToJson() {
        DapServerInfo expected;
        expected.address = "89.163.225.221";
        expected.location = DapServerLocation::GERMANY;
        expected.name = "ap-de-0";
        expected.port = 8001;

        auto s = DapServerInfo::toJSON(expected);
        DapServerInfo result;
        DapServerInfo::fromJSON(s, result);

        QVERIFY(result == expected);

        compareExpResultServs(expected, result);
    }
};

#endif // DAPSERVERINFOTEST_H
