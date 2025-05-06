cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(dapuisdk_core CXX)

qt_standard_project_setup()

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core5Compat)
find_package(Qt6 REQUIRED COMPONENTS StateMachine)
find_package(Qt6 REQUIRED COMPONENTS Network)

file(GLOB DAP_UI_SDK_VPN_CORE_NATIVE_HDR *.h)
if (IOS)
    find_library(FOUNDATION Foundation)
    if (NOT FOUNDATION) 
        message(FATAL_ERROR "Could not found foundation framework")
    endif()
    set (LDOBJC_FLAGS "-lobjc -framework Foundation")
    file(GLOB DAP_UI_SDK_VPN_CORE_NATIVE_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/*.m
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
else()
    file(GLOB DAP_UI_SDK_VPN_CORE_NATIVE_SRC  *.cpp)
endif()
list(REMOVE_ITEM DAP_UI_SDK_VPN_CORE_NATIVE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/DapShopManager.cpp)
list(REMOVE_ITEM DAP_UI_SDK_VPN_CORE_NATIVE_HDR ${CMAKE_CURRENT_SOURCE_DIR}/DapShopManager.h)

add_library(${PROJECT_NAME} STATIC ${DAP_UI_SDK_VPN_CORE_NATIVE_SRC} ${DAP_UI_SDK_VPN_CORE_NATIVE_HDR})
target_include_directories(${PROJECT_NAME} PUBLIC ./)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core5Compat)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Network)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core5Compat)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::StateMachine)
target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Vpn::Client)
target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Vpn::Common)
target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Zip)
target_link_libraries(${PROJECT_NAME} PRIVATE dap_client)
target_link_libraries(${PROJECT_NAME} PUBLIC maxminddb)


message("[*] DAP-UI-SDK: include core")
