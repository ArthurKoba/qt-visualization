cmake_minimum_required(VERSION 3.30)
set(PROJECT_NAME "qt-visualization")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Charts Core Gui Multimedia)

add_subdirectory(src)
