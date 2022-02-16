SOURCES += \
    $$PWD/DapCmdBugReportsStatus.cpp \
    $$PWD/DapCmdLastConnectionData.cpp \
    $$PWD/DapCmdNews.cpp \
    $$PWD/DapCmdPingServer.cpp \
    $$PWD/DapCmdResetSeialKey.cpp \
    $$PWD/DapCmdSignUp.cpp \
    $$PWD/DapCmdSendBugReport.cpp \
    $$PWD/DapCmdStats.cpp \
    $$PWD/DapCmdTunTap.cpp \
    $$PWD/DapCmdServerData.cpp \
    $$PWD/DapCmdUserData.cpp \
    $$PWD/DapCmdStates.cpp \
    $$PWD/DapCmdConnect.cpp \
    $$PWD/DapCmdAuth.cpp \
    $$PWD/DapCmdServersList.cpp \
    $$PWD/DapCmdAndroidTunnel.cpp

equals(BUILD_VARIANT, "GooglePlay") {
	SOURCES += $$PWD/DapCmdPurchase.cpp
}

HEADERS += $$PWD/DapCmdLastConnectionData.h \
    $$PWD/DapCmdBugReportsStatus.h \
    $$PWD/DapCmdNews.h \
    $$PWD/DapCmdPingServer.h \
    $$PWD/DapCmdResetSeialKey.h \
    $$PWD/DapCmdSignUp.h \
    $$PWD/DapCmdSendBugReport.h \
    $$PWD/DapCmdStats.h \
    $$PWD/DapCmdTunTap.h \
    $$PWD/DapCmdServerData.h \
    $$PWD/DapCmdUserData.h \
    $$PWD/DapCmdStates.h \
    $$PWD/DapCmdConnect.h \
    $$PWD/DapCmdAuth.h \
    $$PWD/DapCmdServersList.h \
    $$PWD/DapCmdAndroidTunnel.h

equals(BUILD_VARIANT, "GooglePlay") {
	HEADERS += $$PWD/DapCmdPurchase.h
}

INCLUDEPATH += $$PWD


