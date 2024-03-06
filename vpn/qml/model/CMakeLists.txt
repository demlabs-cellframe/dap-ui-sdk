# ------------------------------------------------------------------------- #
# project settings
# ------------------------------------------------------------------------- #

cmake_minimum_required(VERSION 3.16)

project(dapuisdk_vpn_qml_model CXX)

# ------------------------------------------------------------------------- #
# requirements
# ------------------------------------------------------------------------- #

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(Qt6 REQUIRED COMPONENTS Core Core5Compat Gui Widgets Qml Quick QuickControls2)

# ------------------------------------------------------------------------- #
# files
# ------------------------------------------------------------------------- #

set(PROJECT_SOURCES
  dapqml-abstract/abstractserverlistmodelbridge.cpp
  dapqml-model/dapqmlmodelautoserverlist.cpp
  dapqml-model/dapqmlmodelbugreports.cpp
  dapqml-model/dapqmlmodelchoosecert.cpp
  dapqml-model/dapqmlmodelchoosecountry.cpp
  dapqml-model/dapqmlmodelchooseserver.cpp
  dapqml-model/dapqmlmodelfullserverlist.cpp
  dapqml-model/dapqmlmodelfaq.cpp
  dapqml-model/dapqmlmodelroutingexceptions.cpp
  dapqml-model/dapqmlmodelserialhistory.cpp
  dapqml-model/dapqmlservermanager.cpp
  dapqml-model/dapqmlmodelmanagecdb.cpp
  #dapqml-model/dapqmlmodelmanageservers.cpp
  dapqml-model/dapqmlmodelsettings.cpp
  dapqml-model/node/dapqmlmodelnodenetworkslist.cpp
  dapqml-model/node/dapqmlmodelnodeorderlist.cpp
  dapqml-model/node/dapqmlmodelnodeproxybase.cpp
  dapqml-model/node/dapqmlmodelnodetokenslist.cpp
  dapqml-model/node/dapqmlmodelnodeunitslist.cpp
  dapqml-model/node/dapqmlmodelnodewalletslist.cpp
)

set(PROJECT_HEADERS
  dapqml-abstract/abstractcdbmanager.h
  dapqml-abstract/abstractcertificate.h
  dapqml-abstract/abstractserverlistmodelbridge.h
  dapqml-abstract/abstractservermanager.h
  dapqml-model/dapqmlmodelautoserverlist.h
  dapqml-model/dapqmlmodelbugreports.h
  dapqml-model/dapqmlmodelchoosecert.h
  dapqml-model/dapqmlmodelchoosecountry.h
  dapqml-model/dapqmlmodelchooseserver.h
  dapqml-model/dapqmlmodelfullserverlist.h
  dapqml-model/dapqmlmodelfaq.h
  dapqml-model/dapqmlmodelroutingexceptions.h
  dapqml-model/dapqmlmodelserialhistory.h
  dapqml-model/dapqmlservermanager.h
  dapqml-model/doc/doc-server-lists-and-models.h
  dapqml-model/dapqmlmodelmanagecdb.h
  #dapqml-model/dapqmlmodelmanageservers.h
  dapqml-model/dapqmlmodelsettings.h
  dapqml-model/node/dapqmlmodelnodedefines.h
  dapqml-model/node/dapqmlmodelnodenetworkslist.h
  dapqml-model/node/dapqmlmodelnodeorderlist.h
  dapqml-model/node/dapqmlmodelnodeproxybase.h
  dapqml-model/node/dapqmlmodelnodetokenslist.h
  dapqml-model/node/dapqmlmodelnodeunitslist.h
  dapqml-model/node/dapqmlmodelnodewalletslist.h
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

add_library(${PROJECT_NAME}
  STATIC
  ${PROJECT_SOURCES}
  ${PROJECT_HEADERS}
  ${PROJECT_ASSETS}
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Core5Compat
  Qt6::Qml
  Qt6::Quick
  DapUiSdk::Core
  dap_crypto
)

# ------------------------------------------------------------------------- #
