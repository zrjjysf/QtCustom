set(SOURCES
    main.cpp
)

set(HEADERS
)

# Demo 可执行程序
add_executable(TemperatureProbeWidget ${SOURCES} ${HEADERS})

target_link_libraries(TemperatureProbeWidget
    PRIVATE
        temperatureprobewidget
)

# 复制 qss/svg 到构建目录
# configure_file(style.qss style.qss COPYONLY)
# configure_file(lightning.svg lightning.svg COPYONLY)

# -------------------------------
# 封装的库
# -------------------------------
set(COMMON_SOURCES
    temperatureprobewidget.cpp
)

set(COMMON_HEADERS
    temperatureprobewidget.h
)

add_library(temperatureprobewidget SHARED ${COMMON_SOURCES} ${COMMON_HEADERS})

target_link_libraries(temperatureprobewidget
    PUBLIC
        common
)

target_include_directories(temperatureprobewidget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
