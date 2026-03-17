cmake_minimum_required(VERSION 3.30)

set(LIB_NAME "aggregator-library")
project(${LIB_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

if (BUILD_SHARED_LIBS)
    add_library(${LIB_NAME}  SHARED)
else ()
    add_library(${LIB_NAME} )
endif ()

target_sources(${LIB_NAME} PUBLIC
        include/aggregator/abstract/types.h

        include/aggregator/aggregator_server.h
        src/aggregator_server.cpp

        include/aggregator/aggregator_client.h
        src/aggregator_client.cpp
)

target_include_directories(${LIB_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${LIB_NAME} PUBLIC
        Qt6::Core
        Qt6::Network
        core
        BDSP
)

add_executable(aggregator_server_app src/main_server.cpp)
target_link_libraries(aggregator_server_app PRIVATE ${LIB_NAME})

add_executable(aggregator_client_app src/main_client.cpp)
target_link_libraries(aggregator_client_app PRIVATE ${LIB_NAME})


if (BUILD_SHARED_LIBS)
    add_custom_command(
            TARGET ${LIB_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:BDSP>"
            "$<TARGET_FILE:core>"
            "$<TARGET_FILE_DIR:${LIB_NAME}>"
    )
endif ()