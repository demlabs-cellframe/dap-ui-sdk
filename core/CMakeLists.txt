project(dap-ui-core)
add_library(${PROJECT_NAME} STATIC)

file(GLOB_RECURSE SOURCES
    DapLogger.cpp
)

file(GLOB_RECURSE HEADERS
    DapLogger.h
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) 

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt6::Core
    Qt6::Core5Compat
    Qt6::Network
    dap_core
    dap_crypto
    dap_client

)