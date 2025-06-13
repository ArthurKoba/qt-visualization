if (BUILD_SHARED_LIBS)
    add_library(analyzer SHARED)
else ()
    add_library(analyzer)
endif ()

add_subdirectory(dsp)
add_subdirectory(generator)

target_sources(analyzer PUBLIC
        analyzer.cpp fft.cpp
)

target_link_libraries(analyzer PUBLIC
        Qt::Core
        dsp
        core
)