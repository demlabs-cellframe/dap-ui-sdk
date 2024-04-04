cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(dapuisdk_ui CXX)

qt_standard_project_setup()

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core5Compat)
find_package(Qt6 REQUIRED COMPONENTS Widgets)


file(GLOB DAP_UI_SDK_UI_SRC  auxiliary/*.cpp )
file(GLOB DAP_UI_SDK_UI_HDR  auxiliary/*.h )

add_library(${PROJECT_NAME} STATIC ${DAP_UI_SDK_UI_SRC} ${DAP_UI_SDK_UI_HDR})

target_include_directories(${PROJECT_NAME} PUBLIC ./auxiliary)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core5Compat)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Vpn::Common)

target_link_libraries(${PROJECT_NAME} PRIVATE dap_crypto)

#target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Xml)
#target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Network)

#target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::StateMachine)

#target_link_libraries(${PROJECT_NAME} PRIVATE dap_client)
#target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Stream)



message("[*] DAP-UI-SDK: include vpn/qml")