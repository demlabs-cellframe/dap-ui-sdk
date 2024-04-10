cmake_minimum_required(VERSION 3.16)

project(dapuisdk_vpn_qml CXX)

add_subdirectory(handlers)
add_subdirectory(model)
add_subdirectory(forms)
add_subdirectory(widgets)

add_library(DapUiSdk::Vpn::Qml::Handlers ALIAS dapuisdk_vpn_qml_handlers)
add_library(DapUiSdk::Vpn::Qml::Forms ALIAS dapuisdk_vpn_qml_forms)
add_library(DapUiSdk::Vpn::Qml::Model ALIAS dapuisdk_vpn_qml_model)
add_library(DapUiSdk::Vpn::Qml::Widgets ALIAS dapuisdk_vpn_qml_widgets)
