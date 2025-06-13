if (BUILD_SHARED_LIBS)
    add_library(charts SHARED)
else ()
    add_library(charts)
endif ()

target_link_libraries(charts
        Qt::Charts
)

set(public_headers
        include/charts/fps.h
        include/charts/mixins.h
)

set(sources
        ${public_headers}
        fps.cpp
        mixins.cpp
)

target_sources(charts PRIVATE ${sources})

target_include_directories(charts PUBLIC include)
target_include_directories(charts PRIVATE include/charts)
