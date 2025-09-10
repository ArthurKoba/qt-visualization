if (BUILD_SHARED_LIBS)
    add_library(charts SHARED)
else ()
    add_library(charts)
endif ()

target_link_libraries(charts
        Qt::Charts
)

set(public_headers
        include/charts/abs.h
        include/charts/fps.h
        include/charts/bar.h
        include/charts/line.h
        include/charts/mixins.h

        include/charts/freq.h
)

set(sources
        ${public_headers}
        abs.cpp
        fps.cpp
        bar.cpp
        line.cpp
        mixins.cpp
        freq.cpp
)

target_sources(charts PRIVATE ${sources})

target_include_directories(charts PUBLIC include)
target_include_directories(charts PRIVATE include/charts)
