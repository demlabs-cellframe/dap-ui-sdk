cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(dapuisdk_vpn_common CXX)

qt_standard_project_setup()
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS StateMachine)
find_package(Qt6 REQUIRED COMPONENTS Core5Compat)
find_package(Qt6 REQUIRED COMPONENTS Network)


file(GLOB DAP_UI_SDK_VPN_COMMON_NATIVE_SRC  *.cpp)
file(GLOB DAP_UI_SDK_VPN_COMMON_NATIVE_HDR *.h)

add_library(${PROJECT_NAME} STATIC ${DAP_UI_SDK_VPN_COMMON_NATIVE_SRC} ${DAP_UI_SDK_VPN_COMMON_NATIVE_HDR})
target_include_directories(${PROJECT_NAME} PUBLIC ./)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::StateMachine)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core5Compat)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Network)

target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Core)



message("[*] DAP-UI-SDK: include vpn/common")

#target_include_directories(${PROJECT_NAME} PUBLIC include/ )
#target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../dap-sdk/3rdparty/json-c)
