#ifndef DAPJSONCMD_TEST_HPP
#define DAPJSONCMD_TEST_HPP

#include <QTest>
#include "../DapJsonCmd.h"
#include <QJsonArray>

class DapJsonCmdTest : public QObject {
    Q_OBJECT
private slots:
    void commandToString() {
        QCOMPARE(DapJsonCmd::commandToString(DapJsonCommand::CONNECTION),
                 "connection");
    }

    void stringToCommand() {
        QCOMPARE(DapJsonCmd::stringToCommand("stats"), DapJsonCommand::STATS);
    }

    void validJson() {
        QJsonObject mainObject
        {
            {"command", "state"}
        };

        QJsonObject params
        {
            {"authorized", true}
        };
        mainObject.insert("params", params);
        QVERIFY(DapJsonCmd::isJsonValid(mainObject));
    }

    void validQStringJson() {
        QString json = R"({"command": "state", "params": {"authorized": true}})";
        QVERIFY(DapJsonCmd::isJsonValid(json));
    }

    void notValidJson() {
        QString badJson = R"({"command": "state" "params": {"authorized": true}})";
        QVERIFY(DapJsonCmd::isJsonValid(badJson) == false);
    }

    void generateCmd() {
        QByteArray bResult = DapJsonCmd::generateCmd(DapJsonCommand::CONNECTION,
        {
            DapJsonParam(DapJsonParams::ADDRESS, "addr")
        });

        auto result = QJsonDocument::fromJson(bResult);
        QVERIFY(result["command"].isString());
        QCOMPARE(result["command"], DapJsonCmd::commandToString(DapJsonCommand::CONNECTION));

        QJsonObject resultParams = result["params"].toObject();

        QVERIFY(resultParams["address"].isString());
        QCOMPARE(resultParams["address"].toString(), "addr");
    }

    void generateCmdWithoutParams() {
        QByteArray bResult = DapJsonCmd::generateCmd(DapJsonCommand::INDICATOR_STATE);
        auto result = QJsonDocument::fromJson(bResult);
        QVERIFY(result["command"].isString());
        QCOMPARE(result["command"], "indicator_state");
    }

    void load() {
        QString sJson = R"({"command": "state", "params": {"authorized":true,"two_int":2}})";
        DapJsonCmdPtr cmd = DapJsonCmd::load(sJson);

        QCOMPARE(cmd->getCommand(), DapJsonCommand::INDICATOR_STATE);
        QJsonValue auth = cmd->getParam("authorized");
        QVERIFY(auth.isBool());
        QCOMPARE(auth.toBool(), true);
        QJsonValue two_int = cmd->getParam("two_int");
        QCOMPARE(two_int.toInt(), 2);
    }

    void getParams() {
        QString sJson = R"({"command": "state", "params": {"authorized":true,"two_int":2}})";
        DapJsonCmdPtr cmd = DapJsonCmd::load(sJson);
        const QJsonObject * params = cmd->getParams();

        QVERIFY(params->value("authorized").isBool());
        QCOMPARE(params->value("authorized").toBool(), true);

        QCOMPARE(params->value("two_int").toInt(), 2);
        qDebug() << "Params " << params->value("authorized").toBool();
    }

    void genResponse() {
        QJsonObject obj;
        obj["value_str"] = "str";
        obj["var"] = 2;

        auto resp = DapJsonCmd::generateResponse(DapJsonCommand::INDICATOR_STATE, obj);
        auto respJson = QJsonDocument::fromJson(resp);
        QCOMPARE(respJson["command"].toString(), "indicator_state");
        QCOMPARE(respJson["result"].toObject()["var"].toInt(), 2);
        QCOMPARE(respJson["result"].toObject()["value_str"].toString(), "str");
    }

    void genResponseArray() {
        QJsonObject obj;
        obj["var1"] = 1;
        obj["var2"] = 2;
        QJsonObject obj2;
        obj2["var12"] = 12;
        obj2["var22"] = 22;

        QJsonArray arr;
        arr.append(obj);
        arr.append(obj2);

        QJsonObject expectedResult;
        QJsonArray expectedArr;
        expectedArr.append(obj);
        expectedArr.append(obj2);
        expectedResult["command"] = "indicator_state";
        expectedResult["result"] = expectedArr;

        auto result = QJsonDocument::fromJson(DapJsonCmd::generateResponse(DapJsonCommand::INDICATOR_STATE, {
                                                                               obj, obj2}));
        QCOMPARE(result, QJsonDocument(expectedResult));
    }
};

#endif // DAPJSONCMD_TEST_HPP
