QT += testlib network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp

HEADERS += \
    TestHeaders.h \
    DapJsonCmd_test.hpp

include (../DapVpnCommon.pri)
