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



if(ANDROID)
    file(GLOB DAP_UI_SDK_VPN_CLIENT_PLATFORM_SRC    ./daptun/android/*.cpp ./daptun/unix/*.cpp)
    file(GLOB DAP_UI_SDK_VPN_CLIENT_PLATFORM_HDR    ./daptun/android/*.h ./daptun/unix/*.h)
    set(DAP_UI_SDK_VPN_PUBLIC_PLATFORM_INCLUDES     ./daptun/android/ ./daptun/unix/)
    message("[*] DAP-UI-SDK: include vpn/clien android + unix")
else()
if (UNIX AND NOT APPLE) #not android, assume it linux
    file(GLOB DAP_UI_SDK_VPN_CLIENT_PLATFORM_SRC    ./daptun/unix/*.cpp ./daptun/linux/*.cpp)
    file(GLOB DAP_UI_SDK_VPN_CLIENT_PLATFORM_HDR    ./daptun/unix/*.h ./daptun/linux/*.h)
    set(DAP_UI_SDK_VPN_PUBLIC_PLATFORM_INCLUDES     ./daptun/unix/ ./daptun/linux/)
    message("[*] DAP-UI-SDK: include vpn/clien unix + linux")
endif()
if (MACOS)
    file(GLOB DAP_UI_SDK_VPN_CLIENT_PLATFORM_SRC    ./daptun/unix/*.cpp ./daptun/macos/*.cpp)
    file(GLOB DAP_UI_SDK_VPN_CLIENT_PLATFORM_HDR    ./daptun/unix/*.h ./daptun/macos/*.h)
    set(DAP_UI_SDK_VPN_PUBLIC_PLATFORM_INCLUDES     ./daptun/unix/ ./daptun/macos/)
    message("[*] DAP-UI-SDK: include vpn/clien unix + macOS")
endif()
endif()

if (WIN32)
    file(GLOB DAP_UI_SDK_VPN_CLIENT_PLATFORM_SRC    ./daptun/windows/*.cpp ./daptun/windows/tuntap/*.cpp)
    file(GLOB DAP_UI_SDK_VPN_CLIENT_PLATFORM_HDR    ./daptun/windows/*.h  ./daptun/windows/tuntap/*.h)
    
    set(DAP_UI_SDK_VPN_PUBLIC_PLATFORM_INCLUDES     ./daptun/windows/ ./daptun/windows/tuntap/)
    message("[*] DAP-UI-SDK: include vpn/clien windows")
endif()

add_library(${PROJECT_NAME} STATIC ${DAP_UI_SDK_VPN_CLIENT_NATIVE_SRC} ${DAP_UI_SDK_VPN_CLIENT_PLATFORM_SRC} ${DAP_UI_SDK_VPN_CLIENT_NATIVE_HDR} ${DAP_UI_SDK_VPN_CLIENT_PLATFORM_HDR})

target_include_directories(${PROJECT_NAME} PUBLIC ./)
target_include_directories(${PROJECT_NAME} PUBLIC ./DapCmdHandlers)
target_include_directories(${PROJECT_NAME} PUBLIC ./DapStateMachine)
target_include_directories(${PROJECT_NAME} PUBLIC ${DAP_UI_SDK_VPN_PUBLIC_PLATFORM_INCLUDES} )


#brand includes (DapChainServiceDefenitions.h) provided from outside
target_include_directories(${PROJECT_NAME} PUBLIC ${BRAND_SERVICE_INCLUDES})
#network monitor includes


target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Xml)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Network)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core5Compat)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::StateMachine)

target_link_libraries(${PROJECT_NAME} PRIVATE dap_client)
target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Stream)
target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Vpn::Common)
target_link_libraries(${PROJECT_NAME} PRIVATE dap_networkmonitor)