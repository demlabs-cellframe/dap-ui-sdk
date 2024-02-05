cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(dapuisdk_stream CXX)

qt_standard_project_setup()

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core5Compat)
find_package(Qt6 REQUIRED COMPONENTS Network)

file(GLOB DAP_UI_SDK_STREAM_NATIVE_SRC  *.cpp)
file(GLOB DAP_UI_SDK_STREAM_NATIVE_HDR *.h)


add_library(${PROJECT_NAME} STATIC ${DAP_UI_SDK_STREAM_NATIVE_SRC} ${DAP_UI_SDK_STREAM_NATIVE_HDR})
target_include_directories(${PROJECT_NAME} PUBLIC ./)

#brand includes (DapChainServiceDefenitions.h) provided from outside
target_include_directories(${PROJECT_NAME} PUBLIC ${BRAND_SERVICE_INCLUDES})

target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Network)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core5Compat)
target_link_libraries(${PROJECT_NAME} PRIVATE dap_client)



message("[*] DAP-UI-SDK: include stream")