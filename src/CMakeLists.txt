set(APP_NAME "qt-application")

# Включить генерацию позиционно-независимого кода (необходимо для shared libs)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(audio)
add_subdirectory(core)
add_subdirectory(charts)

add_executable(${APP_NAME} main.cpp)

target_link_libraries(${APP_NAME} PUBLIC
        Qt::Charts
        Qt::Core
        Qt::Gui
#        Qt::Multimedia
        audio
        charts
)

target_sources(${APP_NAME} PRIVATE
        app.h app.cpp
#        wasapi.h wasapi.cpp
        #        loopback.hpp loopback.cpp
        #        widget.h widget.cpp
        #        xyseriesiodevice.h xyseriesiodevice.cpp

        #        charts/amplitudes.cpp charts/amplitudes.h
)




#set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../BDSP")
#add_subdirectory(${LIB_PATH} ${CMAKE_CURRENT_BINARY_DIR}/BDSP)
#target_include_directories(${PROJECT_NAME} PRIVATE ${LIB_PATH}/src)
#target_link_libraries(${PROJECT_NAME} PRIVATE BDSP)
