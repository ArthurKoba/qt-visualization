# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME "qt-visualization")
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Qt/6.7.2/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Charts Core Gui)

add_subdirectory(src)

qt_standard_project_setup()

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt::Charts
    Qt::Core
    Qt::Gui
)

set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../BDSP")
add_subdirectory(${LIB_PATH} ${CMAKE_CURRENT_BINARY_DIR}/BDSP)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIB_PATH}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE BDSP)