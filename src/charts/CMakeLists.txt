project(${PROJECT_NAME} LANGUAGES CXX)

add_library(charts SHARED)

target_link_libraries(charts PRIVATE
        Qt::Charts
)

target_sources(charts PUBLIC
        abs_view.h abs_view.cpp
        amplitudes.h amplitudes.cpp
        samples.h samples.cpp
)

set(CMAKE_BUILD_RPATH "${CMAKE_BINARY_DIR}/charts")
