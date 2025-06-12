add_library(core SHARED)

target_sources(core PUBLIC
    abs_task.h abs_task.cpp
)

set(CMAKE_BUILD_RPATH "${CMAKE_BINARY_DIR}/core")