SOURCES += \
    #$$PWD/DapChSockForw.cpp \
    $$PWD/DapTunAbstract.cpp\
    $$PWD/DapTunWorkerAbstract.cpp \
    $$PWD/DapTunNative.cpp \
    $$PWD/DapDNSController.cpp

HEADERS += \
    #$$PWD/DapChSockForw.h \
    $$PWD/DapTunAbstract.h \
    $$PWD/DapTunWorkerAbstract.h \
    $$PWD/DapTunNative.h \
    $$PWD/DapSockForwPacket.h \
    $$PWD/DapStreamChChainVpnPacket.h \
    $$PWD/DapDNSController.h

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
    HEADERS += $$PWD/DapTunWorkerAndroid.h $$PWD/DapTunAndroid.h
    SOURCES += $$PWD/DapTunWorkerAndroid.cpp $$PWD/DapTunAndroid.cpp
}

darwin {
#    HEADERS  += $$PWD/DapTunDarwin.h
#    $$PWD/DapTunWorkerDarwin.h
#    SOURCES += $$PWD/DapTunWorkerDarwin.cpp \
#    $$PWD/DapTunDarwin.mm
    include ($$PWD/darwin/darwin.pri)
    #INCLUDEPATH += /usr/local/opt/openssl/include
    #LIBS += -L/usr/local/opt/openssl/lib
    LIBS += -framework NetworkExtension
}

win32{
    QMAKE_CXXFLAGS +=  -mno-ms-bitfields
    INCLUDEPATH += $$PWD/../../../os/windows/windows
    INCLUDEPATH += $$PWD/../../../os/windows/tuntap
    INCLUDEPATH += $$PWD/../../../os/windows/service
    HEADERS+= $$PWD/../../../os/windows/windows/ip.h
    HEADERS+= $$PWD/../../../os/windows/tuntap/tuntap.h
    HEADERS+= $$PWD/../../../os/windows/service/Service.h
    SOURCES+= $$PWD/../../../os/windows/tuntap/tuntap.cpp
    SOURCES+= $$PWD/../../../os/windows/service/Service.cpp

    LIBS += -ladvapi32 -luuid -liphlpapi -lnetshell -luser32 -lws2_32 -lole32 -ldnsapi -ldhcpcsvc -lsetupapi -lcfgmgr32 -lnewdev

    DEFINES += NTDDI_VERSION=0x06000000
    DEFINES += _WIN32_WINNT=0x0600
}
