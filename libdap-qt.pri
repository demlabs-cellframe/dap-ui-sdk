include ( libdap/libdap.pri )
include ( libdap/libdap-crypto.pri )
INCLUDEPATH += libdap/libdap/include
INCLUDEPATH += libdap/libdap-crypto/include

SOURCES += \
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

INCLUDEPATH += $$PWD $$PWD/../



