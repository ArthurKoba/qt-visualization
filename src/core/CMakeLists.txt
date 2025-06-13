if (BUILD_SHARED_LIBS)
    add_library(core SHARED)
else ()
    add_library(core)
endif ()

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
