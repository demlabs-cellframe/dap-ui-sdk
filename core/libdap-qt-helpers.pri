QT += core network

include (../zip/zip.pri)
SOURCES += \
    $$PWD/DapLogHandler.cpp \
    $$PWD/DapPluginsPathControll.cpp \
    $$PWD/DapLogger.cpp \
    $$PWD/DapUtils.cpp

HEADERS += \
    $$PWD/DapLocationInfo.h \
    $$PWD/DapLogHandler.h \
    $$PWD/DapPluginsPathControll.h \
    $$PWD/DapUtils.h \
    $$PWD/DapLogger.h \


INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/../quazip