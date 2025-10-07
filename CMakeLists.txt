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
  Widgets
)
# Qt Xml is optional: link if available
find_package(Qt6 COMPONENTS Xml QUIET)
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

# EXCLUDE migrated classes
# DapNode* and DapCdbManager (migrated to DapChainVpnService/web3)
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapNode\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapNodeWeb3\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapNodeOrderInfo\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapNodeTransactionHistory\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapNodeWalletData\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapCdbManager\\.cpp$")
# DapGeoIP (migrated to Common/Service/GeoIP)
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapGeoIP\\.cpp$")
# DapBugReportHistory (migrated to Common/Service/BugReport)
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapBugReportHistory\\.cpp$")
# DapSession (migrated to DapChainVpnService/vpn)
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapSession\\.cpp$")
# DapSpeed (migrated to Common/Commands/Common)
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapSpeed\\.cpp$")
# DapSerialKey* (migrated to Common/Service/SerialKey)
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapSerialKeyData\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapSerialKeyHistory\\.cpp$")
# DapBugReport (migrated to Common/Service/BugReport)
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapBugReport\\.cpp$")
# DataLocal classes (migrated to Common/Service/DataLocal)
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapDataLocal\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapBaseDataLocal\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapServiceDataLocal\\.cpp$")
# Network classes (migrated to Common/Network)
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapConnectClient\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapNetworkAccessManager\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapNetworkReply\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapHttpPing\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapDownload\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapServerConnectivityChecker\\.cpp$")
# Crypto classes (migrated to Common/Crypto)
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapCrypt\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapKeyCommon\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapKeyMsrln\\.cpp$")
# Utils (migrated to Common/Utils)
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapUtils\\.cpp$")
# Shop (migrated to Common/Shop)
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapShopManager\\.cpp$")
# Servers (migrated to Common/Service/Servers)
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapServerInfo\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapServerList\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapServersData\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapServersLocalStorage\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapLocationInfo\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DataToUpdate\\.cpp$")
# Notifications (migrated to Common/Service/Notifications)
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapNotification\\.cpp$")
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapNotificationHistory\\.cpp$")
# Updates (migrated to Common/Service/Updates)
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapUpdateOperationLogic\\.cpp$")
# Config (migrated to Common/Service/Config)
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/UserConfigManager\\.cpp$")
# BugReportData (migrated to Common/Service/BugReport)
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/DapBugReportData\\.cpp$")

