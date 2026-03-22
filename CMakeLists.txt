cmake_minimum_required(VERSION 3.30)
set(PROJECT_NAME "qt-visualization")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

include(FetchContent)

FetchContent_Declare(
        BDSP
        GIT_REPOSITORY https://github.com/KobaProduction/BDSP.git
        FIND_PACKAGE_ARGS NAMES BDSP
)

FetchContent_Declare(
        QtNodes
        GIT_REPOSITORY https://github.com/paceholder/nodeeditor.git
        GIT_TAG master
)

add_subdirectory(src)
