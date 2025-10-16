set(SOURCES
    main.cpp
)

set(HEADERS
)

# Demo 可执行程序
add_executable(TemperatureModuleWidget ${SOURCES} ${HEADERS})

target_link_libraries(TemperatureModuleWidget
    PRIVATE
        temperaturemodulewidget
)

# 复制 qss/svg 到构建目录
# configure_file(style.qss style.qss COPYONLY)
# configure_file(lightning.svg lightning.svg COPYONLY)

# -------------------------------
# 封装的库
# -------------------------------
set(COMMON_SOURCES
    temperaturemodulewidget.cpp
)

set(COMMON_HEADERS
    temperaturemodulewidget.h
)

add_library(temperaturemodulewidget SHARED ${COMMON_SOURCES} ${COMMON_HEADERS})

target_link_libraries(temperaturemodulewidget
    PUBLIC
        temperatureprobewidget
        customscrollbar
)

target_include_directories(temperaturemodulewidget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
