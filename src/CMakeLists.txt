set(APP_NAME "qt-application")
set(BUILD_SHARED_LIBS ON)

project(${APP_NAME}  LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


add_executable(${APP_NAME} main.cpp)
target_sources(${APP_NAME} PRIVATE
        app.h app.cpp
        surfacegraph.cpp surfacegraph.h
        surfacegraphmodifier.cpp surfacegraphmodifier.h
)

qt6_add_resources(${APP_NAME}  "graphgallery"
        PREFIX
        "/"
        FILES
        "data/oilrig.obj"
        "data/pipe.obj"
)

include(FetchContent)

FetchContent_Declare(
        BDSP
        GIT_REPOSITORY https://github.com/KobaProduction/BDSP.git
        FIND_PACKAGE_ARGS NAMES BDSP
)

FetchContent_MakeAvailable(BDSP)
find_package(BDSP)

set(APP_PROJECT_LIBS
        audio core charts analyzer
)

foreach(lib IN LISTS APP_PROJECT_LIBS)
    add_subdirectory(${lib})
endforeach()

add_subdirectory(aggregator)

target_link_libraries(${APP_NAME} PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::SerialPort

        Qt::DataVisualization
        BDSP
        ${APP_PROJECT_LIBS}
)

if (BUILD_SHARED_LIBS)
    set(LIB_FILES "")
    foreach(lib IN LISTS APP_PROJECT_LIBS)
        list(APPEND LIB_FILES "$<TARGET_FILE:${lib}>")
    endforeach()

    list(APPEND LIB_FILES "$<TARGET_FILE:BDSP>")

    add_custom_command(
            TARGET ${APP_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIB_FILES}
            "$<TARGET_FILE_DIR:${APP_NAME}>"
    )
endif ()


#if(WIN32)
#    find_program(WINDEPLOYQT windeployqt HINTS "${Qt6_DIR}/../../../bin")
#    if(WINDEPLOYQT)
#        add_custom_command(TARGET ${APP_NAME} POST_BUILD
#                COMMAND "${WINDEPLOYQT}"
#                --qmldir "${CMAKE_SOURCE_DIR}"
#                "$<TARGET_FILE:${APP_NAME}>"
#                COMMENT "Running windeployqt..."
#        )
#    endif()
#endif()

