cmake_minimum_required(VERSION 3.16)

include(../cellframe-sdk/dap-sdk/cmake/OS_Detection.cmake)
project(DAP_UI_SDK_NATIVE CXX)

add_subdirectory(stream)
add_subdirectory(core)
add_subdirectory(vpn)
add_subdirectory(zip)
add_subdirectory(crypto)
add_subdirectory(ui)

add_library(DapUiSdk::Core ALIAS dapuisdk_core)
add_library(DapUiSdk::Stream ALIAS dapuisdk_stream)
add_library(DapUiSdk::Zip ALIAS dapuisdk_zip)
add_library(DapUiSdk::Crypto ALIAS dapuisdk_crypto)
add_library(DapUiSdk::Ui ALIAS dapuisdk_ui)
