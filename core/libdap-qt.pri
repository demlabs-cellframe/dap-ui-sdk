QT += core network xml

include (../zip/zip.pri)
SOURCES += \
    $$PWD/DapBaseDataLocal.cpp \
    $$PWD/DapBugReport.cpp \
    $$PWD/DapBugReportHistory.cpp \
    $$PWD/DapCdbManager.cpp \
    $$PWD/DapDownload.cpp \
    $$PWD/DapLocationInfo.cpp \
    $$PWD/DapLogHandler.cpp \
    $$PWD/DapNetworkAccessManager.cpp \
    $$PWD/DapNetworkReply.cpp \
    $$PWD/DapNodeOrderInfo.cpp \
    $$PWD/DapNodeWalletData.cpp \
    $$PWD/DapNotification.cpp \
    $$PWD/DapPluginsPathControll.cpp \
    $$PWD/DapSerialKeyData.cpp \
    $$PWD/DapSerialKeyHistory.cpp \
    $$PWD/DapServerList.cpp \
    $$PWD/DapServersData.cpp \
    $$PWD/DapCrypt.cpp \
    $$PWD/DapKeyMsrln.cpp \
    $$PWD/DapLogger.cpp \
    $$PWD/DapKeyCommon.cpp \
    $$PWD/DapConnectClient.cpp \
    $$PWD/DapHttpPing.cpp \
    $$PWD/DapServerConnectivityChecker.cpp \
    $$PWD/DapServerInfo.cpp \
    $$PWD/DapSpeed.cpp \
    $$PWD/DapServersLocalStorage.cpp \
    $$PWD/DapUpdateOperationLogic.cpp \
    $$PWD/DapUtils.cpp

HEADERS += \
    $$PWD/DapBaseDataLocal.h \
    $$PWD/DapBugReport.h \
    $$PWD/DapBugReportHistory.h \
    $$PWD/DapCdbManager.h \
    $$PWD/DapDownload.h \
    $$PWD/DapLocationInfo.h \
    $$PWD/DapLogHandler.h \
    $$PWD/DapNetworkAccessManager.h \
    $$PWD/DapNetworkReply.h \
    $$PWD/DapNodeOrderInfo.h \
    $$PWD/DapNodeWalletData.h \
    $$PWD/DapNotification.h \
    $$PWD/DapPluginsPathControll.h \
    $$PWD/DapSerialKeyData.h \
    $$PWD/DapSerialKeyHistory.h \
    $$PWD/DapServerList.h \
    $$PWD/DapServersData.h \
    $$PWD/DapCrypt.h \
    $$PWD/DapUpdateOperationLogic.h \
    $$PWD/DapUtils.h \
    $$PWD/InterfaceDapKey.h \
    $$PWD/DapKeyMsrln.h \
    $$PWD/DapLogger.h \
    $$PWD/DapKeyCommon.h \
    $$PWD/DapConnectClient.h \
    $$PWD/DapHttpPing.h \
    $$PWD/DapServerConnectivityChecker.h \
    $$PWD/DapServerInfo.h \
    $$PWD/DapSpeed.h \
    $$PWD/DapServersLocalStorage.h \
    $$PWD/DataToUpdate.h


equals(BUILD_VARIANT, "GooglePlay") {
    SOURCES += $$PWD/DapShopManager.cpp
    HEADERS += $$PWD/DapShopManager.h
}

INCLUDEPATH += $$PWD

win32{
    QMAKE_CXXFLAGS +=  -mno-ms-bitfields
}
android {
QT += androidextras
}

INCLUDEPATH += $$PWD/../quazip
