#include <QTest>
#include <QDebug>

#define RUN_TESTS(TestObject) { \
    TestObject tc; \
    if(QTest::qExec(&tc)) \
    exit(1); }

/* comment this and add RUN_TESTS in main function
 * for run and debugging one testing class */
#define RUN_ALL_TESTS

#ifdef RUN_ALL_TESTS
#include "TestHeaders.h"
void run_all_tests() {
    RUN_TESTS(DapJsonCmdTest);
}
#endif

void sendCommand(std::initializer_list<QPair<QString, const QJsonValue&>> cmd = {})
{
    QJsonObject nes;
    for(auto c : cmd) {
        nes[c.first] = c.second;
    }
    qDebug() << nes;
}

#define DapJsonParams(key,value) \
    QPair<QString, const QJsonValue&>(key, value)

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    QTEST_SET_MAIN_SOURCE_PATH
#ifdef RUN_ALL_TESTS
            run_all_tests();
#endif
    QJsonObject a;
    a["key"] = 1;

    sendCommand({DapJsonParams("s", 2), DapJsonParams("obj", a)});
    sendCommand();
}
