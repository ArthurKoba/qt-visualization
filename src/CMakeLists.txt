set(APP_NAME "qt-application")

set(BUILD_SHARED_LIBS ON)

add_executable(${APP_NAME} main.cpp)
target_sources(${APP_NAME} PRIVATE app.h app.cpp)

set(APP_PROJECT_LIBS
        audio core charts analyzer
)

foreach(lib IN LISTS APP_PROJECT_LIBS)
    add_subdirectory(${lib})
endforeach()

target_link_libraries(${APP_NAME} PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        ${APP_PROJECT_LIBS}
)

if (BUILD_SHARED_LIBS)
    set(LIB_FILES "")
    foreach(lib IN LISTS APP_PROJECT_LIBS)
        list(APPEND LIB_FILES "$<TARGET_FILE:${lib}>")
    endforeach()

    add_custom_command(
            TARGET ${APP_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${LIB_FILES}
            "$<TARGET_FILE_DIR:${APP_NAME}>"
    )
endif ()
