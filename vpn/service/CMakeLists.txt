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


if(UNIX)
    file(GLOB DAP_UI_SDK_VPN_SERVICE_PLATFORM_SRC  ./platform/unix/*.cpp)
    file(GLOB DAP_UI_SDK_VPN_SERVICE_PLATFORM_HDR  ./platform/unix/*.h)
    set(DAP_UI_SDK_VPN_SERVICE_PLATFORM_INCLUDES   ./platform/unix/)
    message("[*] DAP-UI-SDK: include vpn/service unix")
endif()

if(WIN32)
    file(GLOB DAP_UI_SDK_VPN_SERVICE_PLATFORM_SRC  ./platform/windows/*.cpp)
    file(GLOB DAP_UI_SDK_VPN_SERVICE_PLATFORM_HDR  ./platform/windows/*.h)
    set(DAP_UI_SDK_VPN_SERVICE_PLATFORM_INCLUDES  ./platform/windows/)
    message("[*] DAP-UI-SDK: include vpn/service windows")
endif()

if(ANDROID)
    file(GLOB DAP_UI_SDK_VPN_SERVICE_PLATFORM_SRC  ./platform/android/*.cpp)
    file(GLOB DAP_UI_SDK_VPN_SERVICE_PLATFORM_HDR  ./platform/android/*.h)
    set(DAP_UI_SDK_VPN_SERVICE_PLATFORM_INCLUDES  ./platform/android/)
    message("[*] DAP-UI-SDK: include vpn/service android")
endif()

if(MACOS)
    file(GLOB DAP_UI_SDK_VPN_SERVICE_PLATFORM_SRC  ./platform/macos/*.cpp)
    file(GLOB DAP_UI_SDK_VPN_SERVICE_PLATFORM_HDR  ./platform/macos/*.h)
    set(DAP_UI_SDK_VPN_SERVICE_PLATFORM_INCLUDES  ./platform/macos/)
    message("[*] DAP-UI-SDK: include vpn/service macos")
endif()

if(IOS)
    file(GLOB DAP_UI_SDK_VPN_SERVICE_PLATFORM_SRC  ./platform/ios/*.cpp)
    file(GLOB DAP_UI_SDK_VPN_SERVICE_PLATFORM_HDR  ./platform/ios/*.h)
    set(DAP_UI_SDK_VPN_SERVICE_PLATFORM_INCLUDES  ./platform/ios/)
    message("[*] DAP-UI-SDK: include vpn/service ios")
endif()


add_library(${PROJECT_NAME} STATIC ${DAP_UI_SDK_VPN_SERVICE_NATIVE_SRC} ${DAP_UI_SDK_VPN_SERVICE_NATIVE_HDR} ${DAP_UI_SDK_VPN_SERVICE_PLATFORM_SRC} ${DAP_UI_SDK_VPN_SERVICE_PLATFORM_HDR})

target_include_directories(${PROJECT_NAME} PUBLIC ./)
target_include_directories(${PROJECT_NAME} PUBLIC ${DAP_UI_SDK_VPN_SERVICE_PLATFORM_INCLUDES})
target_include_directories(${PROJECT_NAME} PUBLIC ${BRAND_SERVICE_INCLUDES} )


target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
if(ANDROID)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::CorePrivate)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Network)
target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Vpn::Common)
target_link_libraries(${PROJECT_NAME} PRIVATE dap_core)

