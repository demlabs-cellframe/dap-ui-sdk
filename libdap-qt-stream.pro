QT += network
CONFIG += c++11 create_prl static
TARGET = libdap-qt-stream
TEMPLATE = lib

include(libdap-qt-stream.pri)
include (../libdap-qt/libdap-qt.pri)
INCLUDEPATH += $$_PRO_FILE_PWD_/../libdap-qt/
