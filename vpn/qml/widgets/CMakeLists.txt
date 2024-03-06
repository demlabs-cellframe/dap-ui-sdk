# ------------------------------------------------------------------------- #
# project settings
# ------------------------------------------------------------------------- #

cmake_minimum_required(VERSION 3.16)

project(dapuisdk_vpn_qml_widgets CXX)

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
  dapqml-widgets/dapimage.cpp
  dapqml-widgets/dapqmlarcanimation.cpp
  dapqml-widgets/dapqmlimage.cpp
  dapqml-widgets/dapqmllistvieweventfilter.cpp
  dapqml-widgets/dapqmlseparatoritem.cpp
  dapqml-widgets/dapqmlserialkeyinput.cpp
  dapqml-widgets/dapqmlstyle.cpp
  dapqml-widgets/dapqmlswitchctl.cpp
  dapqml-widgets/dapqmlthememodel.cpp
  dapqml-widgets/listvieweventfilter/lefcollector.cpp
  dapqml-widgets/listvieweventfilter/lefdisablemomentum.cpp
  dapqml-widgets/listvieweventfilter/lefscrollfix.cpp
  dapqml-widgets/listvieweventfilter/leftest.cpp
  dapqml-widgets/style/debug/styledebugitemdescriptor.cpp
  dapqml-widgets/style/qssitem.cpp
  dapqml-widgets/style/qsslink.cpp
  dapqml-widgets/style/qssmap.cpp
  dapqml-widgets/style/qssparser.cpp
  dapqml-widgets/style/qssvalue.cpp
  dapqml-widgets/style/scaled.cpp
  dapqml-widgets/style/debug/styledebugtree.cpp
)

set(PROJECT_HEADERS
  dapqml-widgets/dapimage.h
  dapqml-widgets/dapqmlarcanimation.h
  dapqml-widgets/dapqmlimage.h
  dapqml-widgets/dapqmllistvieweventfilter.h
  dapqml-widgets/dapqmlseparatoritem.h
  dapqml-widgets/dapqmlserialkeyinput.h
  dapqml-widgets/dapqmlstyle.h
  dapqml-widgets/listvieweventfilter/lefbase.h
  dapqml-widgets/dapqmlswitchctl.h
  dapqml-widgets/dapqmlthememodel.h
  dapqml-widgets/doc-group-dapqmlstyle.h
  dapqml-widgets/doc-group-qml-widgets.h
  dapqml-widgets/listvieweventfilter/lefcollector.h
  dapqml-widgets/listvieweventfilter/lefdisablemomentum.h
  dapqml-widgets/listvieweventfilter/lefscrollfix.h
  dapqml-widgets/listvieweventfilter/leftest.h
  dapqml-widgets/style/debug/styledebugitemdescriptor.h
  dapqml-widgets/style/qssitem.h
  dapqml-widgets/style/qsslink.h
  dapqml-widgets/style/qssmap.h
  dapqml-widgets/style/qssparser.h
  dapqml-widgets/style/qssvalue.h
  dapqml-widgets/style/scaled.h
  dapqml-widgets/style/debug/styledebugtree.h
)

set(PROJECT_ASSETS
  dapqml-widgets-asset/brand/${BRAND}/dapqml-widgets-asset-brand.qrc
  dapqml-widgets-asset/brand/${BRAND}/dapqml-widgets-style-brand.qrc
  dapqml-widgets-asset/nonbrand/nonbrandAsset.qrc
  dapqml-widgets.qrc
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
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../model
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
  DapUiSdk::Vpn::Qml::Model
)

# ------------------------------------------------------------------------- #
