cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(dapuisdk_crypto CXX)

qt_standard_project_setup()

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core5Compat)


file(GLOB DAP_UI_SDK_CRYPTO_NATIVE_SRC  src/*.cpp)
file(GLOB DAP_UI_SDK_CRYPTO_NATIVE_HDR  include/*.h)

add_library(${PROJECT_NAME} STATIC ${DAP_UI_SDK_CRYPTO_NATIVE_SRC} ${DAP_UDAP_UI_SDK_CRYPTO_NATIVE_HDRI_SDK_VPN_CORE_NATIVE_HDR})
target_include_directories(${PROJECT_NAME} PUBLIC ./include)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core5Compat)
target_link_libraries(${PROJECT_NAME} PRIVATE dap_core)
target_link_libraries(${PROJECT_NAME} PRIVATE dap_crypto)
target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Stream)



message("[*] DAP-UI-SDK: include crypto")