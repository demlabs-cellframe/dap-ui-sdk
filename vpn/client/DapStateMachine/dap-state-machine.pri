greaterThan(QT_MAJOR_VERSION, 5): QT += statemachine

SOURCES += $$PWD/DapIndicatorStateAbstract.cpp \
           $$PWD/DapStateMachine.cpp \
           $$PWD/DapIndicatorSession.cpp \
           $$PWD/DapIndicatorStream.cpp \
           $$PWD/DapIndicatorTunnel.cpp

HEADERS += $$PWD/DapIndicatorStateAbstract.h \
           $$PWD/DapStateMachine.h \
           $$PWD/DapIndicatorSession.h \
           $$PWD/DapIndicatorStream.h \
           $$PWD/DapIndicatorTunnel.h

INCLUDEPATH += $$PWD
