#ifndef TESTDAPSERVERSLOCALSTORAGE_H
#define TESTDAPSERVERSLOCALSTORAGE_H

#include <QTest>
#include "DapServersLocalStorage.h"

class DapServersLocalStorageTest : public QObject {
    Q_OBJECT
private:
private slots:
    void addRemoveServer()
    {
        QString testFileName = "testServers1";
        DapServersLocalStorage t(this, testFileName);

        DapServerInfo s;
        s.address = "127.0.0.1";
        s.port = 8080;
        s.name = "localhost";
        s.location = DapServerLocation::UNKNOWN;

        t.addServer(s);

        QCOMPARE(t.getServersList().size(), 1);

        QCOMPARE(t.getServersList()[0].address, s.address);
        QCOMPARE(t.getServersList()[0].port, s.port);
        QCOMPARE(t.getServersList()[0].name, s.name);
        QCOMPARE(t.getServersList()[0].location, s.location);

        t.removeServer(s);
        QCOMPARE(t.getServersList().size(), 0);
    }

    void loadFileTest()
    {
        QString testFileName = "testServers2";
        DapServersLocalStorage t(this, testFileName);

        DapServerInfo s;
        s.address = "127.0.0.1";
        s.port = 8080;
        s.name = "localhost";
        s.location = DapServerLocation::UNKNOWN;

        t.addServer(s);

        DapServersLocalStorage t2(this, testFileName);

        QCOMPARE(t2.getServersList().size(), 1);

        QCOMPARE(t2.getServersList()[0].address, s.address);
        QCOMPARE(t2.getServersList()[0].port, s.port);
        QCOMPARE(t2.getServersList()[0].name, s.name);
        QCOMPARE(t2.getServersList()[0].location, s.location);

        // this server already exists
        QCOMPARE(t2.addServer(s), 21);

        t2.removeServer(s);
        QCOMPARE(t2.getServersList().size(), 0);
    }

    void editServer() {
        QString testFileName = "testServers3";
        DapServersLocalStorage t(this, testFileName);

        DapServerInfo s;
        s.address = "127.0.0.1";
        s.port = 8080;
        s.name = "localhost";
        s.location = DapServerLocation::UNKNOWN;

        t.addServer(s);

        DapServerInfo s2(s);
        s2.port = 8081;
        s2.name = "newName";
        t.editServer(s, s2);

        QCOMPARE(t.getServersList().size(), 1);

        QCOMPARE(t.getServersList()[0].address, s2.address);
        QCOMPARE(t.getServersList()[0].port, s2.port);
        QCOMPARE(t.getServersList()[0].name, s2.name);
        QCOMPARE(t.getServersList()[0].location, s2.location);

        t.removeServer(s2);
        QCOMPARE(t.getServersList().size(), 0);
    }
};

#endif // TESTDAPSERVERSLOCALSTORAGE_H
