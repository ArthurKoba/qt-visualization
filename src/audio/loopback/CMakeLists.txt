target_sources(audio PRIVATE
        types.h
        factory.h factory.cpp
)


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