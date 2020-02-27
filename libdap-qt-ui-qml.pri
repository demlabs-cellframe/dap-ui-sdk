QT += core qml

INCLUDEPATH += $$PWD $$PWD/../

win32{
    QMAKE_CXXFLAGS +=  -mno-ms-bitfields
}

DISTFILES +=

RESOURCES += \
    $$PWD/libdap-qt-ui-qml.qrc




