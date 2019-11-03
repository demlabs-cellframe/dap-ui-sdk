
SOURCES += \
    $$PWD/DapChSockForw.cpp \
    $$PWD/DapTunAbstract.cpp\
    $$PWD/DapTunWorkerAbstract.cpp \
    $$PWD/DapTunNative.cpp

HEADERS += \
    $$PWD/DapChSockForw.h \
    $$PWD/DapTunAbstract.h \
    $$PWD/DapTunWorkerAbstract.h \
    $$PWD/DapTunNative.h \
    $$PWD/DapSockForwPacket.h

INCLUDEPATH += $$PWD


unix: !win32 {
    HEADERS += $$PWD/DapTunUnixAbstract.h $$PWD/DapTunWorkerUnix.h $$PWD/SigUnixHandler.h
    SOURCES += $$PWD/DapTunUnixAbstract.cpp $$PWD/DapTunWorkerUnix.cpp $$PWD/SigUnixHandler.cpp
}

linux-* {
    include ($$PWD/linux-src/linux-src.pri)
}

win32 {
    HEADERS += $$PWD/DapTunWindows.h $$PWD/DapTunWorkerWindows.h
    SOURCES += $$PWD/DapTunWindows.cpp $$PWD/DapTunWorkerWindows.cpp
}

android{
    HEADERS += $$PWD/DapTunAndroid.h
    SOURCES += $$PWD/DapTunAndroid.cpp
    HEADERS += $$PWD/DapTunWorkerAndroid.h $$PWD/DapTunAndroid.h
    SOURCES += $$PWD/DapTunWorkerAndroid.cpp $$PWD/DapTunAndroid.cpp
}

darwin {
    HEADERS  += $$PWD/DapTunDarwin.h
    SOURCES += $$PWD/DapTunDarwin.cpp
    #INCLUDEPATH += /usr/local/opt/openssl/include
    #LIBS += -L/usr/local/opt/openssl/lib
    #LIBS += -lcrypto
}

macos {
    CONFIG  -= app_bundle
    DEFINES += DAP_SERVICE_CONNECT_TCP VPN_TUNTAP
    HEADERS += $$PWD/DapTunMac.h $$PWD/DapTunWorkerMac.h
    SOURCES += $$PWD/DapTunMac.cpp $$PWD/DapTunWorkerMac.cpp
}

win32{
    QMAKE_CXXFLAGS +=  -mno-ms-bitfields
    INCLUDEPATH += $$_PRO_FILE_PWD_/../windows/windows
    INCLUDEPATH += $$_PRO_FILE_PWD_/../windows/tuntap
    HEADERS+= $$PWD/../windows/windows/ip.h
    HEADERS+= $$PWD/../windows/tuntap/tuntap.h
    SOURCES+= $$PWD/../windows/tuntap/tuntap.cpp

    LIBS += -lWS2_32
    LIBS += -lAdvapi32
    LIBS += -lIphlpapi
    LIBS += -lUser32
    LIBS += -lole32
    LIBS += -luuid

    DEFINES += NTDDI_VERSION=0x06000000
    DEFINES += _WIN32_WINNT=0x0600
}

android{
    TEMPLATE = lib
    CONFIG += shared
    CONFIG -= console
    DEFINES += DAP_SERVICE_BUILD_LIB DAP_SERVICE_CONNECT_TCP
    QT += androidextras
    DEFINES += DAP_PLATFORM_MOBILE
}

ios{
    DEFINES += DAP_PLATFORM_MOBILE
}
