# ------------------------------------------------------------------------- #
# project settings
# ------------------------------------------------------------------------- #

cmake_minimum_required(VERSION 3.16)

project(dapuisdk_vpn_qml_handlers CXX)

# ------------------------------------------------------------------------- #
# requirements
# ------------------------------------------------------------------------- #

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

# ------------------------------------------------------------------------- #
# files
# ------------------------------------------------------------------------- #

set(PROJECT_SOURCES
  DapCmdBugReportsStatus.cpp
  DapCmdCdbCtl.cpp
  DapCmdLastConnectionData.cpp
  DapCmdNews.cpp
  DapCmdPingServer.cpp
  DapCmdResetSeialKey.cpp
  DapCmdSignUp.cpp
  DapCmdSendBugReport.cpp
  DapCmdStats.cpp
  DapCmdTunTap.cpp
  DapCmdServerData.cpp
  DapCmdUpdateOperation.cpp
  DapCmdUserData.cpp
  DapCmdStates.cpp
  DapCmdConnect.cpp
  DapCmdAuth.cpp
  DapCmdServersList.cpp
  DapCmdAndroidTunnel.cpp
  DapCmdNode.cpp
  DapCmdSystemTray.cpp
  DapCmdPurchase.cpp
)

set(PROJECT_HEADERS
  DapCmdLastConnectionData.h
  DapCmdBugReportsStatus.h
  DapCmdCdbCtl.h
  DapCmdNews.h
  DapCmdPingServer.h
  DapCmdResetSeialKey.h
  DapCmdSignUp.h
  DapCmdSendBugReport.h
  DapCmdStats.h
  DapCmdTunTap.h
  DapCmdServerData.h
  DapCmdUpdateOperation.h
  DapCmdUserData.h
  DapCmdStates.h
  DapCmdConnect.h
  DapCmdAuth.h
  DapCmdServersList.h
  DapCmdAndroidTunnel.h
  DapCmdSystemTray.h
  DapCmdNode.h
  vpnDefine.h
  DapCmdPurchase.h
)

set(PROJECT_ASSETS
)

# ------------------------------------------------------------------------- #
# groups
# ------------------------------------------------------------------------- #

source_group("Source Files" FILES ${PROJECT_SOURCES})
source_group("Header Files" FILES ${PROJECT_HEADERS})
source_group("Assets" FILES ${PROJECT_ASSETS})

# ------------------------------------------------------------------------- #
# library
# ------------------------------------------------------------------------- #

add_library(
  ${PROJECT_NAME}
  STATIC
  ${PROJECT_SOURCES}
  ${PROJECT_HEADERS}
  ${PROJECT_ASSETS}
)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
  Qt6::Core
  Qt6::Core5Compat
  Qt6::Gui
  Qt6::Widgets
  DapUiSdk::Core
  DapUiSdk::Ui
  DapUiSdk::Vpn::Common
  dap_crypto
)

# ------------------------------------------------------------------------- #
