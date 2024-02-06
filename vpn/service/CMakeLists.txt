cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(dapuisdk_vpn_service CXX)

qt_standard_project_setup()

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Network)


file(GLOB DAP_UI_SDK_VPN_SERVICE_NATIVE_SRC  *.cpp)
file(GLOB DAP_UI_SDK_VPN_SERVICE_NATIVE_HDR *.h)

#Linux

list(REMOVE_ITEM DAP_UI_SDK_VPN_SERVICE_NATIVE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/DapServiceNativeAndroid.cpp)
list(REMOVE_ITEM DAP_UI_SDK_VPN_SERVICE_NATIVE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/DapServiceNativeDarwin.cpp)
list(REMOVE_ITEM DAP_UI_SDK_VPN_SERVICE_NATIVE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/DapServiceNativeIOS.cpp)
list(REMOVE_ITEM DAP_UI_SDK_VPN_SERVICE_NATIVE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/DapServiceNativeWindows.cpp)


add_library(${PROJECT_NAME} STATIC ${DAP_UI_SDK_VPN_SERVICE_NATIVE_SRC} ${DAP_UI_SDK_VPN_SERVICE_NATIVE_HDR})

target_include_directories(${PROJECT_NAME} PUBLIC ./)
target_include_directories(${PROJECT_NAME} PUBLIC ${BRAND_SERVICE_INCLUDES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Network)
target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Vpn::Common)

message("[*] DAP-UI-SDK: include vpn/service")