file(GLOB_RECURSE DAPUI_CORE_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/core/*.h
)

# EXCLUDE migrated headers
# DapNode* and DapCdbManager headers (migrated to DapChainVpnService/web3)
list(FILTER DAPUI_CORE_HEADERS EXCLUDE REGEX ".*/DapNode\\.h$")
list(FILTER DAPUI_CORE_HEADERS EXCLUDE REGEX ".*/DapNodeWeb3\\.h$")
list(FILTER DAPUI_CORE_HEADERS EXCLUDE REGEX ".*/DapNodeOrderInfo\\.h$")
list(FILTER DAPUI_CORE_HEADERS EXCLUDE REGEX ".*/DapNodeTransactionHistory\\.h$")
list(FILTER DAPUI_CORE_HEADERS EXCLUDE REGEX ".*/DapNodeWalletData\\.h$")
list(FILTER DAPUI_CORE_HEADERS EXCLUDE REGEX ".*/DapCdbManager\\.h$")
# DapGeoIP header (migrated to Common/Service/GeoIP)
list(FILTER DAPUI_CORE_HEADERS EXCLUDE REGEX ".*/DapGeoIP\\.h$")
# DapBugReportHistory header (migrated to Common/Service/BugReport)
list(FILTER DAPUI_CORE_HEADERS EXCLUDE REGEX ".*/DapBugReportHistory\\.h$")
file(GLOB_RECURSE DAPUI_VPN_COMMON_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/*.cpp
)
file(GLOB_RECURSE DAPUI_VPN_COMMON_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/*.h
)

# Stream layer (MIGRATED to DapChainVpnService/vpn/stream/)
# All stream classes moved to DapChainVpnService/vpn during Architecture V3 migration
# Commenting out to avoid multiple definition errors with dap-vpn-controller
# file(GLOB DAPUI_STREAM_SOURCES
#   ${CMAKE_CURRENT_SOURCE_DIR}/stream/*.cpp
# )
# file(GLOB DAPUI_STREAM_HEADERS
#   ${CMAKE_CURRENT_SOURCE_DIR}/stream/*.h
# )
set(DAPUI_STREAM_SOURCES)  # Empty - migrated
set(DAPUI_STREAM_HEADERS)  # Empty - migrated

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

# TUN drivers (MIGRATED to DapChainVpnService/vpn/)
# All TUN/TAP classes moved to service layer as part of Architecture V3
set(DAPUI_TUN_SOURCES)
set(DAPUI_TUN_HEADERS)

# VPN client state machine (migrated to Common/StateMachine)
# file(GLOB DAPUI_VPN_STATE_MACHINE
#   ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client/DapStateMachine/*.cpp
# )
set(DAPUI_VPN_STATE_MACHINE)  # Empty - migrated to Common/StateMachine

# Service client (MIGRATED to Common/Service/IPC/)
# All IPC client classes moved to modular structure as part of Architecture V3
set(DAPUI_VPN_SERVICE_SOURCES)  # Empty

## Network monitor now provided by separate target dap_network_monitor

# Exclude tests
list(FILTER DAPUI_CORE_SOURCES EXCLUDE REGEX ".*/core/test/.*")
list(FILTER DAPUI_CORE_HEADERS EXCLUDE REGEX ".*/core/test/.*")
list(FILTER DAPUI_VPN_COMMON_SOURCES EXCLUDE REGEX ".*/vpn/common/.*/.*Test.*\\.(cpp|c)$")
list(FILTER DAPUI_VPN_COMMON_HEADERS EXCLUDE REGEX ".*/vpn/common/.*/.*Test.*\\.(h|hpp)$")
list(FILTER DAPUI_VPN_COMMON_SOURCES EXCLUDE REGEX ".*/vpn/common/DapCommonTest/.*")

# Exclude classes migrated to Common/Commands/Common (Architecture V3)
# All command base classes moved to modular structure
list(REMOVE_ITEM DAPUI_VPN_COMMON_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/DapCommandControllerAbstract.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/DapServerConnectionInfo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/DapState.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/DapIndicator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/DapCmdAbstract.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/DapCmdParser.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/DapCmdClientAbstract.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/DapCmdServiceAbstract.cpp
)
list(REMOVE_ITEM DAPUI_VPN_COMMON_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/DapCommandControllerAbstract.h
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/DapServerConnectionInfo.h
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/DapState.h
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/DapIndicator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/DapCmdAbstract.h
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/DapCmdParser.h
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/DapCmdClientAbstract.h
  ${CMAKE_CURRENT_SOURCE_DIR}/vpn/common/DapCmdServiceAbstract.h
)



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
    # ${REPO_ROOT}/dap-ui-sdk/vpn/qml/handlers  # MIGRATED to Common/Commands/Client/ClientHandlers
    ${REPO_ROOT}/dap-ui-sdk/vpn/service
    ${REPO_ROOT}/dap-ui-sdk/ui/auxiliary
    ${REPO_ROOT}/brand/${BRAND}/DapChainVpnService
    ${REPO_ROOT}/DapNetworkMonitor
    # Architecture V3 - Migrated Common modules
    ${REPO_ROOT}/Common/Service/BugReport  
    ${REPO_ROOT}/Common/Service/DataLocal  
    ${REPO_ROOT}/Common/Service/Servers  
    ${REPO_ROOT}/Common/Service/SerialKey
    ${REPO_ROOT}/Common/Service/GeoIP
    ${REPO_ROOT}/Common/Crypto  
    ${REPO_ROOT}/Common/Network  
    ${REPO_ROOT}/Common/Utils
    ${REPO_ROOT}/Common/StateMachine
    ${REPO_ROOT}/Common/Commands/Common
    ${REPO_ROOT}/DapChainVpnService/vpn  
    ${REPO_ROOT}/DapChainVpnService/web3
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
    Qt6::Widgets
    dap_network_monitor
    cellframe-sdk
    ZLIB::ZLIB
)

if(TARGET Qt6::Xml)
  target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Xml)
endif()

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
# Service Handlers migrated to Common/Commands/Service/ServiceHandlers
# file(GLOB DAPUI_CLIENT_CMD_HANDLERS
#   ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client/DapCmdHandlers/*.cpp
# )
# add_library(dap_ui_handlers_service STATIC ${DAPUI_CLIENT_CMD_HANDLERS})
# target_include_directories(dap_ui_handlers_service
#   PUBLIC
#     ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client
#     ${CMAKE_CURRENT_SOURCE_DIR}/vpn/client/DapStateMachine
# )
# target_link_libraries(dap_ui_handlers_service PUBLIC dap_ui_sdk Qt6::Core Qt6::Network Qt6::StateMachine Qt6::Core5Compat)

# QML Handlers (MIGRATED to Common/Commands/Client/ClientHandlers/)
# All QML command handlers (24 files) moved to modular structure as part of Architecture V3
# Note: This library is kept as INTERFACE for backward compatibility with existing code
add_library(dap_ui_handlers_qml INTERFACE)
target_link_libraries(dap_ui_handlers_qml INTERFACE Qt6::Core Qt6::Network Qt6::StateMachine Qt6::Core5Compat)
