QT += core network xml
QT += widgets #for DapServerSelector - QMessageBox
#INCLUDEPATH += ../libdap/include ../libdap/src
#INCLUDEPATH += ../libdap-crypto/include  ../libdap-crypto/src
include (../quazip/quazip.pri)

SOURCES += \
    $$PWD/DapBugReport.cpp \
    $$PWD/DapBugReportData.cpp \
    $$PWD/DapDataLocal.cpp \
    $$PWD/DapLogHandler.cpp \
    $$PWD/DapServerSelector.cpp \
    $$PWD/DapServersData.cpp \
    $$PWD/DapSession.cpp \
    $$PWD/DapCrypt.cpp \
    $$PWD/DapKeyMsrln.cpp \
    $$PWD/DapLogger.cpp \
    $$PWD/DapKeyAes.cpp \
    $$PWD/DapConnectClient.cpp \
    $$PWD/DapHttpPing.cpp \
    $$PWD/DapServerInfo.cpp \
    $$PWD/DapServersListRequester.cpp \
    $$PWD/DapServersLocalStorage.cpp

HEADERS += \
    $$PWD/DapBugReport.h \
    $$PWD/DapBugReportData.h \
    $$PWD/DapDataLocal.h \
    $$PWD/DapLogHandler.h \
    $$PWD/DapServerSelector.h \
    $$PWD/DapServersData.h \
    $$PWD/DapSession.h \
    $$PWD/DapCrypt.h \
    $$PWD/InterfaceDapKey.h \
    $$PWD/DapKeyMsrln.h \
    $$PWD/DapLogger.h \
    $$PWD/DapKeyAes.h \
    $$PWD/DapConnectClient.h \
    $$PWD/DapReplyTimeout.h \
    $$PWD/DapHttpPing.h \
    $$PWD/DapServerInfo.h \
    $$PWD/DapServersListRequester.h \
    $$PWD/DapServersLocalStorage.h

INCLUDEPATH += $$PWD

win32{
    QMAKE_CXXFLAGS +=  -mno-ms-bitfields
}

INCLUDEPATH += $$PWD/../quazip


