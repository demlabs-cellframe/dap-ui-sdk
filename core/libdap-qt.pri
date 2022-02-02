QT += core network xml
#INCLUDEPATH += ../libdap/include ../libdap/src
include (../quazip/quazip.pri)
SOURCES += \
    $$PWD/DapBugReport.cpp \
    $$PWD/DapBugReportData.cpp \
    $$PWD/DapDataLocal.cpp \
    $$PWD/DapLocationInfo.cpp \
    $$PWD/DapLogHandler.cpp \
    $$PWD/DapNetworkAccessManager.cpp \
    $$PWD/DapNetworkReply.cpp \
    $$PWD/DapPluginsPathControll.cpp \
    $$PWD/DapSerialKeyData.cpp \
    $$PWD/DapServersData.cpp \
    $$PWD/DapSession.cpp \
    $$PWD/DapCrypt.cpp \
    $$PWD/DapKeyMsrln.cpp \
    $$PWD/DapLogger.cpp \
    $$PWD/DapKeyCommon.cpp \
    $$PWD/DapConnectClient.cpp \
    $$PWD/DapHttpPing.cpp \
    $$PWD/DapServerInfo.cpp \
    $$PWD/DapServersLocalStorage.cpp \
    $$PWD/DapSignUpData.cpp

#equals(BUILD_VARIANT, "GooglePlay") {
#	SOURCES += $$PWD/DapShopManager.cpp
#}

HEADERS += \
    $$PWD/DapBugReport.h \
    $$PWD/DapBugReportData.h \
    $$PWD/DapDataLocal.h \
    $$PWD/DapLocationInfo.h \
    $$PWD/DapLogHandler.h \
    $$PWD/DapNetworkAccessManager.h \
    $$PWD/DapNetworkReply.h \
    $$PWD/DapPluginsPathControll.h \
    $$PWD/DapSerialKeyData.h \
    $$PWD/DapServersData.h \
    $$PWD/DapSession.h \
    $$PWD/DapCrypt.h \
    $$PWD/DapSignUpData.h \
    $$PWD/DapUtils.h \
    $$PWD/InterfaceDapKey.h \
    $$PWD/DapKeyMsrln.h \
    $$PWD/DapLogger.h \
    $$PWD/DapKeyCommon.h \
    $$PWD/DapConnectClient.h \
    $$PWD/DapHttpPing.h \
    $$PWD/DapServerInfo.h \
    $$PWD/DapServersLocalStorage.h \
    $$PWD/DataToUpdate.h

#equals(BUILD_VARIANT, "GooglePlay") {
#	HEADERS += $$PWD/DapShopManager.h
#}

INCLUDEPATH += $$PWD

win32{
    QMAKE_CXXFLAGS +=  -mno-ms-bitfields
}
android {
QT += androidextras
}

INCLUDEPATH += $$PWD/../quazip
