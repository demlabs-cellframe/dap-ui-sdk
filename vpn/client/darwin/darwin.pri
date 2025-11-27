# NetworkExtension files with ARC enabled
SOURCES += $$PWD/DapPacketTunnelProvider.mm
HEADERS += $$PWD/DapPacketTunnelProvider.h
SOURCES += $$PWD/CoreBridge.mm
HEADERS += $$PWD/CoreBridge.h
HEADERS += $$PWD/CoreBridgeSessionCompat.h \
           $$PWD/CoreBridgeSessionCAPI.h

# Legacy UTUN files (deprecated)
SOURCES += $$PWD/DapUtun.cpp
HEADERS += $$PWD/DapUtun.h

# Enable ARC for NetworkExtension files
QMAKE_CXXFLAGS += -fobjc-arc
QMAKE_OBJECTIVE_CFLAGS += -fobjc-arc

# Enable C++17 for Cellframe Core compatibility
CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17

# Cellframe SDK paths - using headers only, no source compilation
CELLFRAME_SDK = $$PWD/../../../cellframe-sdk/dap-sdk

INCLUDEPATH += \
    $$CELLFRAME_SDK/include \
    $$CELLFRAME_SDK/include/dap \
    $$CELLFRAME_SDK/include/dap/core \
    $$CELLFRAME_SDK/include/dap/crypto \
    $$CELLFRAME_SDK/include/dap/net \
    $$CELLFRAME_SDK/include/dap/net/stream \
    $$CELLFRAME_SDK/include/dap/net/stream/session

# No source compilation - use stubs instead
# SOURCES += $$CELLFRAME_SDK/net/stream/session/dap_stream_session.c

# Add stubs for Cellframe functions
SOURCES += $$PWD/CellframeStubs.c

# Define available APIs based on source analysis
DEFINES += HAVE_DAP_STREAM_SESSION_NEW
DEFINES += HAVE_DAP_STREAM_SESSION_OPEN
DEFINES += HAVE_DAP_STREAM_SESSION_CLOSE_MT

# Use stubs instead of real implementation
DEFINES += USE_CELLFRAME_STUBS
