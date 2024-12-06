SOURCES += \
    $$PWD/DapCmdBugReportsStatus.cpp \
    $$PWD/DapCmdCdbCtl.cpp \
    $$PWD/DapCmdDataLocal.cpp \
    $$PWD/DapCmdGeoIP.cpp \
    $$PWD/DapCmdLastConnectionData.cpp \
    $$PWD/DapCmdNews.cpp \
    $$PWD/DapCmdPingServer.cpp \
    $$PWD/DapCmdResetSerialKey.cpp \
    $$PWD/DapCmdSignUp.cpp \
    $$PWD/DapCmdSendBugReport.cpp \
    $$PWD/DapCmdStats.cpp \
    $$PWD/DapCmdTunTap.cpp \
    $$PWD/DapCmdServerData.cpp \
    $$PWD/DapCmdUpdateOperation.cpp \
    $$PWD/DapCmdUserData.cpp \
    $$PWD/DapCmdStates.cpp \
    $$PWD/DapCmdConnect.cpp \
    $$PWD/DapCmdAuth.cpp \
    $$PWD/DapCmdServersList.cpp \
    $$PWD/DapCmdAndroidTunnel.cpp \
    $$PWD/DapCmdNode.cpp \
    $$PWD/DapCmdSystemTray.cpp

equals(BUILD_VARIANT, "GooglePlay") {
	SOURCES += $$PWD/DapCmdPurchase.cpp
}

HEADERS += $$PWD/DapCmdLastConnectionData.h \
    $$PWD/DapCmdBugReportsStatus.h \
    $$PWD/DapCmdCdbCtl.h \
    $$PWD/DapCmdDataLocal.h \
    $$PWD/DapCmdGeoIP.h \
    $$PWD/DapCmdNews.h \
    $$PWD/DapCmdPingServer.h \
    $$PWD/DapCmdResetSerialKey.h \
    $$PWD/DapCmdSignUp.h \
    $$PWD/DapCmdSendBugReport.h \
    $$PWD/DapCmdStats.h \
    $$PWD/DapCmdTunTap.h \
    $$PWD/DapCmdServerData.h \
    $$PWD/DapCmdUpdateOperation.h \
    $$PWD/DapCmdUserData.h \
    $$PWD/DapCmdStates.h \
    $$PWD/DapCmdConnect.h \
    $$PWD/DapCmdAuth.h \
    $$PWD/DapCmdServersList.h \
    $$PWD/DapCmdAndroidTunnel.h \
    $$PWD/DapCmdSystemTray.h \
    $$PWD/DapCmdNode.h \
    $$PWD/vpnDefine.h

equals(BUILD_VARIANT, "GooglePlay") {
	HEADERS += $$PWD/DapCmdPurchase.h
}

INCLUDEPATH += $$PWD


