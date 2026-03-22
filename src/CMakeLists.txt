set(APP_NAME "qt-application")
set(BUILD_SHARED_LIBS ON)

project(${APP_NAME} LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)


find_package(Qt6 REQUIRED COMPONENTS Charts Core Gui Multimedia SerialPort DataVisualization Network)

add_executable(${APP_NAME} main.cpp app.h app.cpp)



FetchContent_MakeAvailable(BDSP)
find_package(BDSP)

set(APP_PROJECT_LIBS core analyzer visualization)

foreach(lib IN LISTS APP_PROJECT_LIBS)
    add_subdirectory(${lib})
endforeach()

add_subdirectory(aggregator)
add_subdirectory(core_app)

target_link_libraries(${APP_NAME} PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::SerialPort

        Qt::DataVisualization
        BDSP
        ${APP_PROJECT_LIBS}
)
