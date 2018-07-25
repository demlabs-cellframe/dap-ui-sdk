include (libdap/libdap.pri)
INCLUDEPATH += libdap/

SOURCES += $$PWD/DapConnectBase.cpp \
    $$PWD/DapSession.cpp \
    $$PWD/BaseObject.cpp \
    $$PWD/DapCrypt.cpp \
    $$PWD/DapKeyAes.cpp \
    $$PWD/BaseForm.cpp \
    $$PWD/DapConnSession.cpp \
    $$PWD/DapKeyMsrln.cpp


HEADERS += \
    $$PWD/DapConnectBase.h \
    $$PWD/DapSession.h \
    $$PWD/BaseObject.h \
    $$PWD/DapCrypt.h \
    $$PWD/InterfaceDapKey.h \
    $$PWD/DapKeyAes.h \
    $$PWD/BaseForm.h \
    $$PWD/DapConnSession.h \
    $$PWD/DapKeyMsrln.h


INCLUDEPATH += $$PWD


