# ------------------------------------------------------------------------- #
# project settings
# ------------------------------------------------------------------------- #

cmake_minimum_required(VERSION 3.16)

project(dapuisdk_vpn_qml_forms CXX)

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
)

set(PROJECT_HEADERS
  dapqml-forms/doc-group-qml-forms.h
)

set(PROJECT_ASSETS
  dapqml-forms-asset/${BRAND}/dapqml-forms-asset.qrc
  dapqml-forms.qrc
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
  Qt6::Core5Compat
  Qt6::Gui
  Qt6::Widgets
  Qt6::Qml
  Qt6::Quick
  Qt6::QuickControls2
)

# ------------------------------------------------------------------------- #
