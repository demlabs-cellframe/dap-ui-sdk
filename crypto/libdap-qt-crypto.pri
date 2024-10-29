QT += core

SOURCES += \
    $$PWD/src/DapHash.cpp \
    $$PWD/src/DapKey.cpp \
    $$PWD/src/DapKeySign.cpp \
    $$PWD/src/DapPKey.cpp \
    $$PWD/src/DapCert.cpp \
    $$PWD/src/DapCrypto.cpp

HEADERS += \
    $$PWD/include/DapHash.h \
    $$PWD/include/DapKey.h \
    $$PWD/include/DapKeySign.h \
    $$PWD/include/DapPKey.h \
    $$PWD/include/DapCert.h \
    $$PWD/include/DapCrypto.h

INCLUDEPATH += $$PWD/include

win32{
    QMAKE_CXXFLAGS +=  -mno-ms-bitfields
}



