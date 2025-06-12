add_library(core)

set(public_headers
        include/core/abs_task.h
)

set(sources
        ${public_headers}
        abs_task.cpp
)

target_sources(core PRIVATE ${sources})

target_include_directories(core PUBLIC include)
target_include_directories(core PRIVATE include/core)

set(CMAKE_BUILD_RPATH "${CMAKE_BINARY_DIR}/core")