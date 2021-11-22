SOURCES += \
    $$PWD/DapCmdLastConnectionData.cpp \
    $$PWD/DapCmdNews.cpp \
    $$PWD/DapCmdPingServer.cpp \
    $$PWD/DapCmdSendBugReport.cpp \
    $$PWD/DapCmdSignUp.cpp \
    $$PWD/DapCmdResetSerialKey.cpp \
    $$PWD/DapCmdTunTap.cpp \
    $$PWD/DapCmdUserData.cpp \
    $$PWD/DapCmdAuth.cpp \
    $$PWD/DapCmdConnect.cpp \
    $$PWD/DapCmdStates.cpp \
    $$PWD/DapCmdStats.cpp \
    $$PWD/DapCmdServersList.cpp \
    $$PWD/DapCmdServerData.cpp \
    $$PWD/DapCmdAndroidTunnel.cpp
    
equals(BUILD_VARIANT, "GooglePlay") {
	SOURCES += $$PWD/DapCmdPurchase.cpp
}

HEADERS += \
    $$PWD/DapCmdLastConnectionData.h \
    $$PWD/DapCmdNews.h \
    $$PWD/DapCmdPingServer.h \
    $$PWD/DapCmdSendBugReport.h \
    $$PWD/DapCmdSignUp.h \
    $$PWD/DapCmdResetSerialKey.h \
    $$PWD/DapCmdTunTap.h \
    $$PWD/DapCmdUserData.h \
    $$PWD/DapCmdConnect.h \
    $$PWD/DapCmdAuth.h \
    $$PWD/DapCmdStates.h \
    $$PWD/DapCmdStats.h \
    $$PWD/DapCmdServersList.h \
    $$PWD/DapCmdServerData.h \
    $$PWD/DapCmdAndroidTunnel.h

equals(BUILD_VARIANT, "GooglePlay") {
	HEADERS += $$PWD/DapCmdPurchase.h
}

INCLUDEPATH += $$PWD

