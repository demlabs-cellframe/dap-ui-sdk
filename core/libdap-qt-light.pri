QT += core network xml

include (../zip/zip.pri)
SOURCES += \
    $$PWD/DapLogHandler.cpp \
    $$PWD/DapNetworkReply.cpp \
    $$PWD/DapPluginsPathControll.cpp \
    $$PWD/DapLogger.cpp


HEADERS += \
    $$PWD/DapLogHandler.h \
    $$PWD/DapNetworkReply.h \
    $$PWD/DapPluginsPathControll.h \
    $$PWD/DapLogger.h

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/../quazip
