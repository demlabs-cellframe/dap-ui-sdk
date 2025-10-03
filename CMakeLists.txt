# Define repository root for relative paths (supports standalone builds)
set(REPO_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)
cmake_minimum_required(VERSION 3.16)

project(dap_ui_sdk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS
  Core
  Network
  StateMachine
  Core5Compat
  Xml
  Widgets
)
find_package(ZLIB REQUIRED)

# Ensure brand headers (DapClientDefinitions.h) are on include path for all sources and AUTOGEN
if(NOT DEFINED BRAND)
  set(BRAND "KelVPN")
endif()
include_directories(${REPO_ROOT}/brand/${BRAND}/DapChainVpnService)

# Collect sources (core and vpn/common). Tests and QML/client/UI/widgets are excluded for now.
file(GLOB_RECURSE DAPUI_CORE_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp
)
file(GLOB_RECURSE DAPUI_CORE_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/core/*.h
)
file(GLOB_RECURSE DAPUI_VPN_COMMON_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/*.cpp
)
file(GLOB_RECURSE DAPUI_VPN_COMMON_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/*.h
)

# Stream layer (needed by Service)
file(GLOB DAPUI_STREAM_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/stream/*.cpp
)
file(GLOB DAPUI_STREAM_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/stream/*.h
)

# Core C++ helpers from cellframe-ui-sdk (Cert, Logger, etc.)
file(GLOB CELLFRAME_UI_CORE_SOURCES
  ${REPO_ROOT}/cellframe-ui-sdk/Core/*.cpp
)

# Crypto sources (Cert/Key implementation)
file(GLOB DAPUI_CRYPTO_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/crypto/src/*.cpp
)

# UI auxiliary helpers (only Utils required for CLI)
set(DAPUI_UI_AUX_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/ui/auxiliary/Utilz.cpp
)

# Chain net service channels (Qt wrappers over Cellframe stream)
set(CELLFRAME_UI_STREAM_CH_SOURCES
  ${REPO_ROOT}/cellframe-ui-sdk/stream/ch/chain/net/srv/DapStreamChChainNetSrv.cpp
  ${REPO_ROOT}/cellframe-ui-sdk/stream/ch/chain/net/srv/vpn/DapStreamChChainNetSrvVpn.cpp
)

# TUN drivers (platform-specific)
set(DAPUI_TUN_SOURCES)
set(DAPUI_TUN_HEADERS)
if(UNIX AND NOT APPLE AND NOT ANDROID)
  list(APPEND DAPUI_TUN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client/DapTunAbstract.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client/DapTunWorkerAbstract.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client/DapTunWorkerUnix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client/DapTunUnixAbstract.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client/linux-src/DapTunLinux.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client/SigUnixHandler.cpp
  )
  list(APPEND DAPUI_TUN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client/DapTunAbstract.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client/DapTunWorkerAbstract.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client/DapTunWorkerUnix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client/DapTunUnixAbstract.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client/linux-src/DapTunLinux.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client/SigUnixHandler.h
  )
endif()

# VPN client state machine
file(GLOB DAPUI_VPN_STATE_MACHINE
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client/DapStateMachine/*.cpp
)

# Service client (platform-specific native parts)
set(DAPUI_VPN_SERVICE_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/service/DapServiceClient.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/service/DapServiceNativeAbstract.cpp
)
if(UNIX AND NOT APPLE AND NOT ANDROID)
  list(APPEND DAPUI_VPN_SERVICE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/service/DapServiceNativeLinux.cpp
  )
elseif(APPLE)
  list(APPEND DAPUI_VPN_SERVICE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/service/DapServiceNativeDarwin.cpp
  )
elseif(ANDROID)
  list(APPEND DAPUI_VPN_SERVICE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/service/DapServiceNativeAndroid.cpp
  )
elseif(WIN32)
  list(APPEND DAPUI_VPN_SERVICE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/service/DapServiceNativeWindows.cpp
  )
endif()

## Network monitor now provided by separate target dap_network_monitor

# Exclude tests
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/core/test/.*")
list(FILTER DAPUI_CORE_HEADERS EXCLUDE REGEX ".*/core/test/.*")
list(FILTER DAPUI_VPN_COMMON_SOURCES EXCLUDE REGEX ".*/vpn/common/.*/.*Test.*\\.(cpp|c)$")
list(FILTER DAPUI_VPN_COMMON_HEADERS EXCLUDE REGEX ".*/vpn/common/.*/.*Test.*\\.(h|hpp)$")
list(FILTER DAPUI_VPN_COMMON_SOURCES EXCLUDE REGEX ".*/vpn/common/DapCommonTest/.*")



add_library(${PROJECT_NAME} STATIC
  ${DAPUI_CORE_SOURCES}
  ${DAPUI_CORE_HEADERS}
  ${DAPUI_VPN_COMMON_SOURCES}
  ${DAPUI_VPN_COMMON_HEADERS}
  ${DAPUI_STREAM_SOURCES}
  ${DAPUI_STREAM_HEADERS}
  ${DAPUI_TUN_SOURCES}
  ${DAPUI_TUN_HEADERS}
  ${DAPUI_VPN_STATE_MACHINE}
  ${DAPUI_VPN_SERVICE_SOURCES}
  ${DAPUI_UI_AUX_SOURCES}
  ${REPO_ROOT}/cellframe-ui-sdk/zip/packzip.cpp
  ${REPO_ROOT}/cellframe-ui-sdk/zip/unpackzip.cpp
  ${REPO_ROOT}/cellframe-ui-sdk/zip/zipbase.cpp
  ${CELLFRAME_UI_CORE_SOURCES}
  ${DAPUI_CRYPTO_SOURCES}
  ${CELLFRAME_UI_STREAM_CH_SOURCES}
)

# Prepare third-party include shims expected by legacy includes
# Use paths relative to this CMake file to support standalone builds
set(REPO_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)
set(THIRDPARTY_INC_DIR ${CMAKE_CURRENT_BINARY_DIR}/thirdparty_includes)
file(MAKE_DIRECTORY ${THIRDPARTY_INC_DIR}/maxminddb)
configure_file(
  ${REPO_ROOT}/cellframe-sdk/3rdparty/libmaxminddb/maxminddb.h
  ${THIRDPARTY_INC_DIR}/maxminddb/maxminddb.h
  COPYONLY
)


target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common
    ${THIRDPARTY_INC_DIR}
    ${REPO_ROOT}/cellframe-ui-sdk/zip
    ${REPO_ROOT}/cellframe-ui-sdk/Core
    ${REPO_ROOT}/dap-ui-sdk/stream
    ${REPO_ROOT}/dap-ui-sdk/vpn/client
    ${REPO_ROOT}/dap-ui-sdk/vpn/client/linux-src
    ${REPO_ROOT}/dap-ui-sdk/vpn/client/DapStateMachine
    ${REPO_ROOT}/dap-ui-sdk/vpn/qml/handlers
    ${REPO_ROOT}/dap-ui-sdk/vpn/service
    ${REPO_ROOT}/dap-ui-sdk/ui/auxiliary
    ${REPO_ROOT}/brand/${BRAND}/DapChainVpnService
    ${REPO_ROOT}/DapNetworkMonitor
    ${REPO_ROOT}/cellframe-sdk/dap-sdk/core/include
    ${REPO_ROOT}/cellframe-sdk/dap-sdk/crypto/include
    ${REPO_ROOT}/cellframe-sdk/dap-sdk/net/client/include
    ${REPO_ROOT}/cellframe-sdk/dap-sdk/net/common/http/include
    ${REPO_ROOT}/cellframe-sdk/dap-sdk/io/include
    ${REPO_ROOT}/cellframe-sdk/dap-sdk/net/stream/
    ${REPO_ROOT}/cellframe-sdk/dap-sdk/net/stream/stream/include
    ${REPO_ROOT}/cellframe-sdk/dap-sdk/net/stream/ch/include
    ${REPO_ROOT}/cellframe-sdk/dap-sdk/net/stream/session/include
    ${REPO_ROOT}/cellframe-sdk/dap-sdk/net/server/http_server/include
    ${REPO_ROOT}/cellframe-sdk/global-db/include
    ${REPO_ROOT}/cellframe-sdk/modules/common/include
    ${REPO_ROOT}/cellframe-sdk/modules/net/include
    ${REPO_ROOT}/cellframe-sdk/modules/chain/include
    ${REPO_ROOT}/cellframe-ui-sdk/stream/ch/chain/net/srv
    ${REPO_ROOT}/cellframe-ui-sdk/stream/ch/chain/net/srv/vpn
    ${CMAKE_CURRENT_SOURCE_DIR}/crypto/include
)

