include (libdap/libdap.pri)
INCLUDEPATH += libdap/ libdap/common

SOURCES += $$PWD/DapConnectBase.cpp \
    $$PWD/DapSession.cpp \
    $$PWD/BaseObject.cpp \
    $$PWD/DapCrypt.cpp \
    $$PWD/DapKeyAes.cpp \
    $$PWD/DapKeyMsrln.cpp \
    $$PWD/DapLogger.cpp


HEADERS += \
    $$PWD/DapConnectBase.h \
    $$PWD/DapSession.h \
    $$PWD/BaseObject.h \
    $$PWD/DapCrypt.h \
    $$PWD/InterfaceDapKey.h \
    $$PWD/DapKeyAes.h \
    $$PWD/DapKeyMsrln.h \
    $$PWD/DapLogger.h


INCLUDEPATH += $$PWD


