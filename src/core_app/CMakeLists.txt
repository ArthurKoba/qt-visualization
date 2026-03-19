cmake_minimum_required(VERSION 3.16)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTORCC OFF)

set(HEADERS
        ${CALC_SOURCE_FILES}
        include/core_app/types.h
        include/core_app/core_application.h
        include/core_app/settings_widget.h
        include/core_app/main_window.h
        include/core_app/scheme_editor/editor_widget.h
        include/core_app/scheme_editor/sink_data_model.h
        include/core_app/scheme_editor/source_data_model.h
        include/core_app/scheme_editor/processor_data_model.h
        include/core_app/scheme_editor/types.h
)

set(SOURCES
        ${CALC_SOURCE_FILES}
        src/core_application.cpp
        src/main_window.cpp
        src/settings_widget.cpp

        src/scheme_editor/editor_widget.cpp

        src/scheme_editor/sink_data_model.cpp
        src/scheme_editor/source_data_model.cpp
        src/scheme_editor/processor_data_model.cpp
        src/scheme_editor/types.cpp
)

add_executable(core_app ${SOURCES} ${HEADERS} "src/main.cpp")
target_include_directories(core_app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(core_app Qt6::Core Qt6::Widgets QtNodes)



FetchContent_Declare(
        QtNodes
        GIT_REPOSITORY https://github.com/paceholder/nodeeditor.git
        GIT_TAG master
)
FetchContent_MakeAvailable(QtNodes)

target_link_libraries(core_app
        Qt6::Core
        Qt6::Widgets
#        Qt6::Network
#        aggregator-library
#        core
        QtNodes
)
