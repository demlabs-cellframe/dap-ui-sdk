cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(dapuisdk_vpn_qml CXX)

qt_standard_project_setup()

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core5Compat)
find_package(Qt6 REQUIRED COMPONENTS Gui)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Qml)
find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS QuickControls2)



file(GLOB DAP_UI_SDK_VPN_QML_SRC  handlers/*.cpp 
                                    model/dapqml-abstract/*.cpp 
                                    model/dapqml-model/*.cpp 
                                    model/dapqml-model/modules/*.cpp 
                                    widgets/dapqml-widgets/*.cpp
                                    widgets/dapqml-widgets/style/*.cpp
                                    widgets/dapqml-widgets/listvieweventfilter/*.cpp
                                    widgets/dapqml-widgets/style/debug/*.cpp
                                    )
file(GLOB DAP_UI_SDK_VPN_QML_HDR  handlers/*.h 
                                    model/dapqml-abstract/*.h 
                                    model/dapqml-model/*.h 
                                    model/dapqml-model/modules/*.h 
                                    widgets/dapqml-widgets/*.h
                                    widgets/dapqml-widgets/style/*.h
                                    widgets/dapqml-widgets/listvieweventfilter/*.h
                                    widgets/dapqml-widgets/style/debug/*.h)

add_library(${PROJECT_NAME} STATIC ${DAP_UI_SDK_VPN_QML_SRC} ${DAP_UI_SDK_VPN_QML_HDR})

target_include_directories(${PROJECT_NAME} PUBLIC handlers/)
target_include_directories(${PROJECT_NAME} PUBLIC model/)
target_include_directories(${PROJECT_NAME} PUBLIC widgets/)


target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Vpn::Common)
target_link_libraries(${PROJECT_NAME} PRIVATE DapUiSdk::Ui)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core5Compat)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Gui)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Qml)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Quick)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::QuickControls2)

target_link_libraries(${PROJECT_NAME} PRIVATE dap_crypto)


message("[*] DAP-UI-SDK: include vpn/qml")