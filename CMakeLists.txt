cmake_minimum_required(VERSION 3.30)
set(PROJECT_NAME "qt-visualization")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)

find_package(Qt6 REQUIRED COMPONENTS Charts Core Gui Multimedia SerialPort DataVisualization Network)

add_subdirectory(src)