# Exclude Android-specific ShopManager on non-Android platforms
# Keep DapShopManager; code is guarded with Q_OS_ANDROID where needed

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    Qt6::Core
    Qt6::Network
    Qt6::StateMachine
    Qt6::Core5Compat
    Qt6::Xml
    Qt6::Widgets
    dap_network_monitor
    cellframe-sdk
    ZLIB::ZLIB
)

target_compile_definitions(${PROJECT_NAME}
  PUBLIC
    DAP_VERSION="${DAP_VERSION_STR}"
    DAP_BRAND="${BRAND}"
)

# Cross-platform compile options
# - include sys/socket.h only on UNIX (Linux/Android)
# - use -fpermissive only for GCC to relax legacy strictness (Qt5-era code)
if(UNIX)
  target_compile_options(${PROJECT_NAME} PUBLIC -include sys/socket.h)
  target_compile_definitions(${PROJECT_NAME} PUBLIC DAP_OS_UNIX DAP_OS_LINUX _GNU_SOURCE)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(${PROJECT_NAME} PUBLIC -fpermissive)
endif()

# ---------------- Handlers libraries ----------------
file(GLOB DAPUI_CLIENT_CMD_HANDLERS
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client/DapCmdHandlers/*.cpp
)
add_library(dap_ui_handlers_service STATIC ${DAPUI_CLIENT_CMD_HANDLERS})
target_include_directories(dap_ui_handlers_service
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client/DapStateMachine
)
target_link_libraries(dap_ui_handlers_service PUBLIC dap_ui_sdk Qt6::Core Qt6::Network Qt6::StateMachine Qt6::Core5Compat)

file(GLOB DAPUI_QML_HANDLERS
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/qml/handlers/*.cpp
)
add_library(dap_ui_handlers_qml STATIC ${DAPUI_QML_HANDLERS})
target_include_directories(dap_ui_handlers_qml
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/vpn/qml/handlers
)
target_link_libraries(dap_ui_handlers_qml PUBLIC dap_ui_sdk Qt6::Core Qt6::Network Qt6::StateMachine Qt6::Core5Compat)
