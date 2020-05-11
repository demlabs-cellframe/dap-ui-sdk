SOURCES += \
    $$PWD/DapCmdParser.cpp \
    $$PWD/DapJsonParams.cpp \
    $$PWD/DapIndicator.cpp \
    $$PWD/DapState.cpp \
    $$PWD/DapJsonCmdController.cpp \
    $$PWD/DapCmdAbstract.cpp \
    $$PWD/DapCmdClientAbstract.cpp

HEADERS += \
    $$PWD/DapCmdParser.h \
    $$PWD/DapJsonParams.h \
    $$PWD/DapIndicator.h \
    $$PWD/DapState.h \
    $$PWD/DapJsonCmdController.h \
    $$PWD/DapCmdAbstract.h \
    $$PWD/DapCmdClientAbstract.h \
    $$PWD/DapCmdServiceAbstract.h \
    $$PWD/../../../brand/$$BRAND/DapChainVpnService/DapClientDefinitions.h

INCLUDEPATH += $$PWD  \
    $$PWD/../../../brand/$$BRAND/DapChainVpnService
