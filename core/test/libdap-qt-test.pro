QT += testlib network

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
QMAKE_CXXFLAGS += "-std=c++17"
TEMPLATE = app

include (../libdap-qt.pri)

# test headers and source
SOURCES +=  main.cpp

HEADERS += \
    TestHeaders.h \
    DapCryptTest.h \
    DapKeyIaesTest.h \
    DapKeyMsrlnTest.h \
    DapServerInfoTest.h \
    DapServersLocalStorageTest.h
