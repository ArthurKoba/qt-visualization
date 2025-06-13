add_library(audio)

set(public_headers
        include/loopback/types.h
        include/loopback/factory.h
)

set(sources
        ${public_headers}
        factory.cpp
)

target_sources(audio PRIVATE ${sources})

target_include_directories(audio PUBLIC include)
target_include_directories(audio PRIVATE include/loopback)
target_include_directories(audio PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(audio PUBLIC core)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")

    target_compile_definitions(audio PRIVATE
            WINDOWS_AUDIO_LOOPBACK
    )
    target_sources(audio PRIVATE
            windows_loopback.h windows_loopback.cpp
    )
else ()
    message(SEND_ERROR "unsupported platform")
endif ()