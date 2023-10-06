QT += network
CONFIG += c++11 create_prl static
TARGET = libdap-qt-stream
TEMPLATE = lib

ios {
    QT += core
}

include (../../core/libdap-qt.pri)
include(libdap-qt-stream.pri)
