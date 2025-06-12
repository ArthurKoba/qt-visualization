add_library(analyzer)

add_subdirectory(dsp)

target_sources(analyzer PUBLIC
        analyzer.cpp fft.cpp
)

target_link_libraries(analyzer PUBLIC
        Qt::Core
        dsp
)
