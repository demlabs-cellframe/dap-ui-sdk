cmake_minimum_required(VERSION 3.16)

project(DAP_UI_SDK_VPN_NATIVE CXX)

add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(qml)
add_subdirectory(service)

add_library(DapUiSdk::Vpn::Common ALIAS dapuisdk_vpn_common)
add_library(DapUiSdk::Vpn::Client ALIAS dapuisdk_vpn_client)