QT += network
CONFIG += c++11 create_prl static
TARGET = libdap-qt-stream
TEMPLATE = lib

include (./libdap-qt/libdap-qt.pri)
include(libdap-qt-stream.pri)
