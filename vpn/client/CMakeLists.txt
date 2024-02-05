cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(dapuisdk_vpn_client CXX)

qt_standard_project_setup()

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core5Compat)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Xml)


file(GLOB DAP_UI_SDK_VPN_CLIENT_NATIVE_SRC  *.cpp ./DapCmdHandlers/*.cpp ./DapStateMachine/*.cpp )
file(GLOB DAP_UI_SDK_VPN_CLIENT_NATIVE_HDR *.h ./DapCmdHandlers/*.h ./DapStateMachine/*.h)

#linux
file(GLOB DAP_UI_SDK_VPN_CLIENT_LINUX_SRC  ./linux-src/*.cpp)
file(GLOB DAP_UI_SDK_VPN_CLIENT_LINUX_HDR  ./linux-src/*.h)
list (APPEND DAP_UI_SDK_VPN_CLIENT_NATIVE_SRC ${DAP_UI_SDK_VPN_CLIENT_LINUX_SRC})
list (APPEND DAP_UI_SDK_VPN_CLIENT_NATIVE_HDR ${DAP_UI_SDK_VPN_CLIENT_LINUX_HDR})

list(REMOVE_ITEM DAP_UI_SDK_VPN_CLIENT_NATIVE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/DapTunAndroid.cpp)
list(REMOVE_ITEM DAP_UI_SDK_VPN_CLIENT_NATIVE_HDR ${CMAKE_CURRENT_SOURCE_DIR}/DapTunAndroid.h)
list(REMOVE_ITEM DAP_UI_SDK_VPN_CLIENT_NATIVE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/DapTunMac.cpp)
list(REMOVE_ITEM DAP_UI_SDK_VPN_CLIENT_NATIVE_HDR ${CMAKE_CURRENT_SOURCE_DIR}/DapTunMac.h)
list(REMOVE_ITEM DAP_UI_SDK_VPN_CLIENT_NATIVE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/DapTunWindows.cpp)
list(REMOVE_ITEM DAP_UI_SDK_VPN_CLIENT_NATIVE_HDR ${CMAKE_CURRENT_SOURCE_DIR}/DapTunWindows.h)
list(REMOVE_ITEM DAP_UI_SDK_VPN_CLIENT_NATIVE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/DapTunIOS.cpp)
list(REMOVE_ITEM DAP_UI_SDK_VPN_CLIENT_NATIVE_HDR ${CMAKE_CURRENT_SOURCE_DIR}/DapTunIOS.h)

list(REMOVE_ITEM DAP_UI_SDK_VPN_CLIENT_NATIVE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/DapTunWorkerAndroid.cpp)
list(REMOVE_ITEM DAP_UI_SDK_VPN_CLIENT_NATIVE_HDR ${CMAKE_CURRENT_SOURCE_DIR}/DapTunWorkerAndroid.h)
list(REMOVE_ITEM DAP_UI_SDK_VPN_CLIENT_NATIVE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/DapTunWorkerMac.cpp)
list(REMOVE_ITEM DAP_UI_SDK_VPN_CLIENT_NATIVE_HDR ${CMAKE_CURRENT_SOURCE_DIR}/DapTunWorkerMac.h)
list(REMOVE_ITEM DAP_UI_SDK_VPN_CLIENT_NATIVE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/DapTunWorkerDarwin.cpp)
list(REMOVE_ITEM DAP_UI_SDK_VPN_CLIENT_NATIVE_HDR ${CMAKE_CURRENT_SOURCE_DIR}/DapTunWorkerDarwin.h)
list(REMOVE_ITEM DAP_UI_SDK_VPN_CLIENT_NATIVE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/DapTunWorkerWindows.cpp)
list(REMOVE_ITEM DAP_UI_SDK_VPN_CLIENT_NATIVE_HDR ${CMAKE_CURRENT_SOURCE_DIR}/DapTunWorkerWindows.h)
list(REMOVE_ITEM DAP_UI_SDK_VPN_CLIENT_NATIVE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/DapTunWorkerIOS.cpp)
list(REMOVE_ITEM DAP_UI_SDK_VPN_CLIENT_NATIVE_HDR ${CMAKE_CURRENT_SOURCE_DIR}/DapTunWorkerIOS.h)

#endif linuex

add_library(${PROJECT_NAME} STATIC ${DAP_UI_SDK_VPN_CLIENT_NATIVE_SRC} ${DAP_UI_SDK_VPN_CLIENT_NATIVE_HDR})

target_include_directories(${PROJECT_NAME} PUBLIC ./)
target_include_directories(${PROJECT_NAME} PUBLIC ./DapCmdHandlers)
target_include_directories(${PROJECT_NAME} PUBLIC ./DapStateMachine)

#linux
target_include_directories(${PROJECT_NAME} PUBLIC ./linux-src/)

#endif linuex

#brand includes (DapChainServiceDefenitions.h) provided from outside
target_include_directories(${PROJECT_NAME} PUBLIC ${BRAND_SERVICE_INCLUDES})
#network monitor includes
target_include_directories(${PROJECT_NAME} PUBLIC ${DAP_NETWORK_MONITOR_INCLUDES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Xml)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Network)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core5Compat)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::StateMachine)

target_link_libraries(${PROJECT_NAME} PRIVATE dap_client)
target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Stream)
target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Vpn::Common)

message("[*] DAP-UI-SDK: include vpn/client")