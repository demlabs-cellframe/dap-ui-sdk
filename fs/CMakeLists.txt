project(dap-ui-fs)
add_library(${PROJECT_NAME} STATIC)

file(GLOB_RECURSE SOURCES
    *.cpp
)

file(GLOB_RECURSE HEADERS
    *.h
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) 

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt6::Core
)