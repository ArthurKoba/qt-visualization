project(${PROJECT_NAME} LANGUAGES CXX)

add_library(audio)

add_subdirectory(loopback)

set(CMAKE_BUILD_RPATH "${CMAKE_BINARY_DIR}/audio")